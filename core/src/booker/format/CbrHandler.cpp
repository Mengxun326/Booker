#include "../../../include/booker/format/CbrHandler.hpp"

#include <fstream>
#include <vector>
#include <imageinfo.hpp>
#include "../../../include/booker/archive/ArchiveUtils.hpp"
#include "../../../include/booker/archive/RarReader.hpp"
#include "../../../include/booker/archive/RarWriter.hpp"
#include "../../../include/booker/document/Document.hpp"
#include "../../../include/booker/utils/ImageUtils.hpp"
#include "../../../include/booker/utils/StringUtils.hpp"

namespace booker
{
	bool CbrHandler::canHandle(std::filesystem::path const& file) const
	{
		return (detectFormat(file) == ArchiveFormat::Rar4 || detectFormat(file) == ArchiveFormat::Rar5) && (file.extension() == ".cbr" || file.extension() == ".rar");
	}
	
	std::vector<PageInfo> CbrHandler::scanPages(std::filesystem::path const& file) const
	{
		RarReader reader(file);
		std::vector<PageInfo> pages;
		
		for(EntryInfo const& entry : reader.entries())
		{
			if(entry.isDirectory)
				continue;
			
			if(isImageExtension(entry.name))
			{
				PageInfo info;
				info.index = pages.size();
				info.mimeType = mimeFromExtension(entry.name);
				
				std::filesystem::path tempPath = std::filesystem::temp_directory_path() / entry.name;
				std::ofstream ofs(tempPath, std::ios::binary);
				reader.extractToStream(entry, ofs);
				ofs.close();
				
				imageinfo::ImageInfo imgInfo = imageinfo::parse<imageinfo::FilePathReader>(tempPath.string().c_str());
				
				if(imgInfo)
				{
					std::filesystem::remove(tempPath);
					
					info.width = static_cast<double>(imgInfo.size().width);
					info.height = static_cast<double>(imgInfo.size().height);
				}
				else
				{
					info.width = 0.0;
					info.height = 0.0;
				}
				
				pages.push_back(info);
			}
		}
		
		return pages;
	}
	
	bool CbrHandler::extractMetadata(std::filesystem::path const& file, Metadata& meta) const
	{
		// TODO
		meta.setTitle(file.filename().string());
		
		return true;
	}
	
	std::vector<uint8_t> CbrHandler::loadPageData(std::filesystem::path const& file, size_t pageIndex) const
	{
		RarReader reader(file);
		size_t imageCount = 0;
		
		for(EntryInfo const& entry : reader.entries())
		{
			if(entry.isDirectory)
				continue;
			
			if(isImageExtension(entry.name))
			{
				if(imageCount == pageIndex)
					return reader.extract(entry);
				
				++imageCount;
			}
		}
		
		return {};
	}
	
	bool CbrHandler::writeDocument(Document const& doc, std::filesystem::path const& output) const
	{
		RarWriter writer;
		std::vector<std::string> arrayPages;
		
		for(size_t i=0;i<doc.pageCount();++i)
		{
			Page const& page = doc.page(i);
			std::vector<uint8_t> data = page.data();
			
			if(data.empty())
				continue;
			
			std::string ext;
			
			if(page.mimeType() == "image/jpeg")
				ext = ".jpg";
			else if(page.mimeType() == "image/png")
				ext = ".png";
			else
				ext = ".jpg";
			
			std::string fileName = "page_" + fillWithLeadingZeros(static_cast<int>((i + 1)), static_cast<int>(doc.pageCount())) + ext;
			
			std::ofstream ofs(fileName, std::ios::binary);
			ofs.write(reinterpret_cast<char const*>(data.data()), static_cast<std::streamsize>(data.size()));
			ofs.close();
			
			arrayPages.push_back(fileName);
			
			page.clearCache();
		}
		
		writer.compressMultipleFiles(arrayPages, output.filename().string());
		
		for(size_t i=0;i<arrayPages.size();++i)
			std::filesystem::remove(arrayPages[i]);
		
		return true;
	}
}