#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "Checksum.hpp"
#include "Config.hpp"
#include "FileSorter.hpp"
#include "Logger.hpp"
#include "PathUtils.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace {

std::filesystem::path prepareTempDirectory(const std::string &name) {
  std::filesystem::path directory =
      std::filesystem::temp_directory_path() / name;
  std::filesystem::remove_all(directory);
  std::filesystem::create_directories(directory);
  return directory;
}

void writeFile(const std::filesystem::path &path, const std::string &content) {
  std::filesystem::create_directories(path.parent_path());

  std::ofstream output(path, std::ios::binary);
  REQUIRE(output.is_open());

  output << content;
  REQUIRE(output.good());
}

} // namespace

TEST_CASE("extension normalization works for positive and negative cases") {
  CHECK(fsorter::normalizeExtension(".JPG") == "jpg");
  CHECK(fsorter::normalizeExtension("PNG") == "png");
  CHECK(fsorter::normalizeExtension("") == "");
  CHECK(fsorter::getFileExtension("archive.TXT") == "txt");
  CHECK(fsorter::getFileExtension("README") == "");
}

TEST_CASE("comma-separated parser handles valid and invalid input") {
  const std::vector<std::string> values =
      fsorter::splitCommaSeparated(" jpg, png ,gif ");

  REQUIRE(values.size() == 3);
  CHECK(values[0] == "jpg");
  CHECK(values[1] == "png");
  CHECK(values[2] == "gif");

  CHECK_THROWS_AS(fsorter::splitCommaSeparated(" , , "), std::invalid_argument);
}

TEST_CASE("unique path generator returns free names") {
  const std::filesystem::path directory =
      prepareTempDirectory("file_sorter_unique_path_test");

  const std::filesystem::path report = directory / "report.txt";
  const std::filesystem::path reportOne = directory / "report_1.txt";

  writeFile(report, "first");
  writeFile(reportOne, "second");

  CHECK(fsorter::makeUniquePath(directory / "free.txt") ==
        directory / "free.txt");
  CHECK(fsorter::makeUniquePath(report).filename().string() == "report_2.txt");
}

TEST_CASE("config loader reads valid config and rejects invalid config") {
  const std::filesystem::path directory =
      prepareTempDirectory("file_sorter_config_test");

  const std::filesystem::path goodConfig = directory / "good_config.txt";
  const std::filesystem::path badConfig = directory / "bad_config.txt";

  writeFile(goodConfig, "images=jpg,png\ntexts=txt,md\n");
  writeFile(badConfig, "images jpg png\n");

  const fsorter::Config config = fsorter::loadConfig(goodConfig);

  CHECK(fsorter::findCategoryByExtension(config, ".JPG") == "images");
  CHECK(fsorter::findCategoryByExtension(config, "txt") == "texts");
  CHECK(fsorter::findCategoryByExtension(config, "unknown") == "other");
  CHECK(fsorter::findCategoryByExtension(config, "") == "no_extension");

  CHECK_THROWS_AS(fsorter::loadConfig(badConfig), std::invalid_argument);
  CHECK_THROWS_AS(fsorter::loadConfig(directory / "missing.txt"),
                  std::runtime_error);
}

TEST_CASE("checksum detects equal and different files") {
  const std::filesystem::path directory =
      prepareTempDirectory("file_sorter_checksum_test");

  const std::filesystem::path first = directory / "first.txt";
  const std::filesystem::path second = directory / "second.txt";
  const std::filesystem::path third = directory / "third.txt";

  writeFile(first, "same content");
  writeFile(second, "same content");
  writeFile(third, "different content");

  CHECK(fsorter::calculateChecksum(first) ==
        fsorter::calculateChecksum(second));
  CHECK(fsorter::calculateChecksum(first) != fsorter::calculateChecksum(third));
  CHECK_THROWS_AS(fsorter::calculateChecksum(directory / "missing.txt"),
                  std::runtime_error);
}

TEST_CASE("file sorter moves files according to rules and detects duplicates") {
  const std::filesystem::path directory =
      prepareTempDirectory("file_sorter_integration_test");
  const std::filesystem::path input = directory / "input";

  writeFile(input / "a.txt", "same");
  writeFile(input / "copy.txt", "same");
  writeFile(input / "photo.JPG", "image");
  writeFile(input / "unknown.xyz", "unknown");
  writeFile(input / "README", "no extension");

  fsorter::Config config;
  config.extensionToCategory["txt"] = "documents";
  config.extensionToCategory["jpg"] = "images";
  config.categories.insert("documents");
  config.categories.insert("images");

  fsorter::Logger logger(directory / "sorter.log");
  fsorter::FileSorter sorter(input, config, logger);

  const fsorter::SortStatistics statistics = sorter.sort();

  CHECK(statistics.processed == 5);
  CHECK(statistics.duplicates == 1);
  CHECK(statistics.unknownExtension == 1);
  CHECK(statistics.withoutExtension == 1);
  CHECK(statistics.errors == 0);

  CHECK(std::filesystem::exists(input / "documents" / "a.txt"));
  CHECK(std::filesystem::exists(input / "duplicates" / "copy.txt"));
  CHECK(std::filesystem::exists(input / "images" / "photo.JPG"));
  CHECK(std::filesystem::exists(input / "other" / "unknown.xyz"));
  CHECK(std::filesystem::exists(input / "no_extension" / "README"));
}