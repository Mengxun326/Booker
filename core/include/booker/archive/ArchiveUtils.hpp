#ifndef ARCHIVEUTILS_HPP
#define ARCHIVEUTILS_HPP

/*!
\file DirectoryUtils.hpp
\brief Define utils functions and structs related to archives.
*/

#include <filesystem>

namespace booker
{
	/*!
	\struct EntryInfo ArchiveUtils.hpp "core/include/booker/archive/ArchiveUtils.hpp"
	\brief A simple struct to describe informations about an archive entry.
	*/
	struct EntryInfo
	{
		std::string name; /*!< Entry name. */
		uint64_t uncompressedSize; /*!< Size when uncompressed. */
		bool isDirectory; /*!< Indicates if it's a directory or not. */
	};
	
	/*!
	\enum ArchiveFormat
	\brief A simple enum class which defines file formats detected.
	*/
	enum class ArchiveFormat
	{
		Zip,
		Rar4,
		Rar5,
		Pdf,
		Unknown
	};
	
	/*!
	\fn ArchiveFormat detectFormat(std::filesystem::path const& path)
	\brief A function which detect format of a given file.
	*/
	ArchiveFormat detectFormat(std::filesystem::path const& path);
}

#endif // ARCHIVEUTILS_HPP