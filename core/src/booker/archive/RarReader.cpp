#include "../../../include/booker/archive/RarReader.hpp"

#include <ios>
#include <fstream>
#include <array>
#include <archive_entry.h>
#include "../../../include/booker/utils/OS.hpp"
#include "../../../include/booker/archive/RarException.hpp"

namespace booker
{
	struct ArchiveGuard
	{
		archive* ar;
		
		explicit ArchiveGuard(archive* a) : ar(a) {}
		
		ArchiveGuard(ArchiveGuard const& src) = delete;
		
		~ArchiveGuard()
		{
			if(ar)
				archive_read_free(ar);
		}
		
		ArchiveGuard& operator=(ArchiveGuard const& src) = delete;
	};
	
	RarReader::RarReader(std::filesystem::path const& path) : m_path(path), m_handle(nullptr)
	{
		m_handle = reopen();
	}
	
	RarReader::~RarReader()
	{
		if(m_handle)
			archive_read_free(m_handle);
	}
	
	
	std::vector<EntryInfo> RarReader::entries()
	{
		if(m_isCachedEntries)
			return m_cachedEntries;
		
		archive_entry* ae = nullptr;
		
		while(archive_read_next_header(m_handle, &ae) == ARCHIVE_OK)
		{
			char const* name = archive_entry_pathname(ae);
			
			if(!name)
			{
				archive_read_data_skip(m_handle);
				
				continue;
			}
			
			EntryInfo entry;
			entry.name = name;
			entry.uncompressedSize = archive_entry_size_is_set(ae) ? static_cast<uint64_t>(archive_entry_size(ae)) : 0;
			entry.isDirectory = !entry.name.empty() && entry.name.back() == '/';
			
			m_cachedEntries.push_back(std::move(entry));
			
			archive_read_data_skip(m_handle);
		}
		
		m_isCachedEntries = true;
		
		archive_read_free(m_handle);
		m_handle = nullptr;
		
		return m_cachedEntries;
	}
	
	void RarReader::extractToStream(EntryInfo const& entry, std::ostream& os) const
	{
		if(!os.good())
			throw MAKE_EXCEPTION(RarException, "ERROR : Output stream is not in a good state for archive \"" + m_path.string() + "\".");
		
		archive* ar = reopen();
		ArchiveGuard guard(ar);
		archive_entry* ae = nullptr;
		
		while(archive_read_next_header(ar, &ae) == ARCHIVE_OK)
		{
			char const* name = archive_entry_pathname(ae);
			
			if(!name || entry.name != name)
			{
				archive_read_data_skip(ar);
				
				continue;
			}
			
			std::array<char, 65536> chunk;
			la_ssize_t bytesRead = 0;
			
			os.exceptions(std::ios::failbit | std::ios::badbit);
			
			while((bytesRead = archive_read_data(ar, chunk.data(), chunk.size())) > 0)
				os.write(chunk.data(), bytesRead);
			
			if(bytesRead < 0)
				throw MAKE_EXCEPTION(RarException, "ERROR : Error reading entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
			
			return;
		}
		
		throw MAKE_EXCEPTION(RarException, "ERROR : Entry \"" + entry.name + "\" not found in archive \"" + m_path.string() + "\".");
	}
	
	std::vector<uint8_t> RarReader::extract(EntryInfo const& entry) const
	{
		archive* ar = reopen();
		ArchiveGuard guard(ar);
		archive_entry* ae = nullptr;
		
		while(archive_read_next_header(ar, &ae) == ARCHIVE_OK)
		{
			char const* name = archive_entry_pathname(ae);
			
			if(!name || entry.name != name)
			{
				archive_read_data_skip(ar);
				
				continue;
			}
			
			std::vector<uint8_t> buffer;
			
			if(entry.uncompressedSize > 0)
				buffer.reserve(entry.uncompressedSize);
			
			std::array<char, 65536> chunk;
			la_ssize_t bytesRead = 0;
			
			while((bytesRead = archive_read_data(ar, chunk.data(), chunk.size())) > 0)
				buffer.insert(buffer.end(), chunk.begin(), chunk.begin() + bytesRead);
			
			if(bytesRead < 0)
				throw MAKE_EXCEPTION(RarException, "ERROR : Error reading entry \"" + entry.name + "\" from archive \"" + m_path.string() + "\".");
			
			return buffer;
		}
		
		throw MAKE_EXCEPTION(RarException, "ERROR : Entry \"" + entry.name + "\" not found in archive \"" + m_path.string() + "\".");
	}
	
	void RarReader::extractAll(std::filesystem::path const& destination) const
	{
		std::filesystem::path const canonicalDest = std::filesystem::weakly_canonical(destination);
		archive* ar = reopen();
		ArchiveGuard guard(ar);
		archive_entry* ae = nullptr;
		
		while(archive_read_next_header(ar, &ae) == ARCHIVE_OK)
		{
			char const* name = archive_entry_pathname(ae);
			
			std::string const entryName(name);
			std::filesystem::path const outPath = std::filesystem::weakly_canonical((canonicalDest / entryName));
			
			if(!name || entryName != name)
			{
				archive_read_data_skip(ar);
				
				continue;
			}
			
			if(outPath.string().compare(0, canonicalDest.string().size(), canonicalDest.string()) != 0)
				throw MAKE_EXCEPTION(RarException, "ERROR : Zip Slip detected for entry \"" + entryName + "\" from archive \"" + m_path.string() + "\".");
			
			if(!entryName.empty() && entryName.back() == '/')
			{
				std::filesystem::create_directories(outPath);
				
				continue;
			}
			
			std::filesystem::create_directories(outPath.parent_path());
			
			std::ofstream ofs(outPath, std::ios::binary);
			ofs.exceptions(std::ios::failbit | std::ios::badbit);
			
			std::array<char, 65536> chunk;
			la_ssize_t bytesRead = 0;
			
			while((bytesRead = archive_read_data(ar, chunk.data(), chunk.size())) > 0)
				ofs.write(chunk.data(), bytesRead);
			
			if(bytesRead < 0)
				throw MAKE_EXCEPTION(RarException, "ERROR : Error writing entry \"" + entryName + "\" from archive \"" + m_path.string() + "\".");
		}
	}
	
	
	archive* RarReader::reopen() const
	{
		archive* ar = archive_read_new();
		
		
		if(!ar)
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to allocate archive handle for: " + m_path.string());
		
		archive_read_support_filter_all(ar);
		archive_read_support_format_rar(ar);
		archive_read_support_format_rar5(ar);
		archive_read_support_format_zip(ar);
		
		#if defined(WINDOWS)
			int const rc = archive_read_open_filename_w(ar, m_path.wstring().c_str(), 65536);
		#else
			int const rc = archive_read_open_filename(ar, m_path.string().c_str(), 65536);
		#endif
		
		if(rc != ARCHIVE_OK)
		{
			std::string const msg = archive_error_string(ar) ? archive_error_string(ar) : "unknown error";
			
			archive_read_free(ar);
			
			ar = nullptr;
			
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to open archive \"" + m_path.string()+ "\": " + msg);
		}
		
		return ar;
	}
}