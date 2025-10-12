#include "../include/Engine.h"

#include <complex>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <unordered_map>



void Engine::parseArgs(const std::vector<std::string_view>& args) {
    bool shouldSave = false;
    bool shouldLoad = false;
    std::string_view fileName;

    for (int i = 0; i < args.size(); i++) { // flags in every even position: 0, 2, 4 etc...
        ArgTypes arg = validateArg(args[i]);

        switch (arg) { // Increments the pointer
            case ArgTypes::ROUNDS:
                rounds = parseParam<int>(args[++i], "--rounds"); break;
            case ArgTypes::REPEATS:
                repeats = parseParam<int>(args[++i], "--repeats"); break;
            case ArgTypes::SEED:
                seed = parseParam<int>(args[++i], "--seed"); break;
            case ArgTypes::EPSILON:
                epsilon = parseParam<float>(args[++i], "--epsilon"); break;
            case ArgTypes::PAYOFFS:
                validatePayoffs(args[++i]); break;
            case ArgTypes::STRATEGIES:
                parseStrategies(args[++i]);
                break;
            case ArgTypes::FORMAT:
                validateFormat(args[++i]);
                format = args[i]; break;
            case ArgTypes::SAVE:
                if (shouldLoad) throw std::invalid_argument("Cannot use --save and --load at the same time");
                shouldSave = true;
                validateFileName(args[i+1]);
                fileName = args[++i]; break;
            case ArgTypes::LOAD:
                if (shouldSave) throw std::invalid_argument("Cannot use --save and --load at the same time");
                shouldLoad = true;
                validateFileName(args[i+1]);
                if (!std::filesystem::exists(args[i+1]))
                    throw std::invalid_argument("File does not exist: " + std::string(args[i+1]));
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
                population = parseParam<int>(args[++i], "--population"); break;
            case ArgTypes::GENERATIONS:
                generations = parseParam<int>(args[++i], "--generations"); break;
            case ArgTypes::MUTATION:
                mutation = parseParam<float>(args[++i], "--mutation"); break;

        } // shouldLoad should call parseArgs , shouldSave should write it all to filename
    }
    if (shouldSave) saveConfig(fileName, args);
    if (shouldLoad) loadConfig(fileName);

}

Engine::ArgTypes Engine::validateArg(std::string_view arg) {
    auto it = argMap.find(arg);

    if (it == argMap.end())
        throw std::invalid_argument("Invalid argument: " + std::string(arg));

    return it->second;
}

void Engine::validateFormat(std::string_view format) {
    if (format != "text" && format != "csv" && format != "json")
        throw std::invalid_argument("Invalid parameter for --format argument: " + std::string(format) + "\nUse --format {text | csv | json}");
}

void Engine::validatePayoffs(const std::string_view &param) {
    auto first = param.data(); auto last = param.data() + param.size();
    float value{};
    auto res = std::from_chars(first, last, value);
    int i = 0;

    while (res.ptr != last) {
        if (res.ec == std::errc::invalid_argument || res.ptr[0] != ',' || value <= 0)
            throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are positive values, integral or floating-point types, each seperated by commas");
        if (res.ec == std::errc::result_out_of_range)
            throw std::invalid_argument("A given value for --payoffs argument is larger than a float: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are integral or floating-point types, each seperated by commas");

        payoffs[i++] = value;
        res = std::from_chars(res.ptr + 1, last, value);
    }
    payoffs[i] = value; // Add last element

    if (i < 3)
        throw std::invalid_argument("Invalid number of parameters for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are positive values, integral or floating-point types, each seperated by commas");
    if (!(payoffs[0] > payoffs[1] && payoffs[1] > payoffs[2] && payoffs[2] > payoffs[3]))
        throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T > R > P > S");
    if (!(2 * payoffs[1] > payoffs[0] + payoffs[3]))
        throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where 2R > T + S");
}

Engine::StrategyTypes Engine::validateStrategies(const std::string_view& strategy){
    auto it = strategyMap.find(strategy);

    if (it == strategyMap.end())
        throw std::invalid_argument("Invalid strategy for --strategies argument: " + std::string(strategy) + "\nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER");

    return it->second;
}

void Engine::parseStrategies(const std::string_view& param) {
    strategies.reserve(10); // At most 10 strategies
    size_t i = 0;
    size_t pos = param.find(',', i);

    while (pos < std::string_view::npos) {
        std::string_view strategy = param.substr(i, pos - i);
        strategies.emplace_back(validateStrategies(strategy));
        i = pos + 1;
        pos = param.find(',', i);
    }
    std::string_view strategy = param.substr(i, pos - i);
    strategies.emplace_back(validateStrategies(strategy));
}

void Engine::validateFileName(const std::string_view& fileName){
    size_t dot = fileName.find('.');
    if (dot == std::string_view::npos)
        throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");
    if (std::strcmp(&fileName.at(dot + 1), "txt") != 0)
        throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");
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


