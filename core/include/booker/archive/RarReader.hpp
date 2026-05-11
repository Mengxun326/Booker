#ifndef DEF_RARREADER
#define DEF_RARREADER

#include <ostream>
#include <filesystem>
#include <vector>
#include <utility>
#include <unarr.h>
#include "ArchiveUtils.hpp"

class RarReader
{
	public:
		RarReader(std::filesystem::path const& path);
		~RarReader();
		
		std::vector<EntryInfo> entries() const;
		void extractToStream(EntryInfo const& entry, std::ostream& os) const;
		std::vector<uint8_t> extract(EntryInfo const& entry) const;
		void extractAll(std::filesystem::path const& destination) const;
	
	private:
		std::filesystem::path m_path;
		ar_stream* m_stream;
		ar_archive* m_handle;
		
		std::pair<ar_stream*, ar_archive*> reopen() const;
};

#endif // DEF_RARREADER