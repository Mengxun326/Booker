#ifndef DEF_TEMPDIRECTORY
#define DEF_TEMPDIRECTORY

#include <filesystem>
#include <string>

class TempDirectory
{
	public:
		TempDirectory();
		TempDirectory(TempDirectory const& src) = delete;
		~TempDirectory();
		
		TempDirectory& operator=(TempDirectory const& src) = delete;
		
		inline std::filesystem::path const& getPath() const { return m_path; }
	
	private:
		std::filesystem::path m_path;
		
		static std::string generateName();
};

#endif // DEF_TEMPDIRECTORY