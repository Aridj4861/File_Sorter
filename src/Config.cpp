#include "Config.hpp"

#include "PathUtils.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace fsorter {

Config loadConfig(const std::filesystem::path &configPath) {
  std::ifstream input(configPath);

  if (!input.is_open()) {
    throw std::runtime_error("Cannot open config file: " +
                             pathToString(configPath));
  }

  Config config;
  std::string line;
  int lineNumber = 0;

  while (std::getline(input, line)) {
    ++lineNumber;

    std::string cleanedLine = trim(line);

    if (cleanedLine.empty() || cleanedLine.front() == '#') {
      continue;
    }

    const std::size_t separatorPosition = cleanedLine.find('=');

    if (separatorPosition == std::string::npos) {
      throw std::invalid_argument("Invalid config line " +
                                  std::to_string(lineNumber) + ": missing '='");
    }

    std::string category = trim(cleanedLine.substr(0, separatorPosition));
    std::string rawExtensions = trim(cleanedLine.substr(separatorPosition + 1));

    if (category.empty()) {
      throw std::invalid_argument("Invalid config line " +
                                  std::to_string(lineNumber) +
                                  ": category is empty");
    }

    std::vector<std::string> extensions = splitCommaSeparated(rawExtensions);
    config.categories.insert(category);

    for (const std::string &extension : extensions) {
      std::string normalized = normalizeExtension(extension);

      if (normalized.empty()) {
        throw std::invalid_argument("Invalid config line " +
                                    std::to_string(lineNumber) +
                                    ": empty extension");
      }

      config.extensionToCategory[normalized] = category;
    }
  }

  if (config.extensionToCategory.empty()) {
    throw std::invalid_argument("Config file does not contain sorting rules");
  }

  return config;
}

std::string findCategoryByExtension(const Config &config,
                                    const std::string &extension) {
  std::string normalized = normalizeExtension(extension);

  if (normalized.empty()) {
    return config.noExtensionCategory;
  }

  const auto iterator = config.extensionToCategory.find(normalized);

  if (iterator != config.extensionToCategory.end()) {
    return iterator->second;
  }

  return config.defaultCategory;
}

} // namespace fsorter