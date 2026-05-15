#ifndef DEF_TEMPDIRECTORY
#define DEF_TEMPDIRECTORY

#include <filesystem>
#include <string>

namespace booker
{
	/*!
	\class TempDirectory TempDirectory.hpp "core/include/booker/io/TempDirectory.hpp"
	\brief A basic RAII class, to create and delete a temporary directory.
	*/
	
	class TempDirectory
	{
		public:
			TempDirectory();
			TempDirectory(TempDirectory const& src) = delete;
			~TempDirectory();
			
			TempDirectory& operator=(TempDirectory const& src) = delete;
			
			inline std::filesystem::path const& getPath() const { return m_path; }
		
		private:
			std::filesystem::path m_path; /*!< The path of temporary directory. */
			
			static std::string generateName();
	};
}

#endif // DEF_TEMPDIRECTORY