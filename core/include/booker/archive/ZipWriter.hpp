#ifndef DEF_ZIPWRITER
#define DEF_ZIPWRITER

/*!
\file ZipWriter.hpp
\brief Define ZipWriter.
\details This file define ZipWriter, a basic class to manage ZIP writing.
*/

#include <filesystem>
#include <string_view>
#include <vector>
#include "ZipException.hpp"
#include <zip.h>

namespace booker
{
	/*!
	\enum ZipCreationOpt
	\brief A simple enum class which defines options to create a ZIP archive.
	*/
	enum class ZipCreationOpt
	{
		CREATE, /*!< Creation of a new archive. */
		ADD, /*!< Add in existing archive. */
		OVERWRITE /*! Overwrite existing archive. */
	};
	
	/*!
	\class ZipWriter ZipWriter.hpp "core/include/booker/archive/ZipWriter.hpp"
	\brief A class allowing to compress RAR.
	*/
	
	class ZipWriter
	{
		public:
			/*!
			\brief Constructor.
			\param path (std::filesystem::path const&) : Archive path.
			\param option (ZipCreationOpt) : Creation option, see \ref ZipCreationOpt.
			\throw ZipException Archive cannot be created.
			*/
			ZipWriter(std::filesystem::path const& path, ZipCreationOpt option = ZipCreationOpt::CREATE);
			/*!
			\brief Destructor.
			\details Close archive.
			*/
			~ZipWriter();
			
			/*!
			\brief Add a file in archive.
			\param entryName (std::string_view) : Name of entry to put in archive.
			\param data (std::vector<uint8_t> const&) : Data to put in archive.
			\param compressionLevel (int) : Compression level of archive ; need to be between 0 and 9.
			\throw ZipException Invalid compression level, failed to open entry, or failed to write entry in archive.
			*/
			void addFile(std::string_view entryName, std::vector<uint8_t> const& data, int compressionLevel = Z_DEFAULT_COMPRESSION);
			/*!
			\brief Add a file in archive.
			\param entryName (std::string_view) : Name of entry to put in archive.
			\param sourcePath (std::filesystem::path const&) : Path of the file to put in archive.
			\param compressionLevel (int) : Compression level of archive ; need to be between 0 and 9.
			\throw ZipException Invalid compression level, failed to open given file, failed to open entry, or failed to write entry in archive.
			*/
			void addFile(std::string_view entryName, std::filesystem::path const& sourcePath, int compressionLevel = Z_DEFAULT_COMPRESSION);
		
		private:
			std::filesystem::path m_path;
			ZipCreationOpt m_option;
			void* m_handle;
	};
}

#endif // DEF_ZIPWRITER