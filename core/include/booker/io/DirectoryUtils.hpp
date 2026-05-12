#ifndef DIRECTORYUTILS_HPP
#define DIRECTORYUTILS_HPP

#include <filesystem>

bool flattenDirectory(std::filesystem::path const& dirPath);

#endif // DIRECTORYUTILS_HPP