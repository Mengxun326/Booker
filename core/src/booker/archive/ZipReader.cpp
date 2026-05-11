#include "../../../include/booker/archive/ZipReader.hpp"

#include <fstream>
#include <array>
#include <utility>
#include <unzip.h>
#include "../../../include/booker/utils/OS.hpp"

#if defined(WINDOWS)
	#include <iowin32.h>
#endif

ZipReader::ZipReader(std::filesystem::path const& path) : m_path(path), m_handle(nullptr)
{
	#if defined(WINDOWS)
		zlib_filefunc64_def ffunc;
		fill_win32_filefunc64W(&ffunc);
		m_handle = unzOpen2_64(m_path.wstring().c_str(), &ffunc);
	#else
		m_handle = unzOpen64(m_path.c_str());
	#endif
	
	if(!m_handle)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open archive: " + m_path.string());
}

ZipReader::~ZipReader()
{
	if(m_handle)
		unzClose(m_handle);
}


std::vector<EntryInfo> ZipReader::entries() const
{
	std::vector<EntryInfo> result;
	
	if(unzGoToFirstFile(m_handle) != UNZ_OK)
		return result;
	
	do
	{
		unz_file_info64 info;
		std::string nameBuff(info.size_filename, '\0');
		
		if(unzGetCurrentFileInfo64(m_handle, &info, nameBuff.data(), info.size_filename, nullptr, 0, nullptr, 0) != UNZ_OK)
			continue;
		
		EntryInfo entry;
		entry.name = std::move(nameBuff);
		entry.uncompressedSize = info.uncompressed_size;
		entry.isDirectory = !entry.name.empty() && entry.name.back() == '/';
		
		result.push_back(std::move(entry));
	}
	while(unzGoToNextFile(m_handle) == UNZ_OK);
	
	return result;
}

void ZipReader::extractToStream(EntryInfo const& entry, std::ostream& os) const
{
	if(unzLocateFile(m_handle, entry.name.c_str(), 0) != UNZ_OK)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Entry \"" + entry.name + "\" not found in archive \"" + m_path.string() + "\".");
	
	if(unzOpenCurrentFile(m_handle) != UNZ_OK)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
	
	std::array<char, 65536> chunk;
	int bytesRead = 0;
	
	os.exceptions(std::ios::failbit | std::ios::badbit);
	
	while((bytesRead = unzReadCurrentFile(m_handle, chunk.data(), static_cast<unsigned int>(chunk.size()))) > 0)
		os.write(chunk.data(), bytesRead);
	
	unzCloseCurrentFile(m_handle);
	
	if(bytesRead < 0)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Error reading entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
}

std::vector<uint8_t> ZipReader::extract(EntryInfo const& entry) const
{
	if(unzLocateFile(m_handle, entry.name.c_str(), 0) != UNZ_OK)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Entry \"" + entry.name + "\" not found in archive \"" + m_path.string() + "\".");
	
	if(unzOpenCurrentFile(m_handle) != UNZ_OK)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Failed to open entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
	
	std::vector<uint8_t> buffer(entry.uncompressedSize);
	int bytesRead = unzReadCurrentFile(m_handle, buffer.data(), static_cast<unsigned int>(buffer.size()));
	
	if(bytesRead < 0)
		throw MAKE_EXCEPTION(ZipException, "ERROR : Error reading entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
	
	return buffer;
}

void ZipReader::extractAll(std::filesystem::path const& destination) const
{
	std::filesystem::path const canonicalDest = std::filesystem::weakly_canonical(destination);
	
	for(EntryInfo const& entry : entries())
	{
		std::filesystem::path const outPath = std::filesystem::weakly_canonical(canonicalDest / entry.name);
		
		if(outPath.compare(canonicalDest) != 0)
			throw MAKE_EXCEPTION(ZipException, "ERROR : Zip Slip detected for entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
		
		if(entry.isDirectory)
			std::filesystem::create_directories(outPath);
		else
		{
			std::filesystem::create_directories(outPath.parent_path());
			
			std::ofstream ofs(outPath, std::ios::binary);
			ofs.exceptions(std::ios::failbit | std::ios::badbit);
			
			extractToStream(entry, ofs);
		}
	}
}