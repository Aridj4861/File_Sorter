#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Config.hpp"
#include "FileSorter.hpp"
#include "Logger.hpp"
#include "Statistics.hpp"

#include <clocale>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

    void configureConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::setlocale(LC_ALL, ".UTF-8");
}

struct CommandLineOptions {
    std::filesystem::path inputDirectory;
    std::filesystem::path configPath;
    std::filesystem::path logPath = "sorter.log";
    bool showHelp = false;
};

void printUsage(std::ostream& output) {
    output << "Использование FileSorter:\n";
    output << "  FileSorter.exe --input <folder> --config <file> --log <file>\n\n";
    output << "Пример:\n";
    output << "  FileSorter.exe --input examples/input --config examples/config.txt "
              "--log sorter.log\n";
}

CommandLineOptions parseArguments(int argc, char* argv[]) {
    CommandLineOptions options;

    if (argc == 1) {
        throw std::invalid_argument("Не переданы аргументы командной строки");
    }

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];

        if (argument == "--help" || argument == "-h") {
            options.showHelp = true;
            return options;
        }

        if (argument == "--input") {
            if (index + 1 >= argc) {
                throw std::invalid_argument("Не указано значение для --input");
            }

            options.inputDirectory = argv[++index];
            continue;
        }

        if (argument == "--config") {
            if (index + 1 >= argc) {
                throw std::invalid_argument("Не указано значение для --config");
            }

            options.configPath = argv[++index];
            continue;
        }

        if (argument == "--log") {
            if (index + 1 >= argc) {
                throw std::invalid_argument("Не указано значение для --log");
            }

            options.logPath = argv[++index];
            continue;
        }

        throw std::invalid_argument("Неизвестный аргумент командной строки: " + argument);
    }

    if (options.inputDirectory.empty()) {
        throw std::invalid_argument("Не указана исходная папка");
    }

    if (options.configPath.empty()) {
        throw std::invalid_argument("Не указан конфигурационный файл");
    }

    return options;
}

}  // namespace

int main(int argc, char* argv[]) {
    configureConsoleEncoding();

    try {
        const CommandLineOptions options = parseArguments(argc, argv);

        if (options.showHelp) {
            printUsage(std::cout);
            return 0;
        }

        fsorter::Config config = fsorter::loadConfig(options.configPath);
        fsorter::Logger logger(options.logPath);
        fsorter::FileSorter sorter(options.inputDirectory, config, logger);

        const fsorter::SortStatistics statistics = sorter.sort();
        fsorter::printStatistics(statistics, std::cout);

        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Ошибка: " << exception.what() << "\n\n";
        printUsage(std::cerr);
        return 1;
    }
}