#ifndef DEF_ZIPEXCEPTION
#define DEF_ZIPEXCEPTION

/*!
\file ZipException.hpp
\brief Define ZipException.
\details This file define ZipException, a class herited from \ref Exception class.
*/

#include "../utils/Exception.hpp"

namespace booker
{
	/*!
	\class ZipException ZipException.hpp "core/include/booker/archive/ZipException.hpp"
	\brief An Exception derived class, which indicates file, line and function name of the thrown exception.
	*/
	
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
}

#endif // DEF_ZIPEXCEPTION