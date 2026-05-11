#ifndef DEF_ZIPWRITER
#define DEF_ZIPWRITER

#include <filesystem>
#include <string_view>
#include <vector>
#include "ZipException.hpp"
#include <zip.h>

enum class ZipCreationOpt
{
	CREATE, // Creation of a new archive.
	ADD, // Add in existing archive.
	OVERWRITE // Overwrite existing archive.
};

class ZipWriter
{
	public:
		ZipWriter(std::filesystem::path const& path, ZipCreationOpt option = ZipCreationOpt::CREATE);
		~ZipWriter();
		
		void addFile(std::string_view entryName, std::vector<uint8_t> const& data, int compressionLevel = Z_DEFAULT_COMPRESSION);
		void addFile(std::string_view entryName, std::filesystem::path const& sourcePath, int compressionLevel = Z_DEFAULT_COMPRESSION);
	
	private:
		std::filesystem::path m_path;
		ZipCreationOpt m_option;
		void* m_handle;
};

#endif // DEF_ZIPWRITER