#include "../../../include/booker/archive/ZipWriter.hpp"

#include <fstream>
#include <array>
#include "../../../include/booker/utils/OS.hpp"

#if defined(WINDOWS)
	#include <iowin32.h>
#endif

ZipWriter::ZipWriter(std::filesystem::path const& path, ZipCreationOpt option) : m_path(path), m_option(option), m_handle(nullptr)
{
	int openingOption = 0;
	
	if(m_option == ZipCreationOpt::CREATE)
		openingOption = APPEND_STATUS_CREATE;
	else
		openingOption = (m_option == ZipCreationOpt::ADD ? APPEND_STATUS_ADDINZIP : APPEND_STATUS_CREATEAFTER);
	
	#if defined(WINDOWS)
		zlib_filefunc64_def ffunc;
		fill_win32_filefunc64W(&ffunc);
		m_handle = zipOpen2_64(m_path.wstring().c_str(), openingOption, nullptr, &ffunc);
	#else
		m_handle = zipOpen64(m_path.c_str(), openingOption);
	#endif
	
	if(!m_handle)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to create archive: " + m_path.string());
}

ZipWriter::~ZipWriter()
{
	if(m_handle)
		zipClose(m_handle, nullptr);
}


void ZipWriter::addFile(std::string_view entryName, std::vector<uint8_t> const& data, int compressionLevel)
{
	if(compressionLevel < 0 || compressionLevel > 9)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Invalid compression level for archive: " + m_path.string() + " (must be between 0 and 9).");
	
	zip_fileinfo fi{};
	fi.dosDate = 0;
	
	if(zipOpenNewFileInZip64(
		m_handle,
		std::string(entryName).c_str(),
		&fi,
		nullptr, 0,
		nullptr, 0,
		nullptr,
		Z_DEFLATED,
		compressionLevel,
		1
		) != ZIP_OK)
	{
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open entry \"" + std::string(entryName) + "\" for writing in archive: \"" + m_path.string() + "\".");
	}
	
	if(zipWriteInFileInZip(m_handle, data.data(), static_cast<unsigned int>(data.size())) != ZIP_OK)
	{
		zipCloseFileInZip(m_handle);
		
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to write entry \"" + std::string(entryName) + "\" in archive: \"" + m_path.string() + "\".");
	}
	
	zipCloseFileInZip(m_handle);
}

void ZipWriter::addFile(std::string_view entryName, std::filesystem::path const& sourcePath, int compressionLevel)
{
	if(compressionLevel < 0 || compressionLevel > 9)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Invalid compression level for archive: " + m_path.string() + " (must be between 0 and 9).");
	
	std::ifstream ifs(sourcePath, std::ios::binary);
	ifs.exceptions(std::ios::failbit | std::ios::badbit);
	
	if(!ifs.is_open())
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open file \"" + sourcePath.string() + "\" for writing in archive: \"" + m_path.string() + "\".");
	
	zip_fileinfo fi{};
	
	if(zipOpenNewFileInZip64(
		m_handle,
		std::string(entryName).c_str(),
		&fi,
		nullptr, 0,
		nullptr, 0,
		nullptr,
		Z_DEFLATED,
		compressionLevel,
		1
		) != ZIP_OK)
	{
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open entry \"" + std::string(entryName) + "\" for writing in archive: \"" + m_path.string() + "\".");
	}
	
	std::array<char, 65536> chunk;
	
	while(ifs.read(chunk.data(), chunk.size()) || ifs.gcount() > 0)
	{
		if(zipWriteInFileInZip(m_handle, chunk.data(), static_cast<unsigned int>(ifs.gcount())) != ZIP_OK)
		{
			zipCloseFileInZip(m_handle);
			
			throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to write entry \"" + std::string(entryName) + "\" in archive: \"" + m_path.string() + "\".");
		}
	}
	
	zipCloseFileInZip(m_handle);
}