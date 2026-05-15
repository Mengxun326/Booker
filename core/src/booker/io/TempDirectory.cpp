#include "../../../include/booker/io/TempDirectory.hpp"

#include <chrono>
#include <atomic>

namespace booker
{
	TempDirectory::TempDirectory() : m_path(std::filesystem::temp_directory_path() / generateName())
	{
		std::filesystem::create_directories(m_path);
	}
	
	TempDirectory::~TempDirectory()
	{
		if(std::filesystem::exists(m_path))
			std::filesystem::remove_all(m_path);
	}
	
	
	std::string TempDirectory::generateName()
	{
		static std::atomic<uint64_t> counter{ 0 };
		auto now = std::chrono::steady_clock::now().time_since_epoch().count();
		
		return "booker_" + std::to_string(now) + "_" + std::to_string(counter.fetch_add(1));
	}
}