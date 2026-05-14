#include "../../../include/booker/document/Metadata.hpp"

std::ostream& operator<<(std::ostream& os, Metadata const& src)
{
	src.write(os);
	
	return os;
}


void Metadata::write(std::ostream& os) const
{
	for(std::map<std::string, std::string>::const_iterator it=m_data.begin();it!=m_data.end();++it)
		os << it->first << ":" << it->second << '\n';
}