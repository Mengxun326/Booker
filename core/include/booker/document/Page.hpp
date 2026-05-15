#ifndef DEF_PAGE
#define DEF_PAGE

#include <filesystem>
#include <string>
#include <vector>
#include <memory>

namespace booker
{
	class IFormatHandler;
	
	class Page
	{
		public:
			Page(size_t index, std::string mimeType, double width, double height, std::shared_ptr<IFormatHandler> handler, std::filesystem::path sourceFile);
			
			inline size_t index() const { return m_index; }
			inline std::string const& mimeType() const { return m_mime; }
			inline double width() const { return m_width; }
			inline double height() const { return m_height; }
			
			std::vector<uint8_t>& data() const;
			bool isLoaded() const { return m_isLoaded; }
			void clearCache() const;
		
		private:
			size_t m_index;
			std::string m_mime;
			double m_width;
			double m_height;
			std::shared_ptr<IFormatHandler> m_handler;
			std::filesystem::path m_sourceFile;
			mutable std::vector<uint8_t> m_cachedData;
			mutable bool m_isLoaded = false;
	};
}

#endif // DEF_PAGE