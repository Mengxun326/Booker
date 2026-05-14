#include "../../../include/booker/document/Page.hpp"

#include <utility>
#include "../../../include/booker/format/IFormatHandler.hpp"

Page::Page(size_t index, std::string mimeType, double width, double height, std::shared_ptr<IFormatHandler> handler, std::filesystem::path sourceFile) : m_index(index), m_mime(mimeType), m_width(width), m_height(height), m_handler(std::move(handler)), m_sourceFile(sourceFile) {}


std::vector<uint8_t>& Page::data() const
{
	if(!m_isLoaded && m_handler)
	{
		m_cachedData = m_handler->loadPageData(m_sourceFile, m_index);
		m_isLoaded = true;
	}
	
	return m_cachedData;
}

void Page::clearCache() const
{
	std::vector<uint8_t>().swap(m_cachedData);
	m_isLoaded = false;
}