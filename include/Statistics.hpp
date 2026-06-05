#pragma once

#include <cstddef>
#include <iosfwd>
#include <string>

namespace fsorter {

/**
 * @brief Статистика, собранная во время сортировки файлов.
 */
struct SortStatistics {
  /**
   * @brief Количество файлов, выбранных для обработки.
   */
  std::size_t processed = 0;

  /**
   * @brief Количество успешно перемещённых файлов.
   */
  std::size_t moved = 0;

  /**
   * @brief Количество найденных файлов-дубликатов.
   */
  std::size_t duplicates = 0;

  /**
   * @brief Количество файлов, переименованных из-за конфликта имён.
   */
  std::size_t renamed = 0;

  /**
   * @brief Количество файлов с расширениями, отсутствующими в конфигурации.
   */
  std::size_t unknownExtension = 0;

  /**
   * @brief Количество файлов без расширения.
   */
  std::size_t withoutExtension = 0;

  /**
   * @brief Количество ошибок, обработанных во время сортировки.
   */
  std::size_t errors = 0;

  /**
   * @brief Преобразует статистику в текст для вывода.
   * @return Строковое представление статистики.
   */
  std::string toString() const;
};

/**
 * @brief Выводит статистику сортировки в указанный поток.
 * @param statistics Объект со статистикой сортировки.
 * @param output Поток вывода.
 */
void printStatistics(const SortStatistics &statistics, std::ostream &output);

} // namespace fsorter