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
        resultsFile.close();
    }

    // To enable SCB use: --scb 1 or --scb
    void Engine::parseArgs(const std::vector<std::string_view>& args) {
        bool shouldSave = false;
        bool shouldLoad = false;
        std::string_view fileName;

        for (int i = 0; i < args.size(); i++) { // flags in every even position: 0, 2, 4 etc...
            ConfigParser::ArgTypes arg = ConfigParser::validateArg(args[i]);

            switch (arg) { // Increments the pointer
                case ConfigParser::ArgTypes::ROUNDS:
                    GameConfig::GameConfig::rounds = ConfigParser::parseParam<int>(args[++i], "--rounds", 0); break;
                case ConfigParser::ArgTypes::REPEATS:
                    GameConfig::GameConfig::repeats = ConfigParser::parseParam<int>(args[++i], "--repeats", 0); break;
                case ConfigParser::ArgTypes::SEED:
                    GameConfig::GameConfig::seed = ConfigParser::parseParam<int>(args[++i], "--seed"); break;
                case ConfigParser::ArgTypes::EPSILON:
                    GameConfig::GameConfig::epsilon = ConfigParser::parseParam<double>(args[++i], "--epsilon", {0,1}); break;
                case ConfigParser::ArgTypes::PAYOFFS:
                    GameConfig::GameConfig::payoffs = ConfigParser::validatePayoffs(args[++i]); break;
                case ConfigParser::ArgTypes::STRATEGIES:
                    strategies = ConfigParser::parseStrategies(args[++i]);
                    break;
                case ConfigParser::ArgTypes::FORMAT:
                    ConfigParser::validateFormat(args[++i]);
                    format = args[i]; break;
                case ConfigParser::ArgTypes::SAVE:
                    if (shouldLoad) throw std::invalid_argument("Cannot use --save and --load at the same time");
                    shouldSave = true;
                    ConfigParser::validateFileName(args[i+1]);
                    fileName = args[++i]; break;
                case ConfigParser::ArgTypes::LOAD:
                    if (shouldSave) throw std::invalid_argument("Cannot use --save and --load at the same time");
                    shouldLoad = true;
                    ConfigParser::validateFileName(args[i+1], true);
                    fileName = args[++i];
                    break;
                case ConfigParser::ArgTypes::EVOLVE:
                    shouldEvolve = true;
                    if (i + 1 >= args.size()) break; // end of arguments
                    if (args[i+1] == "1") i += 1;
                    else if (args[i+1][0] != '-') // allows --evolve without the '1' parameter
                        throw std::invalid_argument("Invalid parameter for --evolve argument: " + std::string(args[i+1]) + "\nUse --evolve 1 or --evolve to enable evolution");
                    break;
                case ConfigParser::ArgTypes::SCB:
                    enableSCB = true;
                    if (i + 1 >= args.size()) break; // end of arguments
                    if (args[i+1] == "1") i += 1;
                    else if (args[i+1][0] != '-') // allows --scb without the '1' parameter
                        throw std::invalid_argument("Invalid parameter for --scb argument: " + std::string(args[i+1]) + "\nUse --scb 1 or --scb to enable evolution");
                    break;
                case ConfigParser::ArgTypes::POPULATION:
                    population = ConfigParser::parseParam<int>(args[++i], "--population"); break;
                case ConfigParser::ArgTypes::GENERATIONS:
                    generations = ConfigParser::parseParam<int>(args[++i], "--generations"); break;
                case ConfigParser::ArgTypes::MUTATION:
                    mutation = ConfigParser::parseParam<double>(args[++i], "--mutation"); break;

            } // shouldLoad should call parseArgs , shouldSave should write it all to filename
        }
        if (shouldSave) saveConfig(fileName, args);
        if (shouldLoad) loadConfig(fileName);
        if (shouldEvolve && strategies.size() < 4)
            throw std::invalid_argument("Not enough strategies provided for evolution: Requires at least 4 distinct strategies. \nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER,FBF,CAUTIOUS");
        if (strategies.empty())
            throw std::invalid_argument("No strategies provided. \nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER,FBF,CAUTIOUS");
        // if enableSCB assume shouldEvolve should also be true
        if (enableSCB && !shouldEvolve) {
            shouldEvolve = true;
            std::cout << "Using --scb, but missing --evolve argument. Simulation will now run with --evolve" << std::endl;
        }
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

        const std::filesystem::path configPath{ROOT/"configurations"}; // to the root of the project - when run with cMake defaults to cmake-build-debug
        std::filesystem::create_directory(configPath);

        std::ofstream file (configPath/fileName.data());
        if (!file.is_open())
            throw std::invalid_argument("Could not open file: " + std::string(fileName));
        file << config;
        std::cout << "Saving config to " << fileName << std::endl;
        file.close();
    }

    void Engine::loadConfig(const std::string_view &fileName) {
        std::string configStr;
        std::vector<std::string_view> config;
        config.reserve(11); // At most 11 arguments
        std::ifstream file (ROOT/fileName.data());

        if (!file.is_open())
            throw std::invalid_argument("Could not open file: " + std::string(fileName));

        std::unique_ptr<std::string> line = std::make_unique<std::string>();
        while (std::getline(file, *line)) { // In case arguments are on separate lines, flattens to a single line
            size_t pos = line->find('\n');
            if (pos != std::string::npos)
                line->replace(pos, 1, ""); // Remove \n
            configStr.append(*line);
            if (line->back() != ' ') configStr.append(" ");
        }
        file.close();

        size_t start = 0; size_t end = configStr.find(' ');
        while (end < std::string::npos) {
            config.emplace_back(std::string_view(configStr).substr(start, end - start));
            start = end + 1;
            end = configStr.find(' ', start);
        }
        parseArgs(config);
    }

    std::filesystem::path Engine::generateFileName() {
        const std::filesystem::path results{ROOT/"results"};
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
                                    dynamic_cast<Tournament::Evolution::EvolutionaryTournament*>( tournament.release()) );

            if (format == "text") {
                auto out = Results::Text::Evolution::Output(std::move(evoTournament), population, generations, mutation, enableSCB);
                file << out;
            }
            if (format == "csv") {
                auto out = Results::CSV::Evolution::Output(std::move(evoTournament), population, generations, mutation, enableSCB);
                file << out;
            }
            if (format == "json") {
                auto out = Results::JSON::Evolution::Output(std::move(evoTournament), population, generations, mutation, enableSCB);
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

