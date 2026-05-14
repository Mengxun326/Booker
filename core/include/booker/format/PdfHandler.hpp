#ifndef DEF_PDFHANDLER
#define DEF_PDFHANDLER

#include "IFormatHandler.hpp"

class PdfHandler : public IFormatHandler
{
	public:
		virtual bool canHandle(std::filesystem::path const& file) const override;
		virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const override;
		virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const override;
		virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const override;
		virtual bool writeDocument(Document const& doc, std::filesystem::path const& output) const override;
};

#endif // DEF_PDFHANDLER