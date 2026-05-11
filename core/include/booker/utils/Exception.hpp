#ifndef DEF_EXCEPTION
#define DEF_EXCEPTION

#include <string>
#include <exception>

#if defined(__GNUC__) || defined(__clang__)
	#define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define FUNCTION_NAME __FUNCSIG__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
	#define FUNCTION_NAME __func__
#else
	#define FUNCTION_NAME "<unknown>"
#endif

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

#define MAKE_EXCEPTION(ExType, msg) ExType((msg), __FILE__, __LINE__, FUNCTION_NAME)

#endif // DEF_EXCEPTION