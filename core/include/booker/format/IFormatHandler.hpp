#ifndef DEF_IFORMATHANDLER
#define DEF_IFORMATHANDLER

#include <filesystem>
#include <string>
#include <vector>
#include "../document/Metadata.hpp"

struct PageInfo
{
	size_t index;
	std::string mimeType;
	double width = 0.0;
	double height = 0.0;
};

class IFormatHandler
{
	public:
		virtual ~IFormatHandler() = default;
		
		virtual bool canHandle(std::filesystem::path const& file) const = 0;
		virtual std::vector<PageInfo> scanPages(std::filesystem::path const& file) const = 0;
		virtual bool extractMetadata(std::filesystem::path const& file, Metadata& meta) const = 0;
		virtual std::vector<uint8_t> loadPageData(std::filesystem::path const& file, size_t pageIndex) const = 0;
		virtual bool create(std::filesystem::path const& output, std::vector<std::filesystem::path> const& inputs) = 0;
};

#endif // DEF_IFORMATHANDLER