#ifndef DIRECTORYUTILS_HPP
#define DIRECTORYUTILS_HPP

/*!
\file DirectoryUtils.hpp
\brief Define utils functions related to directories.
*/

#include <filesystem>

namespace booker
{
	/*!
	\fn bool flattenDirectory(std::filesystem::path const& dirPath)
	\brief A function which make a directory flat.
	*/
	bool flattenDirectory(std::filesystem::path const& dirPath);
}


#endif // DIRECTORYUTILS_HPP