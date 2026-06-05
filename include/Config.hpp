#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace fsorter {

/**
 * @brief Конфигурация правил сортировки файлов.
 */
struct Config {
  /**
   * @brief Соответствие между расширением файла и категорией.
   *
   * Например: "jpg" -> "images".
   */
  std::unordered_map<std::string, std::string> extensionToCategory;

  /**
   * @brief Множество известных категорий из конфигурационного файла.
   */
  std::unordered_set<std::string> categories;

  /**
   * @brief Категория для файлов с расширениями, которых нет в конфигурации.
   */
  std::string defaultCategory = "other";

  /**
   * @brief Категория для файлов без расширения.
   */
  std::string noExtensionCategory = "no_extension";

  /**
   * @brief Категория для найденных дубликатов.
   */
  std::string duplicateCategory = "duplicates";
};

/**
 * @brief Загружает правила сортировки из конфигурационного файла.
 *
 * Ожидаемый формат строки:
 * category=ext1,ext2,ext3
 *
 * Пустые строки и строки, начинающиеся с символа "#", игнорируются.
 *
 * @param configPath Путь к конфигурационному файлу.
 * @return Загруженная конфигурация сортировки.
 * @throws std::runtime_error Если файл конфигурации не удалось открыть.
 * @throws std::invalid_argument Если файл содержит некорректные правила.
 */
Config loadConfig(const std::filesystem::path &configPath);

/**
 * @brief Находит категорию для указанного расширения файла.
 *
 * Если расширение пустое, возвращается категория для файлов без расширения.
 * Если расширение неизвестно, возвращается категория по умолчанию.
 *
 * @param config Конфигурация сортировки.
 * @param extension Расширение файла с точкой или без точки.
 * @return Название категории для указанного расширения.
 */
std::string findCategoryByExtension(const Config &config,
                                    const std::string &extension);

} // namespace fsorter