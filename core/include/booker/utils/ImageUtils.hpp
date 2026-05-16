#ifndef IMAGEUTILS_HPP
#define IMAGEUTILS_HPP

/*!
\file ImageUtils.hpp
\brief Define utils functions related to images.
*/

#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <poppler/cpp/poppler-page.h>

namespace booker
{
	bool isImageExtension(std::filesystem::path const& filePath);
	std::string mimeFromExtension(std::filesystem::path const& filePath);
	std::vector<uint8_t> convertToJPEG(std::vector<uint8_t> const& inputData, std::string const& inputMimeType, int quality = 95);
	std::string convertPdfPageToJPEG(std::shared_ptr<poppler::page> page, int dpi = 300, std::string const& outputPrefix = "page");
	std::vector<uint8_t> loadJpegData(std::filesystem::path const& filePath);
}

#endif // IMAGEUTILS_HPP