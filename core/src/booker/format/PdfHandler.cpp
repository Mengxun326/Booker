#include "../../../include/booker/format/PdfHandler.hpp"

#include <fstream>
#include <vector>
#include <memory>
#include <stdio.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <poppler/cpp/poppler-image.h>
#include <turbojpeg.h>
#include <hpdf.h>
#include "../../../include/booker/archive/ArchiveUtils.hpp"
#include "../../../include/booker/utils/ImageUtils.hpp"
#include "../../../include/booker/document/Document.hpp"

namespace booker
{
	void HPDF_STDCALL haruErrorHandler(HPDF_STATUS errorNo, HPDF_STATUS detailNo, void* userData)
	{
		(void)userData;
		fprintf(stderr, "Libharu error: error_no=%04X, detail_no=%d\n", (unsigned int)errorNo, (int)detailNo);
	}
	
	bool PdfHandler::canHandle(std::filesystem::path const& file) const
	{
		return detectFormat(file) == ArchiveFormat::Pdf && file.extension() == ".pdf";
	}
	
	std::vector<PageInfo> PdfHandler::scanPages(std::filesystem::path const& file) const
	{
		std::vector<PageInfo> pages;
		
		std::unique_ptr<poppler::document> doc = std::unique_ptr<poppler::document>(poppler::document::load_from_file(file.string()));
		
		if(!doc || doc->is_locked())
			return {};
		
		for(int i=0;i<doc->pages();++i)
		{
			std::unique_ptr<poppler::page> page = std::unique_ptr<poppler::page>(doc->create_page(i));
			
			if(!page)
				continue;
			
			PageInfo info;
			info.index = i;
			info.mimeType = "image/jpeg";
			auto pageRect = page->page_rect();
			info.width = pageRect.width();
			info.height = pageRect.height();
			
			pages.push_back(info);
		}
		
		return pages;
	}
	
	bool PdfHandler::extractMetadata(std::filesystem::path const& file, Metadata& meta) const
	{
		// TODO
		meta.setTitle(file.filename().string());
		
		return true;
	}
	
	std::vector<uint8_t> PdfHandler::loadPageData(std::filesystem::path const& file, size_t pageIndex) const
	{
		std::unique_ptr<poppler::document> doc = std::unique_ptr<poppler::document>(poppler::document::load_from_file(file.string()));
		
		if(!doc || doc->is_locked())
			return {};
		
		if(pageIndex >= static_cast<size_t>(doc->pages()))
			return {};
		
		std::unique_ptr<poppler::page> page = std::unique_ptr<poppler::page>(doc->create_page(static_cast<int>(pageIndex)));
		
		if(!page)
			return {};
		
		poppler::page_renderer renderer;
		renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
		renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);
		renderer.set_image_format(poppler::image::format_rgb24);
		
		int const dpi = 300;
		double const scale = static_cast<double>(dpi) / 72.0;
		
		poppler::image img = renderer.render_page(page.get(), scale, scale);
		
		if(!img.is_valid())
			return {};
		
		if(img.format() != poppler::image::format_rgb24)
			return {};
		
		unsigned char const* rgbData = reinterpret_cast<unsigned char const*>(img.const_data());
		int width = img.width();
		int height = img.height();
		
		tjhandle tjInstance = tjInitCompress();
		
		if(!tjInstance)
			return {};
		
		unsigned char* jpegBuf = nullptr;
		unsigned long jpegSize = 0;
		int ret = tjCompress2(tjInstance, rgbData, width, 0, height, TJPF_RGB, &jpegBuf, &jpegSize, TJSAMP_444, 95, TJFLAG_FASTDCT);
		std::vector<uint8_t> result;
		
		if(ret == 0 && jpegBuf != nullptr && jpegSize > 0)
			result.assign(jpegBuf, jpegBuf + jpegSize);
		
		tjFree(jpegBuf);
		tjDestroy(tjInstance);
		
		return result;
	}
	
	bool PdfHandler::writeDocument(Document const& doc, std::filesystem::path const& output) const
	{
		HPDF_Doc pdf = HPDF_New(haruErrorHandler, nullptr);
		
		if(!pdf)
			return false;
		
		Metadata const& meta = doc.metadata();
		
		if(meta.hasTitle())
			HPDF_SetInfoAttr(pdf, HPDF_INFO_TITLE, meta.title().c_str());
		
		if(meta.hasCreator())
			HPDF_SetInfoAttr(pdf, HPDF_INFO_AUTHOR, meta.creator().c_str());
		
		if(meta.hasSubject())
			HPDF_SetInfoAttr(pdf, HPDF_INFO_SUBJECT, meta.subject().c_str());
		
		for(size_t i=0;i<doc.pageCount();++i)
		{
			Page const& page = doc.page(i);
			std::vector<uint8_t> const& data = page.data();
			
			if(data.empty())
				continue;
			
			std::string const& mimeType = page.mimeType();
			std::vector<uint8_t> imageData;
			std::string finalMimeType = mimeType;
			
			if(mimeType != "image/jpeg" && mimeType != "image/png")
			{
				try
				{
					imageData = convertToJPEG(data, mimeType, 95);
					finalMimeType = "image/jpeg";
				}
				catch(std::exception const& err)
				{
					continue;
				}
			}
			else
				imageData = data;
			
			page.clearCache();
			
			HPDF_Image image = nullptr;
			
			if(finalMimeType == "image/jpeg")
				image = HPDF_LoadJpegImageFromMem(pdf, imageData.data(), imageData.size());
			else if(finalMimeType == "image/png")
				image = HPDF_LoadPngImageFromMem(pdf, imageData.data(), imageData.size());
			
			if(!image)
				continue;
			
			HPDF_UINT imgWidth = HPDF_Image_GetWidth(image);
			HPDF_UINT imgHeight = HPDF_Image_GetHeight(image);
			HPDF_Page pdfPage = HPDF_AddPage(pdf);
			
			HPDF_Page_SetWidth(pdfPage, static_cast<HPDF_REAL>(imgWidth));
			HPDF_Page_SetHeight(pdfPage, static_cast<HPDF_REAL>(imgHeight));
			
			HPDF_Page_DrawImage(pdfPage, image, 0, 0, static_cast<HPDF_REAL>(imgWidth), static_cast<HPDF_REAL>(imgHeight));
		}
		
		HPDF_SaveToFile(pdf, output.string().c_str());
		HPDF_Free(pdf);
		
		return true;
	}
}