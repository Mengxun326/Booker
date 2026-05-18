#ifndef DEF_RARREADER
#define DEF_RARREADER

/*!
\file RarReader.hpp
\brief Define RarReader.
\details This file define RarReader, a basic class to manage RAR reading.
*/

#include <ostream>
#include <filesystem>
#include <vector>
#include <utility>
#include <archive.h>
#include "ArchiveUtils.hpp"

namespace booker
{
	/*!
	\class RarReader RarReader.hpp "core/include/booker/archive/RarReader.hpp"
	\brief A class allowing to read RAR informations, and deflating RAR archive.
	*/
	
	class RarReader
	{
		public:
			/*!
			\brief Constructor.
			\param path (std::filesystem::path) : Archive path.
			\throw RarException Archive cannot be opened.
			*/
			RarReader(std::filesystem::path const& path);
			/*!
			\brief Destructor.
			\details Close archive.
			*/
			~RarReader();
			
			std::vector<EntryInfo> entries();
			/*!
			\brief Extract an entry from archive into a stream.
			\throw RarException The given entry cannot be found.
			*/
			void extractToStream(EntryInfo const& entry, std::ostream& os) const;
			/*!
			\brief Extract an entry from archive into a raw buffer.
			\throw RarException The given entry cannot be found.
			*/
			std::vector<uint8_t> extract(EntryInfo const& entry) const;
			/*!
			\brief Extract the archive.
			\throw RarException A Zip Slip is detected.
			*/
			void extractAll(std::filesystem::path const& destination) const;
		
		private:
			std::filesystem::path m_path;
			archive* m_handle;
			std::vector<EntryInfo> m_cachedEntries;
			bool m_isCachedEntries = false;
			
			archive* reopen() const;
	};
}

#endif // DEF_RARREADER