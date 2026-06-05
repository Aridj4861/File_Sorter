#include "Statistics.hpp"

#include <ostream>
#include <sstream>

namespace fsorter {

std::string SortStatistics::toString() const {
    std::ostringstream output;

    output << "Итоговая статистика сортировки\n";
    output << "Обработано файлов: " << processed << '\n';
    output << "Перемещено файлов: " << moved << '\n';
    output << "Найдено дубликатов: " << duplicates << '\n';
    output << "Переименовано файлов: " << renamed << '\n';
    output << "Файлов с неизвестным расширением: " << unknownExtension << '\n';
    output << "Файлов без расширения: " << withoutExtension << '\n';
    output << "Ошибок при обработке: " << errors << '\n';

    return output.str();
}

void printStatistics(const SortStatistics& statistics, std::ostream& output) {
    output << statistics.toString();
}

}  // namespace fsorter