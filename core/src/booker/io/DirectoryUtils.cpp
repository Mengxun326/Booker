#include "../../../include/booker/io/DirectoryUtils.hpp"

#include <string>

bool flattenDirectory(std::filesystem::path const& dirPath)
{
	if(!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
		return false;
	
	bool success = true;
	
	for(std::filesystem::recursive_directory_iterator it(dirPath, std::filesystem::directory_options::skip_permission_denied);it!=std::filesystem::recursive_directory_iterator();++it)
	{
		std::filesystem::directory_entry const& entry = *it;
		
		if(!entry.is_regular_file() || entry.path().parent_path() == dirPath)
			continue;
		
		std::filesystem::path src = entry.path();
		std::filesystem::path target = dirPath / src.filename();
		
		if(std::filesystem::exists(target))
		{
			std::string stem = target.stem().string();
			std::string ext = target.extension().string();
			int counter = 1;
			
			do
			{
				target = dirPath / (stem + "_" + std::to_string(counter) + ext);
				
				++counter;
			}
			while(std::filesystem::exists(target));
		}
		
		try
		{
			std::filesystem::rename(src, target);
		}
		catch(std::filesystem::filesystem_error const& err)
		{
			success = false;
			
			break;
		}
	}
	
	if(success)
	{
		for(std::filesystem::directory_entry const& entry : std::filesystem::directory_iterator(dirPath))
		{
			if(entry.is_directory())
			{
				try
				{
					std::filesystem::remove_all(entry.path());
				}
				catch(std::filesystem::filesystem_error const& err)
				{
					success = false;
					
					break;
				}
			}
		}
	}
	
	return success;
}