#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>

namespace fsorter {

/**
 * @brief Класс для записи действий программы в лог-файл.
 */
class Logger {
public:
  /**
   * @brief Открывает лог-файл для записи.
   * @param logPath Путь к лог-файлу.
   * @throws std::runtime_error Если лог-файл не удалось открыть.
   */
  explicit Logger(const std::filesystem::path &logPath);

  /**
   * @brief Записывает обычное текстовое сообщение в лог-файл.
   * @param message Текст сообщения.
   * @throws std::runtime_error Если запись в лог-файл завершилась ошибкой.
   */
  void writeMessage(std::string_view message);

  /**
   * @brief Записывает действие, выполненное над файлом.
   *
   * Формат записи содержит название действия, исходный путь и путь назначения.
   *
   * @param action Название действия.
   * @param source Исходный путь к файлу.
   * @param destination Путь, по которому был перемещён файл.
   * @throws std::runtime_error Если запись в лог-файл завершилась ошибкой.
   */
  void writeFileAction(std::string_view action,
                       const std::filesystem::path &source,
                       const std::filesystem::path &destination);

private:
  /**
   * @brief Поток вывода, связанный с лог-файлом.
   */
  std::ofstream stream_;

  /**
   * @brief Путь к текущему лог-файлу.
   */
  std::filesystem::path logPath_;
};

} // namespace fsorter