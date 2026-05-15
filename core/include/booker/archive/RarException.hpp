#ifndef DEF_RAREXCEPTION
#define DEF_RAREXCEPTION

/*!
\file RarException.hpp
\brief Define RarException.
\details This file define RarException, a class herited from \ref Exception class.
*/

#include "../utils/Exception.hpp"

namespace booker
{
	/*!
	\class RarException RarException.hpp "core/include/booker/archive/RarException.hpp"
	\brief An Exception derived class, which indicates file, line and function name of the thrown exception.
	*/
	
	class RarException : public Exception
	{
		public:
			RarException(char const* what_arg, char const* file, int line, char const* functionName) noexcept;
			RarException(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept;
			RarException(RarException const& src) noexcept;
			virtual ~RarException() override;
			
			RarException& operator=(RarException const& src) noexcept;
			
			virtual char const* what() const noexcept override;
	};
}

#endif // DEF_RAREXCEPTION