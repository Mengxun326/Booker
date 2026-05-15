#include "../../../include/booker/archive/ZipException.hpp"

namespace booker
{
	ZipException::ZipException(char const* what_arg, char const* file, int line, char const* functionName) noexcept : Exception(what_arg, file, line, functionName) {}
	
	ZipException::ZipException(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept : Exception(what_arg, file, line, functionName) {}
	
	ZipException::ZipException(ZipException const& src) noexcept : Exception(src) {}
	
	ZipException::~ZipException() {}
	
	
	ZipException& ZipException::operator=(ZipException const& src) noexcept
	{
		Exception::operator=(src);
		
		return *this;
	}
	
	
	char const* ZipException::what() const noexcept
	{
		return Exception::what();
	}
}