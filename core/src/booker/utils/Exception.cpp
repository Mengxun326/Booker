#include "../../../include/booker/utils/Exception.hpp"

Exception::Exception(char const* what_arg, char const* file, int line, char const* functionName) noexcept : std::exception(), m_msg(what_arg), m_file(file), m_line(line), m_function(functionName)
{
	formatWhatMsg();
}

Exception::Exception(std::string const& what_arg, std::string const& file, int line, std::string const& functionName) noexcept : std::exception(), m_msg(what_arg), m_file(file), m_line(line), m_function(functionName)
{
	formatWhatMsg();
}

Exception::Exception(Exception const& src) noexcept : std::exception(src), m_msg(src.m_msg), m_file(src.m_file), m_line(src.m_line), m_function(src.m_function)
{
	formatWhatMsg();
}

Exception::~Exception() {}


Exception& Exception::operator=(Exception const& src) noexcept
{
	if(this != &src)
	{
		std::exception::operator=(src);
		
		m_msg = src.m_msg;
		m_file = src.m_file;
		m_line = src.m_line;
		m_function = src.m_function;
		
		formatWhatMsg();
	}
	
	return *this;
}


char const* Exception::what() const noexcept
{
	return m_whatMsg.c_str();
}


void Exception::formatWhatMsg()
{
	m_whatMsg = m_file + " : " + std::to_string(m_line) + " : " + m_function + " : " + m_msg;
}