#pragma once

#include "Config.hpp"
#include "Logger.hpp"
#include "Statistics.hpp"

#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace fsorter {

/**
 * @brief Класс, выполняющий сортировку файлов по правилам конфигурации.
 */
class FileSorter {
public:
  /**
   * @brief Создаёт объект сортировщика файлов.
   * @param inputDirectory Папка с файлами для сортировки.
   * @param config Конфигурация правил сортировки.
   * @param logger Логгер для записи действий программы.
   */
  FileSorter(std::filesystem::path inputDirectory, Config config,
             Logger &logger);

  /**
   * @brief Выполняет сортировку файлов в исходной папке.
   * @return Статистика, собранная во время сортировки.
   * @throws std::invalid_argument Если исходный путь не является папкой.
   */
  SortStatistics sort();

private:
  /**
   * @brief Собирает список обычных файлов из исходной папки.
   * @return Вектор путей к найденным файлам.
   * @throws std::invalid_argument Если исходный путь не является папкой.
   */
  std::vector<std::filesystem::path> collectFiles() const;

  /**
   * @brief Сортирует один файл и обновляет статистику.
   * @param filePath Путь к обрабатываемому файлу.
   * @param statistics Объект статистики, который нужно обновить.
   */
  void sortSingleFile(const std::filesystem::path &filePath,
                      SortStatistics &statistics);

  /**
   * @brief Перемещает файл в папку дубликатов.
   * @param filePath Путь к файлу-дубликату.
   * @param checksum Контрольная сумма файла-дубликата.
   * @param statistics Объект статистики, который нужно обновить.
   */
  void moveToDuplicates(const std::filesystem::path &filePath,
                        std::uint64_t checksum, SortStatistics &statistics);

  /**
   * @brief Папка с файлами для сортировки.
   */
  std::filesystem::path inputDirectory_;

  /**
   * @brief Правила сортировки и названия служебных категорий.
   */
  Config config_;

  /**
   * @brief Логгер, используемый сортировщиком.
   */
  Logger &logger_;

  /**
   * @brief Контрольные суммы уже обработанных файлов.
   *
   * Ключом является контрольная сумма, значением — путь к файлу,
   * который был обработан первым.
   */
  std::unordered_map<std::uint64_t, std::filesystem::path> seenChecksums_;
};

} // namespace fsorter