#include "Checksum.hpp"

#include "PathUtils.hpp"

#include <array>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace fsorter {

std::uint64_t calculateChecksum(const std::filesystem::path &filePath) {
  std::ifstream input(filePath, std::ios::binary);

  if (!input.is_open()) {
    throw std::runtime_error("Cannot open file for checksum: " +
                             pathToString(filePath));
  }

  constexpr std::uint64_t fnvOffsetBasis = 14695981039346656037ull;
  constexpr std::uint64_t fnvPrime = 1099511628211ull;

  std::uint64_t hash = fnvOffsetBasis;
  std::array<char, 4096> buffer{};

  while (input) {
    input.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    const std::streamsize bytesRead = input.gcount();

    for (std::streamsize index = 0; index < bytesRead; ++index) {
      hash ^=
          static_cast<unsigned char>(buffer[static_cast<std::size_t>(index)]);
      hash *= fnvPrime;
    }
  }

  if (input.bad()) {
    throw std::runtime_error("Error while reading file: " +
                             pathToString(filePath));
  }

  return hash;
}

std::string checksumToHex(std::uint64_t checksum) {
  std::ostringstream output;
  output << std::hex << std::setw(16) << std::setfill('0') << checksum;
  return output.str();
}

} // namespace fsorter