#ifndef DEF_IFORMATHANDLER
#define DEF_IFORMATHANDLER

#include <filesystem>
#include <string>
#include <vector>
#include "../document/Metadata.hpp"

namespace booker
{
	struct PageInfo
	{
		size_t index;
		std::string mimeType;
		double width = 0.0;
		double height = 0.0;
	};
	
	class Document;
	
	class IFormatHandler
	{
		public:
			virtual ~IFormatHandler() = default;
			
			virtual bool canHandle(std::filesystem::path const& file) const = 0;
			virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const = 0;
			virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const = 0;
			virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const = 0;
			virtual bool writeDocument(Document const& doc, std::filesystem::path const& output) const = 0;
	};
}

#endif // DEF_IFORMATHANDLER