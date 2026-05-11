#include "../../../include/booker/archive/ArchiveUtils.hpp"

#include <array>
#include <fstream>
#include "../../../include/booker/utils/Exception.hpp"

ArchiveFormat detectFormat(std::filesystem::path const& path)
{
	std::ifstream ifs(path, std::ios::binary);
	
	if(!ifs.is_open())
		throw MAKE_EXCEPTION(Exception, "ERROR : Failed to open file: " + path.string());
	
	std::array<uint8_t, 8> magic{};
	
	ifs.read(reinterpret_cast<char*>(magic.data()), magic.size());
	
	std::streamsize const bytesRead = ifs.gcount();
	
	// ZIP
	if(bytesRead >= 4 && magic[0] == 0x50 && magic[1] == 0x4B && magic[2] == 0x03 && magic[3] == 0x04)
		return ArchiveFormat::Zip;
	
	// RAR5
	if(bytesRead >= 8 && magic[0] == 0x52 && magic[1] == 0x61 && magic[2] == 0x72 && magic[3] == 0x21 && magic[4] == 0x1A && magic[5] == 0x07 && magic[6] == 0x01 && magic[7] == 0x00)
		return ArchiveFormat::Rar5;
	
	// RAR4
	if(bytesRead >= 7 && magic[0] == 0x52 && magic[1] == 0x61 && magic[2] == 0x72 && magic[3] == 0x21 && magic[4] == 0x1A && magic[5] == 0x07 && magic[6] == 0x00)
		return ArchiveFormat::Rar4;
	
	// PDF
	if(bytesRead >= 4 && magic[0] == 0x25 && magic[1] == 0x50 && magic[2] == 0x44 && magic[3] == 0x46)
		return ArchiveFormat::Pdf;
	
	return ArchiveFormat::Unknown;
}