#include "../../../include/booker/format/CbzHandler.hpp"

#include <fstream>
#include <vector>
#include <imageinfo.hpp>
#include "../../../include/booker/archive/ArchiveUtils.hpp"
#include "../../../include/booker/archive/ZipReader.hpp"
#include "../../../include/booker/archive/ZipWriter.hpp"
#include "../../../include/booker/document/Document.hpp"
#include "../../../include/booker/utils/ImageUtils.hpp"
#include "../../../include/booker/utils/StringUtils.hpp"

namespace booker
{
	bool CbzHandler::canHandle(std::filesystem::path const& file) const
	{
		return detectFormat(file) == ArchiveFormat::Zip && (file.extension() == ".cbz" || file.extension() == ".zip");
	}
	
	std::vector<PageInfo> CbzHandler::scanPages(std::filesystem::path const& file) const
	{
		ZipReader reader(file);
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
				std::filesystem::create_directories(tempPath.parent_path());
				std::ofstream ofs(tempPath, std::ios::binary);
				reader.extractToStream(entry, ofs);
				ofs.close();
				
				imageinfo::ImageInfo imgInfo = imageinfo::parse<imageinfo::FilePathReader>(tempPath.string().c_str());
				
				if(imgInfo)
				{
					info.width = static_cast<double>(imgInfo.size().width);
					info.height = static_cast<double>(imgInfo.size().height);
				}
				else
				{
					info.width = 0.0;
					info.height = 0.0;
				}
				
				std::filesystem::remove(tempPath);
				
				pages.push_back(info);
			}
		}
		
		return pages;
	}
	
	bool CbzHandler::extractMetadata(std::filesystem::path const& file, Metadata& meta) const
	{
		// TODO
		meta.setTitle(file.filename().string());
		
		return true;
	}
	
	std::vector<uint8_t> CbzHandler::loadPageData(std::filesystem::path const& file, size_t pageIndex) const
	{
		ZipReader reader(file);
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
	
	bool CbzHandler::writeDocument(Document const& doc, std::filesystem::path const& output) const
	{
		ZipWriter writer(output, ZipCreationOpt::CREATE);
		
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
			
			writer.addFile(fileName, data);
			
			page.clearCache();
		}
		
		return true;
	}
}