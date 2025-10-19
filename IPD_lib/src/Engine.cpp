#include "../include/Engine.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>

#include "../include/ConfigParser.h"
#include "../include/MathUtil.h"
#include "../include/RandomUtil.h"
using json = nlohmann::json;

namespace Engine {

    void Engine::runIPD() {
        std::vector<Tournament::MatchResults>& matches = tournament->simulateTournament();
        outputResults();
    }

    void Engine::parseArgs(const std::vector<std::string_view>& args) {
        bool shouldSave = false;
        bool shouldLoad = false;
        std::string_view fileName;

        for (int i = 0; i < args.size(); i++) { // flags in every even position: 0, 2, 4 etc...
            ArgTypes arg = ConfigParser::validateArg(args[i]);

            switch (arg) { // Increments the pointer
                case ArgTypes::ROUNDS:
                    GameConfig::GameConfig::rounds = ConfigParser::parseParam<int>(args[++i], "--rounds", 0); break;
                case ArgTypes::REPEATS:
                    GameConfig::GameConfig::repeats = ConfigParser::parseParam<int>(args[++i], "--repeats", 0); break;
                case ArgTypes::SEED:
                    GameConfig::GameConfig::seed = ConfigParser::parseParam<int>(args[++i], "--seed"); break;
                case ArgTypes::EPSILON:
                    GameConfig::GameConfig::epsilon = ConfigParser::parseParam<double>(args[++i], "--epsilon", {0,1}); break;
                case ArgTypes::PAYOFFS:
                    GameConfig::GameConfig::payoffs = ConfigParser::validatePayoffs(args[++i]); break;
                case ArgTypes::STRATEGIES:
                    strategies = ConfigParser::parseStrategies(args[++i]);
                    break;
                case ArgTypes::FORMAT:
                    ConfigParser::validateFormat(args[++i]);
                    GameConfig::GameConfig::format = args[i]; break;
                case ArgTypes::SAVE:
                    if (shouldLoad) throw std::invalid_argument("Cannot use --save and --load at the same time");
                    shouldSave = true;
                    ConfigParser::validateFileName(args[i+1]);
                    fileName = args[++i]; break;
                case ArgTypes::LOAD:
                    if (shouldSave) throw std::invalid_argument("Cannot use --save and --load at the same time");
                    shouldLoad = true;
                    ConfigParser::validateFileName(args[i+1], true);
                    fileName = args[++i];
                    break;
                case ArgTypes::EVOLVE:
                    shouldEvolve = true;
                    if (i + 1 >= args.size()) break;
                    if (args[i+1] == "1") i += 1;
                    else if (args[i+1][0] != '-')
                        throw std::invalid_argument("Invalid parameter for --evolve argument: " + std::string(args[i+1]) + "\nUse --evolve 1 or --evolve to enable evolution");
                    break;
                case ArgTypes::POPULATION:
                    population = ConfigParser::parseParam<int>(args[++i], "--population"); break;
                case ArgTypes::GENERATIONS:
                    generations = ConfigParser::parseParam<int>(args[++i], "--generations"); break;
                case ArgTypes::MUTATION:
                    mutation = ConfigParser::parseParam<double>(args[++i], "--mutation"); break;

            } // shouldLoad should call parseArgs , shouldSave should write it all to filename
        }
        if (shouldSave) saveConfig(fileName, args);
        if (shouldLoad) loadConfig(fileName);

    }

    // files used for saveConfig and loadConfig shouldn't include --save and --load arguments to avoid circular calls

    void Engine::saveConfig(const std::string_view& fileName, const std::vector<std::string_view>& args) {
        std::string config;
        for ( int i = 0; i < args.size(); i++ ) {
            if (args[i] == "--save" || args[i] == "--load")
                i += 1;
            else
                config += std::string(args[i]) + " "; // All arguments on a single line
        }

        std::ofstream file (fileName.data());
        if (!file.is_open())
            throw std::invalid_argument("Could not open file: " + std::string(fileName));
        file << config;
        file.close();
    }

    void Engine::loadConfig(const std::string_view &fileName) {
        std::string configStr;
        std::vector<std::string_view> config;
        config.reserve(11); // At most 11 arguments
        std::ifstream file (fileName.data());

        if (!file.is_open())
            throw std::invalid_argument("Could not open file: " + std::string(fileName));

        std::unique_ptr<std::string> line = std::make_unique<std::string>();
        while (std::getline(file, *line)) { // In case arguments are on separate lines, flattens to a single line
            size_t pos = line->find('\n');
            if (pos != std::string::npos)
                line->replace(pos, 1, ""); // Remove \n
            configStr.append(*line);
            configStr.append(" ");
        }
        file.close();
        configStr.pop_back(); // Remove last space

        size_t start = 0; size_t end = configStr.find(' ');
        while (end < std::string::npos) {
            config.emplace_back(std::string_view(configStr).substr(start, end - start));
            start = end + 1;
            end = configStr.find(' ', start);
        }
        config.emplace_back(std::string_view(configStr).substr(start, end - start)); // Add last element

        parseArgs(config);
    }

    void Engine::outputResults() {
        const std::filesystem::path results{"../../results"}; // to the root of the project - when run with cMake defaults to cmake-build-debug
        std::filesystem::create_directory(results);

        std::string filename = "results_";
        {
            struct tm newtime{};
            time_t now;
            char suffix[20];
            time( &now );
            // Convert to local time.
            localtime_s( &newtime, &now );

            std::strftime(suffix, 20, "%y-%m-%d_%H%M%S", &newtime);
            filename.append(suffix);
        }
        std::ofstream file;
        if (GameConfig::GameConfig::format == "text") {
            filename.append(".txt\0");
            file.open(results/filename);
            outputText(file);
        } else if (GameConfig::GameConfig::format == "csv") {
            filename.append(".csv\0");
            file.open(results/filename);
            outputCSV(file);
        } else if (GameConfig::GameConfig::format == "json") {
            filename.append(".json\0");
            file.open(results/filename);
            outputJSON(file);
        }
        file.close();
        std::cout << "Results saved in: " << filename << std::endl;
    }

    namespace Statistics {

        std::ostream& operator<<(std::ostream& os, const OverallStats& stats) {
            os << std::left
                << std::setw(15) << stats.name
                << std::setprecision(4) << stats.mean << " ± " << stats.stdDev
                << " [" << stats.CI.first << "-" << stats.CI.second << "]";
            return os;
        }

    }

    std::unique_ptr<std::vector<Statistics::OverallStats>> Engine::generateLeaderboard() const {
        std::unique_ptr<std::vector<Statistics::OverallStats>> leaderboard = std::make_unique<std::vector<Statistics::OverallStats>>();
        const auto& totalScores = tournament->getTotalScores();
        for (const auto& [strat, scores] : totalScores) {
            Statistics::OverallStats stats{};
            stats.name = strat;
            stats.mean = MathUtil::calculateMean(scores, scores.size());
            stats.stdDev = MathUtil::calculateStdDev(scores, scores.size(), stats.mean);
            stats.CI = MathUtil::calculateCI(stats.mean, stats.stdDev, scores.size() );
            leaderboard->emplace_back(stats);
        }
        return leaderboard;
    }

    std::ostream& Engine::outputText(std::ostream& os) {
        os  << " Tournament Summary " << std::endl
            << "--------------------" << std::endl
            << "Rounds: " << GameConfig::GameConfig::rounds << "\t"
            << "Repeats: " << GameConfig::GameConfig::repeats << "\t"
            << "Seed: " << GameConfig::GameConfig::seed << "\t"
            << "Epsilon: " << GameConfig::GameConfig::epsilon << std::endl
            << "Payoffs: T=" << GameConfig::GameConfig::payoffs[0]
            << ", R=" << GameConfig::GameConfig::payoffs[1]
            << ", P=" << GameConfig::GameConfig::payoffs[2]
            << ", S=" << GameConfig::GameConfig::payoffs[3]
        << std::endl << std::endl;

        outputLeaderboard(os);
        outputPayoffMatrix(os);
        return os;
    }
    // Output example: "ALLC      2.86 ± 0.12 [2.82–2.90]"

    std::ostream& Engine::outputLeaderboard(std::ostream& os) const {
        os  << "Leaderboard " << std::endl
            << std::left << std::setw(3) << " " << std::setw(15) << "Strategy"
            << "Mean ± stdev [95% CI]" << std::endl;

        auto leaderboard = generateLeaderboard();

        std::ranges::sort(*leaderboard,
                          [](const auto& a, const auto& b) { return a.mean > b.mean; });

        int i = 1;
        for (const auto& stats : *leaderboard) {
            os << std::left
            << std::setw(3) << i++
            << stats << std::endl;
        }
        return os << std::endl;

    }

    std::string strategyToString(const StrategyTypes& strategy) {
        switch (strategy) {
            case StrategyTypes::ALLC:
                return "ALLC";
            case StrategyTypes::ALLD:
                return "ALLD";
            case StrategyTypes::CONTRITE:
                return "CONTRITE";
            case StrategyTypes::GRIM:
                return "GRIM";
            case StrategyTypes::PAVLOV:
                return "PAVLOV";
            case StrategyTypes::PROBER:
                return "PROBER";
            case StrategyTypes::RND03:
                return "RND0.3";
            case StrategyTypes::TFT:
                return "TFT";
        }
        throw std::invalid_argument("Invalid Strategy Type");
    }

    std::ostream& Engine::outputPayoffMatrix(std::ostream& os) {
        os  << " Payoff matrix " << std::endl
            << "---------------" << std::endl
            << std::left << std::setw(10) << " " ;

        for (const StrategyTypes& strat : strategies) {
            os  << std::setw(10) << strategyToString(strat);
        }

        const auto& matches  = tournament->getMatchResults();
        std::string header = "";
        for (const auto& match : matches) {
            if (header != match.player1->getName()) {
                header = match.player1->getName();
                os << std::endl << std::setw(10) << header;
            }
            os << std::setw(10) << match.p1Mean;
        }

        return os << std::endl;
    }

    // TODO add evolution parameters
    std::ostream& Engine::outputCSV(std::ostream& os) {
        os << "Strategy, Opponent, Mean, StdDev, CI_LB, CI_UB, Rounds, Repeats, Seed, Epsilon, T, R, P, S" << std::endl;
        for (const auto& match : tournament->getMatchResults()) {
            os << match;
        }
        return os;
    }

    std::ostream& Engine::outputJSON(std::ostream& os) {
        json output;
        output["metadata"] = {
            {"rounds", GameConfig::GameConfig::rounds},
            {"repeats", GameConfig::GameConfig::repeats},
            {"seed", GameConfig::GameConfig::seed},
            {"epsilon", GameConfig::GameConfig::epsilon},
            {"payoffs",
                {"T", GameConfig::GameConfig::payoffs[0]},
                {"R", GameConfig::GameConfig::payoffs[1]},
                {"P", GameConfig::GameConfig::payoffs[2]},
                {"S", GameConfig::GameConfig::payoffs[3]},
            }
        };

        const auto leaderboard = generateLeaderboard();

        for (const auto& entry : *leaderboard) {
            output["leaderboard"][entry.name]["mean"] = entry.mean;
            output["leaderboard"][entry.name]["stdev"] = entry.stdDev;
            output["leaderboard"][entry.name]["CI"] = entry.CI;
        }

        const auto& matches  = tournament->getMatchResults();
        std::string header = "";
        for (const auto& match : matches) {
            output["pairwise"][match.player1->getName()][match.player2->getName()] = match.p1Mean;
        }

        os << output;
        return os;
    }

}

