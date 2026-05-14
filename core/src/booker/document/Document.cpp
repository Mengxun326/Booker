#include "../../../include/booker/document/Document.hpp"

#include <utility>
#include "../../../include/booker/format/IFormatHandler.hpp"
#include "../../../include/booker/utils/Exception.hpp"

std::unique_ptr<Document> Document::open(std::filesystem::path path, std::shared_ptr<IFormatHandler> handler)
{
	if(!handler || !handler->canHandle(path))
		return nullptr;
	
	Metadata meta;
	handler->extractMetadata(path, meta);
	
	std::vector<PageInfo> pageInfos = handler->scanPages(path);
	std::vector<Page> pages;
	pages.reserve(pageInfos.size());
	
	for(PageInfo const& info : pageInfos)
		pages.emplace_back(info.index, info.mimeType, info.width, info.height, handler, path);
	
	return std::unique_ptr<Document>(new Document(path, std::move(pages), std::move(meta)));
}


Page const& Document::page(size_t index) const
{
	if(index >= m_pages.size())
		throw MAKE_EXCEPTION(Exception, "ERROR : Page index out of bounds.");
	
	return m_pages[index];
}


Document::Document(std::filesystem::path path, std::vector<Page> pages, Metadata meta) : m_path(std::move(path)), m_pages(std::move(pages)), m_metadata(std::move(meta)) {}