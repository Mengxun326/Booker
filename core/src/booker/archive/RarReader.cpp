#include "../../../include/booker/archive/RarReader.hpp"

#include <ios>
#include <fstream>
#include <array>
#include "../../../include/booker/utils/OS.hpp"
#include "../../../include/booker/archive/RarException.hpp"

namespace booker
{
	struct ArchiveGuard
	{
		ar_stream* stream;
		ar_archive* archive;
		
		~ArchiveGuard()
		{
			if(archive)
				ar_close_archive(archive);
			
			if(stream)
				ar_close(stream);
		}
	};
	
	RarReader::RarReader(std::filesystem::path const& path) : m_path(path), m_stream(nullptr), m_handle(nullptr)
	{
		#if defined(WINDOWS)
			m_stream = ar_open_file_w(m_path.wstring().c_str());
		#else
			m_stream = ar_open_file(m_path.c_str());
		#endif
		
		if(!m_stream)
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to open file: " + m_path.string());
		
		m_handle = ar_open_rar_archive(m_stream);
		
		if(!m_handle)
		{
			ar_close(m_stream);
			
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to open RAR archive: " + m_path.string());
		}
	}
	
	RarReader::~RarReader()
	{
		if(m_handle)
			ar_close_archive(m_handle);
		
		if(m_stream)
			ar_close(m_stream);
	}
	
	
	std::vector<EntryInfo> RarReader::entries()
	{
		if(m_isCachedEntries)
			return m_cachedEntries;
		
		std::vector<EntryInfo> result;
		
		while(ar_parse_entry(m_handle))
		{
			char const* name = ar_entry_get_name(m_handle);
			
			if(!name)
				continue;
			
			EntryInfo entry;
			entry.name = name;
			entry.uncompressedSize = static_cast<uint64_t>(ar_entry_get_size(m_handle));
			entry.isDirectory = !entry.name.empty() && entry.name.back() == '/';
			
			result.push_back(std::move(entry));
		}
		
		m_cachedEntries = result;
		m_isCachedEntries = true;
		
		return m_cachedEntries;
	}
	
	void RarReader::extractToStream(EntryInfo const& entry, std::ostream& os) const
	{
		auto [stream, archive] = reopen();
		ArchiveGuard guard{ stream, archive };
		
		while(ar_parse_entry(archive))
		{
			char const* name = ar_entry_get_name(archive);
			
			if(!name || entry.name != name)
				continue;
			
			std::array<char, 65536> chunk;
			size_t bytesRead = 0;
			
			while((bytesRead = ar_entry_uncompress(archive, chunk.data(), chunk.size())) > 0)
				os.write(chunk.data(), static_cast<std::streamsize>(bytesRead));
			
			return;
		}
		
		throw MAKE_EXCEPTION(RarException, "ERROR : Entry \"" + entry.name + "\" in archive \"" + m_path.string() + "\".");
	}
	
	std::vector<uint8_t> RarReader::extract(EntryInfo const& entry) const
	{
		auto [stream, archive] = reopen();
		ArchiveGuard guard{ stream, archive };
		
		while(ar_parse_entry(archive))
		{
			char const* name = ar_entry_get_name(archive);
			
			if(!name || entry.name != name)
				continue;
			
			std::vector<uint8_t> buffer(entry.uncompressedSize);
			
			ar_entry_uncompress(archive, buffer.data(), buffer.size());
			
			return buffer;
		}
		
		throw MAKE_EXCEPTION(RarException, "ERROR : Entry \"" + entry.name + "\" in archive \"" + m_path.string() + "\".");
	}
	
	void RarReader::extractAll(std::filesystem::path const& destination) const
	{
		std::filesystem::path const canonicalDest = std::filesystem::weakly_canonical(destination);
		auto [stream, archive] = reopen();
		ArchiveGuard guard{ stream, archive };
		
		while(ar_parse_entry(archive))
		{
			char const* name = ar_entry_get_name(archive);
			
			if(!name)
				continue;
			
			std::string const entryName(name);
			std::filesystem::path const outPath = std::filesystem::weakly_canonical(canonicalDest / entryName);
			
			if(outPath.compare(canonicalDest) != 0)
				throw MAKE_EXCEPTION(RarException, "ERROR : Zip Slip detected for entry \"" + entryName + "\" from archive \"" + m_path.string() + "\".");
			
			if(!entryName.empty() && entryName.back() == '/')
				std::filesystem::create_directories(outPath);
			else
			{
				std::filesystem::create_directories(outPath.parent_path());
				
				std::ofstream ofs(outPath, std::ios::binary);
				ofs.exceptions(std::ios::failbit | std::ios::badbit);
				
				std::array<char, 65536> chunk;
				size_t bytesRead = 0;
				
				while((bytesRead = ar_entry_uncompress(archive, chunk.data(), chunk.size())) > 0)
					ofs.write(chunk.data(), static_cast<std::streamsize>(bytesRead));
			}
		}
	}
	
	
	std::pair<ar_stream*, ar_archive*> RarReader::reopen() const
	{
		#if defined(WINDOWS)
			ar_stream* stream = ar_open_file_w(m_path.wstring().c_str());
		#else
			ar_stream* stream = ar_open_file(m_path.c_str());
		#endif
		
		if(!stream)
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to reopen file: " + m_path.string());
		
		ar_archive* archive = ar_open_rar_archive(stream);
		
		if(!archive)
		{
			ar_close(stream);
			
			throw MAKE_EXCEPTION(RarException, "ERROR : Failed to reopen RAR archive: " + m_path.string());
		}
		
		return { stream, archive };
	}
}