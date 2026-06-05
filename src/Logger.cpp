#include "Logger.hpp"

#include "PathUtils.hpp"

#include <stdexcept>
#include <string>

namespace fsorter {

Logger::Logger(const std::filesystem::path &logPath) : logPath_(logPath) {
  if (logPath_.has_parent_path()) {
    std::filesystem::create_directories(logPath_.parent_path());
  }

  stream_.open(logPath_, std::ios::out);

  if (!stream_.is_open()) {
    throw std::runtime_error("Cannot open log file: " + pathToString(logPath_));
  }
}

void Logger::writeMessage(std::string_view message) {
  stream_ << message << '\n';

  if (!stream_) {
    throw std::runtime_error("Cannot write to log file: " +
                             pathToString(logPath_));
  }
}

void Logger::writeFileAction(std::string_view action,
                             const std::filesystem::path &source,
                             const std::filesystem::path &destination) {
  writeMessage(std::string{action} + ": " + pathToString(source) + " -> " +
               pathToString(destination));
}

} // namespace fsorter