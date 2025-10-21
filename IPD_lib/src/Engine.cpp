#include "../include/Engine.h"

#include <fstream>
#include <stdexcept>

#include "../include/ConfigParser.h"
#include "../include/Output/Text.h"
#include "../include/Output/CSV.h"
#include "../include/Output/JSON.h"

namespace Engine {

    void Engine::runIPD() {
        tournament->simulateTournament();


        const std::filesystem::path resultsFilePath = generateFileName();
        std::ofstream resultsFile(resultsFilePath);
        outputResults(resultsFile);
        std::cout << "Results saved in: " << resultsFilePath << std::endl;
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
                    format = args[i]; break;
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
        if (shouldEvolve && strategies.size() < 4)
            throw std::invalid_argument("Not enough strategies provided for evolution: Requires at least 4 distinct strategies. \nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER");

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

    std::filesystem::path Engine::generateFileName() {
        const std::filesystem::path results{"..\\..\\results"}; // to the root of the project - when run with cMake defaults to cmake-build-debug
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

        // Extensions
        if (format == "text")
            filename.append(".txt");
        if (format == "csv")
            filename.append(".csv");
        if (format == "json")
            filename.append(".json");

        return results/filename;
    }

    // Transfers ownership of tournament ptr
    std::ofstream& Engine::outputResults(std::ofstream& file) {
        if (shouldEvolve) {
            auto evoTournament = std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>(
                                    static_cast<Tournament::Evolution::EvolutionaryTournament*>( tournament.release()) );

            if (format == "text") {
                auto out = Results::Text::Evolution::Output(std::move(evoTournament), population, generations, mutation);
                file << out;
            }
            if (format == "csv") {
                auto out = Results::CSV::Evolution::Output(std::move(evoTournament), population, generations, mutation);
                file << out;
            }
            if (format == "json") {
                auto out = Results::JSON::Evolution::Output(std::move(evoTournament), population, generations, mutation);
                file << out;
            }
        }
        else {
            if (format == "text") {
                auto out = Results::Text::Output(std::move(tournament));
                file << out;
            }
            else if (format == "csv") {
                auto out = Results::CSV::Output(std::move(tournament));
                file << out;
            }
            else if (format == "json") {
                auto out = Results::JSON::Output(std::move(tournament));
                file << out;
            }
        }
        return file;
    }
}

