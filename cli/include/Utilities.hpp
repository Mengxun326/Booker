#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <filesystem>
#include <memory>
#include <booker/format/IFormatHandler.hpp>

std::shared_ptr<booker::IFormatHandler> get(std::filesystem::path const& path);
static inline std::shared_ptr<booker::IFormatHandler> getReader(std::filesystem::path const& path) { return get(path); }
static inline std::shared_ptr<booker::IFormatHandler> getWriter(std::filesystem::path const& path) { return get(path); }

#endif // UTILITIES_HPP