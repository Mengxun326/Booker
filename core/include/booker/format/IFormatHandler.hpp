#ifndef DEF_IFORMATHANDLER
#define DEF_IFORMATHANDLER

/** @file IFormatHandler.hpp
 *  @brief Interface for reading and writing supported document formats.
 */

#include <filesystem>
#include <string>
#include <vector>
#include "../document/Metadata.hpp"

namespace booker
{
	/** @brief Summary of a single page inside a document file. */
	struct PageInfo
	{
		size_t index;           /**< Zero-based page index. */
		std::string mimeType;   /**< MIME type of the page image data. */
		double width = 0.0;     /**< Page width in pixels, when known. */
		double height = 0.0;    /**< Page height in pixels, when known. */
	};
	
	class Document;
	
	/** @brief Abstract handler for a supported ebook or comic archive format. */
	class IFormatHandler
	{
		public:
			virtual ~IFormatHandler() = default;
			
			/** @brief Returns whether this handler can process the given file path. */
			virtual bool canHandle(std::filesystem::path const& file) const = 0;
			
			/** @brief Lists pages and their properties without loading full image data. */
			virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const = 0;
			
			/** @brief Fills @p meta with metadata extracted from @p file. */
			virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const = 0;
			
			/** @brief Loads the raw image bytes for the page at @p pageIndex. */
			virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const = 0;
			
			/** @brief Writes @p doc to @p output in this handler's format. */
			virtual bool writeDocument(Document const& doc, std::filesystem::path const& output) const = 0;
	};
}

#endif // DEF_IFORMATHANDLER
