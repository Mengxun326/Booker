#include "../include/Utilities.hpp"

#include <string>
#include <booker/format/CbzHandler.hpp>
#include <booker/format/CbrHandler.hpp>
#include <booker/format/PdfHandler.hpp>

std::shared_ptr<booker::IFormatHandler> get(std::filesystem::path const& path)
{
	std::string ext = path.extension().string();
	
	for(char& c : ext)
		c = std::tolower(c);
	
	if(ext == ".cbz" || ext == ".zip")
		return std::make_shared<booker::CbzHandler>();
	else if(ext == ".cbr" || ext == ".rar")
		return std::make_shared<booker::CbrHandler>();
	else if(ext == ".pdf")
		return std::make_shared<booker::PdfHandler>();
	
	return nullptr;
}