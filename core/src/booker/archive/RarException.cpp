#include "../../../include/booker/archive/RarException.hpp"

namespace booker
{
	RarException::RarException(char const* what_arg, char const* file, int line, char const* functionName) noexcept : Exception(what_arg, file, line, functionName) {}
	
	RarException::RarException(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept : Exception(what_arg, file, line, functionName) {}
	
	RarException::RarException(RarException const& src) noexcept : Exception(src) {}
	
	RarException::~RarException() {}
	
	
	RarException& RarException::operator=(RarException const& src) noexcept
	{
		Exception::operator=(src);
		
		return *this;
	}
	
	
	char const* RarException::what() const noexcept
	{
		return Exception::what();
	}
}