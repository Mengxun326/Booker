#ifndef ARCHIVEUTILS_HPP
#define ARCHIVEUTILS_HPP

#include <filesystem>

struct EntryInfo
{
	std::string name;
	uint64_t uncompressedSize;
	bool isDirectory;
};

enum class ArchiveFormat
{
	Zip,
	Rar4,
	Rar5,
	Pdf,
	Unknown
};

ArchiveFormat detectFormat(std::filesystem::path const& path);

#endif // ARCHIVEUTILS_HPP