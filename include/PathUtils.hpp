#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace fsorter {

/**
 * @brief Удаляет пробельные символы в начале и в конце строки.
 * @param text Исходный текст.
 * @return Строка без начальных и конечных пробелов.
 */
std::string trim(std::string_view text);

/**
 * @brief Нормализует расширение файла.
 *
 * Функция удаляет начальную точку и переводит расширение в нижний регистр.
 * Например, ".JPG" преобразуется в "jpg".
 *
 * @param extension Расширение файла с точкой или без точки.
 * @return Нормализованное расширение без начальной точки.
 */
std::string normalizeExtension(std::string extension);

/**
 * @brief Получает нормализованное расширение из пути к файлу.
 * @param filePath Путь к файлу.
 * @return Нормализованное расширение файла или пустая строка, если расширения
 * нет.
 */
std::string getFileExtension(const std::filesystem::path &filePath);

/**
 * @brief Разделяет строку со значениями, перечисленными через запятую.
 *
 * Каждое полученное значение очищается от пробелов по краям. Пустые значения
 * не добавляются в результат.
 *
 * @param text Строка со значениями, разделёнными запятыми.
 * @return Вектор непустых очищенных значений.
 * @throws std::invalid_argument Если в строке не найдено ни одного значения.
 */
std::vector<std::string> splitCommaSeparated(std::string_view text);

/**
 * @brief Генерирует свободный путь, если исходный путь уже занят.
 *
 * Если файл report.txt уже существует, функция последовательно проверяет
 * варианты report_1.txt, report_2.txt и так далее.
 *
 * @param desiredPath Желаемый путь к файлу.
 * @return Свободный путь, который можно использовать для сохранения или
 * перемещения файла.
 * @throws std::runtime_error Если свободный путь не удалось подобрать.
 */
std::filesystem::path makeUniquePath(const std::filesystem::path &desiredPath);

/**
 * @brief Преобразует путь к строке с прямыми слешами.
 * @param path Исходный путь.
 * @return Строковое представление пути в универсальном формате.
 */
std::string pathToString(const std::filesystem::path &path);

} // namespace fsorter