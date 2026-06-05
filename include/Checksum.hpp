#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace fsorter {

/**
 * @brief Вычисляет 64-битную контрольную сумму файла.
 *
 * Функция использует алгоритм FNV-1a. Контрольная сумма применяется
 * для практического поиска дубликатов и не является криптографической защитой.
 *
 * @param filePath Путь к файлу.
 * @return Значение 64-битной контрольной суммы.
 * @throws std::runtime_error Если файл не удалось открыть или прочитать.
 */
std::uint64_t calculateChecksum(const std::filesystem::path &filePath);

/**
 * @brief Преобразует контрольную сумму в шестнадцатеричную строку.
 * @param checksum Числовое значение контрольной суммы.
 * @return Шестнадцатеричное представление контрольной суммы.
 */
std::string checksumToHex(std::uint64_t checksum);

} // namespace fsorter