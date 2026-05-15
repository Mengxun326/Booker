#ifndef IMAGEUTILS_HPP
#define IMAGEUTILS_HPP

/*!
\file ImageUtils.hpp
\brief Define utils functions related to images.
*/

#include <filesystem>
#include <string>
#include <vector>
#include <cstdint>

namespace booker
{
	bool isImageExtension(std::filesystem::path const& filePath);
	std::string mimeFromExtension(std::filesystem::path const& filePath);
	std::vector<uint8_t> convertToJPEG(std::vector<uint8_t> const& inputData, std::string const& inputMimeType, int quality = 95);
}

#endif // IMAGEUTILS_HPP