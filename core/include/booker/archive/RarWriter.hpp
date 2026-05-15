#ifndef DEF_RARWRITER
#define DEF_RARWRITER

/*!
\file RarWriter.hpp
\brief Define RarWriter.
\details This file define RarWriter, a basic class to manage RAR writing.
*/

#include <RarLib/Rar.hpp>

namespace booker
{
	/*!
	\class RarWriter RarWriter.hpp "core/include/booker/archive/RarWriter.hpp"
	\brief A class allowing to compress RAR.
	\details This class, by heriting from Rar (from RarLib), exposes these methods :
	- bool isRarInstalled() const;
	- bool compressOneFile(std::string const& filePath, std::string const& archiveName, bool checkUnsafeChars = true) const;
	- bool compressMultipleFiles(std::vector<std::string> const& files, std::string const& archiveName) const;
	- bool compressDirectory(std::string const& directoryPath, std::string const& archiveName, bool checkUnsafeChars = true) const;
	*/
	
	class RarWriter : public Rar
	{
		public:
			RarWriter();
			RarWriter(RarWriter const& src);
			virtual ~RarWriter() = default;
			
			RarWriter& operator=(RarWriter const& src);
	};
}

#endif // DEF_RARWRITER