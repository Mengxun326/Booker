#include "../include/Include.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <chrono>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <poppler/cpp/poppler-image.h>

bool convertPdfToJpg(std::string const& pdfPath, int dpi = 300, std::string const& outputPrefix = "page")
{
	std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(pdfPath));
	
	if(!doc || doc->is_locked())
	{
		std::cerr << "Error : unable to load PDF file or file is locked." << std::endl;
		
		return false;
	}
	
	if(!poppler::page_renderer::can_render())
	{
		std::cerr << "Error : renderer not supported (missing Splash backend)." << std::endl;
		
		return false;
	}
	
	poppler::page_renderer renderer;
	renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
	renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);
	renderer.set_image_format(poppler::image::format_rgb24);
	
	int const numPages = doc->pages();
	
	for(int i=0;i<numPages;++i)
	{
		std::unique_ptr<poppler::page> page(doc->create_page(i));
		
		if(!page)
		{
			std::cerr << "Error : unable to load page " << (i + 1) << std::endl;
			
			continue;
		}
		
		poppler::image img = renderer.render_page(page.get(), dpi, dpi);
		
		if(!img.is_valid())
		{
			std::cerr << "Error : invalid render for page " << (i + 1) << "." << std::endl;
			
			continue;
		}
		
		std::string outFilename = outputPrefix + "_" + std::to_string(i + 1) + ".jpg";
		
		if(!img.save(outFilename, "jpeg"))
			std::cerr << "Error : unable to save " << outFilename << std::endl;
		else
			std::cout << "Successfull conversion of page " << (i + 1) << " : " << outFilename << std::endl;
	}
	
	return true;
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage : " << argv[0] << " [PDF_FILENAME]" << std::endl;
		
		return -1;
	}
	
	std::string pdfFileName = argv[1];
	
	if(convertPdfToJpg(pdfFileName))
		std::cout << "Successfull conversion of \"" << pdfFileName << "\"' pages into JPG." << std::endl;
	else
		std::cerr << "An error occured while converting pages of \"" << pdfFileName << "\" into JPG." << std::endl;
	
	return 0;
}