#ifndef DEF_ZIPEXCEPTION
#define DEF_ZIPEXCEPTION

#include "../utils/Exception.hpp"

class ZipException : public Exception
{
	public:
		ZipException(char const* what_arg, char const* file, int line, char const* functionName) noexcept;
		ZipException(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept;
		ZipException(ZipException const& src) noexcept;
		virtual ~ZipException() override;
		
		ZipException& operator=(ZipException const& src) noexcept;
		
		virtual char const* what() const noexcept override;
};

#endif // DEF_ZIPEXCEPTION