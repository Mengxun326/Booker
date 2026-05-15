#ifndef DEF_EXCEPTION
#define DEF_EXCEPTION

/*!
\file Exception.hpp
\brief Define an Exception class.
\details This file define an Exception class, which show (in what() method) the error message, and the file, line and function name where the exception were thrown. It defines also two useful macros.
*/

#include <string>
#include <exception>

/*!
\def FUNCTION_NAME
A cross-platform and cross-compiler macro which give name of the current function.
*/
#if defined(__GNUC__) || defined(__clang__)
	#define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define FUNCTION_NAME __FUNCSIG__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
	#define FUNCTION_NAME __func__
#else
	#define FUNCTION_NAME "<unknown>"
#endif

namespace booker
{
	/*!
	\class Exception Exception.hpp "core/include/booker/utils/Exception.hpp"
	\brief An Exception class, which indicates file, line and function name of the thrown exception.
	*/
	class Exception : public std::exception
	{
		public:
			Exception(char const* what_arg, char const* file, int line, char const* functionName) noexcept;
			Exception(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept;
			Exception(Exception const& src) noexcept;
			virtual ~Exception() override;
			
			Exception& operator=(Exception const& src) noexcept;
			
			virtual char const* what() const noexcept override;
		
		private:
			std::string m_msg;
			std::string m_file;
			int m_line;
			std::string m_function;
			std::string m_whatMsg;
			
			void formatWhatMsg();
	};
}

/*!
\def MAKE_EXCEPTION(ExType, msg)
\brief A macro which create an Exception instance, with Exception type and the message.
\details It fills automatically the file, line and function name of the function where it's thrown.
\param ExType (class) : The exception class (Exception, or an herited class).
\param msg (string) : The exception message.
\return An exception instance, with file, line and function name already filled (Exception or an herited class).
*/
#define MAKE_EXCEPTION(ExType, msg) ExType((msg), __FILE__, __LINE__, FUNCTION_NAME)

#endif // DEF_EXCEPTION