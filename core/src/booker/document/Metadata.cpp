#include "../../../include/booker/document/Metadata.hpp"

namespace booker
{
	void Metadata::write(std::ostream& os) const
	{
		for(std::map<std::string, std::string>::const_iterator it=m_data.begin();it!=m_data.end();++it)
			os << it->first << ":" << it->second << '\n';
	}
}