#ifndef DEF_ZIPREADER
#define DEF_ZIPREADER

#include <ostream>
#include <filesystem>
#include <vector>
#include "ZipException.hpp"

struct EntryInfo
{
	std::string name;
	uint64_t uncompressedSize;
	bool isDirectory;
};

class ZipReader
{
	public:
		ZipReader(std::filesystem::path const& path);
		~ZipReader();
		
		std::vector<EntryInfo> entries() const;
		void extractToStream(EntryInfo const& entry, std::ostream& os) const;
		std::vector<uint8_t> extract(EntryInfo const& entry) const;
		void extractAll(std::filesystem::path const& destination) const;
	
	private:
		std::filesystem::path m_path;
		void* m_handle;
};

#endif // DEF_ZIPREADER