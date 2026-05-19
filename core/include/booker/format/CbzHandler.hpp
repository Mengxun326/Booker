#ifndef DEF_CBZHANDLER
#define DEF_CBZHANDLER

/** @file CbzHandler.hpp
 *  @brief Handler for CBZ comic archives (ZIP containers of images).
 */

#include "IFormatHandler.hpp"

namespace booker
{
	/** @brief Reads and writes CBZ files (.cbz, .zip). */
	class CbzHandler : public IFormatHandler
	{
		public:
			virtual bool canHandle(std::filesystem::path const& file) const override;
			virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const override;
			virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const override;
			virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const override;
			virtual bool writeDocument(Document const& doc, std::filesystem::path const& output) const override;
	};
}

#endif // DEF_CBZHANDLER
