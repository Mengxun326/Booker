#include "../../../include/booker/utils/StringUtils.hpp"

#include <iomanip>
#include <sstream>

namespace booker
{
	std::string fillWithLeadingZeros(int nb, int maximum)
	{
		size_t width = std::to_string(maximum).size();
		
		std::ostringstream oss;
		
		oss << std::setw(static_cast<int>(width)) << std::setfill('0') << nb;
		
		return oss.str();
	}
}