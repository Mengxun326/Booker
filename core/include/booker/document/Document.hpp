#ifndef DEF_DOCUMENT
#define DEF_DOCUMENT

#include <filesystem>
#include <vector>
#include <memory>
#include "Metadata.hpp"
#include "Page.hpp"

class IFormatHandler;

class Document
{
	public:
		static std::unique_ptr<Document> open(std::filesystem::path path, std::shared_ptr<IFormatHandler> handler);
		
		inline Metadata const& metadata() const { return m_metadata; }
		inline size_t pageCount() const { return m_pages.size(); }
		Page const& page(size_t index) const;
		inline std::filesystem::path const& path() const { return m_path; }
	
	private:
		std::filesystem::path m_path;
		std::vector<Page> m_pages;
		Metadata m_metadata;
		
		Document(std::filesystem::path path, std::vector<Page> pages, Metadata meta);
};

#endif // DEF_DOCUMENT