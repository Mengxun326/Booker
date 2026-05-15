#ifndef DEF_ZIPREADER
#define DEF_ZIPREADER

/*!
\file ZipReader.hpp
\brief Define ZipReader.
\details This file define ZipReader, a basic class to manage ZIP reading.
*/

#include <ostream>
#include <filesystem>
#include <vector>
#include "ZipException.hpp"
#include "ArchiveUtils.hpp"

namespace booker
{
	/*!
	\class ZipReader ZipReader.hpp "core/include/booker/archive/ZipReader.hpp"
	\brief A class allowing to read ZIP informations, and deflating ZIP archive.
	*/
	
	class ZipReader
	{
		public:
			/*!
			\brief Constructor.
			\param path (std::filesystem::path) : Archive path.
			\throw ZipException Archive cannot be opened.
			*/
			ZipReader(std::filesystem::path const& path);
			/*!
			\brief Destructor.
			\details Close archive.
			*/
			~ZipReader();
			
			std::vector<EntryInfo> entries() const;
			/*!
			\brief Extract an entry from archive into a stream.
			\throw ZipException The given entry cannot be found, or failed to read it.
			*/
			void extractToStream(EntryInfo const& entry, std::ostream& os) const;
			/*!
			\brief Extract an entry from archive into a stream.
			\throw ZipException The given entry cannot be found, or failed to read it.
			*/
			std::vector<uint8_t> extract(EntryInfo const& entry) const;
			/*!
			\brief Extract the archive.
			\throw ZipException A Zip Slip is detected.
			*/
			void extractAll(std::filesystem::path const& destination) const;
		
		private:
			std::filesystem::path m_path;
			void* m_handle;
	};
}

#endif // DEF_ZIPREADER