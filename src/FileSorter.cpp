#include "FileSorter.hpp"

#include "Checksum.hpp"
#include "PathUtils.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace fsorter {

FileSorter::FileSorter(std::filesystem::path inputDirectory, Config config,
                       Logger &logger)
    : inputDirectory_(std::move(inputDirectory)), config_(std::move(config)),
      logger_(logger) {}

SortStatistics FileSorter::sort() {
  SortStatistics statistics;
  std::vector<std::filesystem::path> files = collectFiles();

  for (const std::filesystem::path &filePath : files) {
    try {
      sortSingleFile(filePath, statistics);
    } catch (const std::exception &exception) {
      ++statistics.errors;
      logger_.writeMessage("Error while processing " + pathToString(filePath) +
                           ": " + exception.what());
    }
  }

  return statistics;
}

std::vector<std::filesystem::path> FileSorter::collectFiles() const {
  if (!std::filesystem::exists(inputDirectory_)) {
    throw std::invalid_argument("Input directory does not exist: " +
                                pathToString(inputDirectory_));
  }

  if (!std::filesystem::is_directory(inputDirectory_)) {
    throw std::invalid_argument("Input path is not a directory: " +
                                pathToString(inputDirectory_));
  }

  std::vector<std::filesystem::path> files;

  for (const std::filesystem::directory_entry &entry :
       std::filesystem::directory_iterator(inputDirectory_)) {
    if (entry.is_regular_file()) {
      files.push_back(entry.path());
    }
  }

  std::sort(files.begin(), files.end());
  return files;
}

void FileSorter::sortSingleFile(const std::filesystem::path &filePath,
                                SortStatistics &statistics) {
  ++statistics.processed;

  const std::uint64_t checksum = calculateChecksum(filePath);
  const std::string extension = getFileExtension(filePath);
  const std::string category = findCategoryByExtension(config_, extension);

  if (extension.empty()) {
    ++statistics.withoutExtension;
  } else if (category == config_.defaultCategory) {
    ++statistics.unknownExtension;
  }

  if (seenChecksums_.contains(checksum)) {
    moveToDuplicates(filePath, checksum, statistics);
    return;
  }

  const std::filesystem::path destinationDirectory = inputDirectory_ / category;
  std::filesystem::create_directories(destinationDirectory);

  std::filesystem::path destinationPath =
      destinationDirectory / filePath.filename();
  bool wasRenamed = false;

  if (std::filesystem::exists(destinationPath)) {
    const std::uint64_t existingChecksum = calculateChecksum(destinationPath);

    if (existingChecksum == checksum) {
      moveToDuplicates(filePath, checksum, statistics);
      return;
    }

    destinationPath = makeUniquePath(destinationPath);
    wasRenamed = true;
    ++statistics.renamed;
  }

  std::filesystem::rename(filePath, destinationPath);
  seenChecksums_[checksum] = destinationPath;
  ++statistics.moved;

  logger_.writeFileAction(wasRenamed ? "Переименовано" : "Перемещено", filePath,
                          destinationPath);
}

void FileSorter::moveToDuplicates(const std::filesystem::path &filePath,
                                  std::uint64_t checksum,
                                  SortStatistics &statistics) {
  const std::filesystem::path duplicateDirectory =
      inputDirectory_ / config_.duplicateCategory;
  std::filesystem::create_directories(duplicateDirectory);

  const std::filesystem::path destinationPath =
      makeUniquePath(duplicateDirectory / filePath.filename());

  std::filesystem::rename(filePath, destinationPath);
  seenChecksums_[checksum] = destinationPath;
  ++statistics.duplicates;

  logger_.writeFileAction("Дубликат", filePath, destinationPath);
}

} // namespace fsorter