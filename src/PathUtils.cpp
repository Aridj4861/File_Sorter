#include "PathUtils.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace fsorter {

std::string trim(std::string_view text) {
  std::size_t begin = 0;
  std::size_t end = text.size();

  while (begin < end &&
         std::isspace(static_cast<unsigned char>(text[begin])) != 0) {
    ++begin;
  }

  while (end > begin &&
         std::isspace(static_cast<unsigned char>(text[end - 1])) != 0) {
    --end;
  }

  return std::string{text.substr(begin, end - begin)};
}

std::string normalizeExtension(std::string extension) {
  while (!extension.empty() && extension.front() == '.') {
    extension.erase(extension.begin());
  }

  std::transform(extension.begin(), extension.end(), extension.begin(),
                 [](unsigned char character) {
                   return static_cast<char>(std::tolower(character));
                 });

  return extension;
}

std::string getFileExtension(const std::filesystem::path &filePath) {
  return normalizeExtension(filePath.extension().string());
}

std::vector<std::string> splitCommaSeparated(std::string_view text) {
  std::vector<std::string> values;
  std::stringstream stream{std::string{text}};
  std::string item;

  while (std::getline(stream, item, ',')) {
    std::string cleaned = trim(item);

    if (!cleaned.empty()) {
      values.push_back(cleaned);
    }
  }

  if (values.empty()) {
    throw std::invalid_argument("Comma-separated list does not contain values");
  }

  return values;
}

std::filesystem::path makeUniquePath(const std::filesystem::path &desiredPath) {
  if (!std::filesystem::exists(desiredPath)) {
    return desiredPath;
  }

  const std::filesystem::path parent = desiredPath.parent_path();
  const std::string stem = desiredPath.stem().string();
  const std::string extension = desiredPath.extension().string();

  for (int index = 1; index <= 9999; ++index) {
    std::filesystem::path candidate =
        parent / (stem + "_" + std::to_string(index) + extension);

    if (!std::filesystem::exists(candidate)) {
      return candidate;
    }
  }

  throw std::runtime_error("Cannot generate unique path for: " +
                           pathToString(desiredPath));
}

std::string pathToString(const std::filesystem::path &path) {
  return path.generic_string();
}

} // namespace fsorter