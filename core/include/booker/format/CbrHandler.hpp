#ifndef DEF_CBRHANDLER
#define DEF_CBRHANDLER

/** @file CbrHandler.hpp
 *  @brief Handler for CBR comic archives (RAR containers of images).
 */

#include "IFormatHandler.hpp"

namespace booker
{
	/** @brief Reads and writes CBR files (.cbr, .rar). */
	class CbrHandler : public IFormatHandler
	{
		public:
			virtual bool canHandle(std::filesystem::path const& file) const override;
			virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const override;
			virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const override;
			virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const override;
			virtual bool writeDocument(Document const& doc, std::filesystem::path const& output) const override;
	};
}

#endif // DEF_CBRHANDLER
