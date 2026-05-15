#include "../../../include/booker/utils/ImageUtils.hpp"

#include <map>
#include <cctype>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <webp/decode.h>
#include <webp/encode.h>

#include "../../../include/booker/utils/Exception.hpp"

namespace booker
{
	static std::map<std::string, std::string> const IMAGE_MIME_MAP = {
		// JPEG
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".jpe", "image/jpeg"},
		{".jfif", "image/jpeg"},
		// PNG
		{".png", "image/png"},
		// GIF
		{".gif", "image/gif"},
		// BMP
		{".bmp", "image/bmp"},
		{".dib", "image/bmp"},
		// WebP
		{".webp", "image/webp"},
		// TIFF
		{".tiff", "image/tiff"},
		{".tif", "image/tiff"},
		// SVG
		{".svg", "image/svg+xml"},
		{".svgz", "image/svg+xml"},
		// Other rare image formats.
		{".avif", "image/avif"},
		{".heic", "image/heic"},
		{".heif", "image/heif"},
		{".jxr", "image/jxr"},
		{".jp2", "image/jp2"}
	};
	
	bool isImageExtension(std::filesystem::path const& filePath)
	{
		std::string extension = filePath.extension().string();
		
		if(extension.empty())
			return false;
		
		for(char& c : extension)
			c = static_cast<char>(std::tolower(static_cast<int>(c)));
		
		return IMAGE_MIME_MAP.find(extension) != IMAGE_MIME_MAP.end();
	}
	
	std::string mimeFromExtension(std::filesystem::path const& filePath)
	{
		std::string extension = filePath.extension().string();
		
		if(extension.empty())
			return "application/octet-stream";
		
		for(char& c : extension)
			c = static_cast<char>(std::tolower(static_cast<int>(c)));
		
		std::map<std::string, std::string>::const_iterator it = IMAGE_MIME_MAP.find(extension);
		
		if(it != IMAGE_MIME_MAP.end())
			return it->second;
		else
			return "application/octet-stream";
	}
	
	std::vector<uint8_t> convertToJPEG(std::vector<uint8_t> const& inputData, std::string const& inputMimeType, int quality)
	{
		if(inputMimeType == "image/jpeg")
			return inputData;
		
		int width = 0;
		int height = 0;
		int channels = 3;
		//unsigned char* decoded = stbi_load_from_memory(inputData.data(), static_cast<int>(inputData.size()), &width, &height, &channels, 3);
		unsigned char* decoded = nullptr;
		
		if(inputMimeType == "image/webp")
		{
			if(inputData.empty())
				throw MAKE_EXCEPTION(Exception, "ERROR : Empty WebP image data.");
			
			decoded = WebPDecodeRGB(inputData.data(), inputData.size(), &width, &height);
			
			if(!decoded)
			{
				int w;
				int h;
				uint8_t* rgba = WebPDecodeRGBA(inputData.data(), inputData.size(), &w, &h);
				
				if(!rgba)
					throw MAKE_EXCEPTION(Exception, "ERROR : Failed to decode WebP image.");
				
				width = w;
				height = h;
				channels = 3;
				decoded = static_cast<unsigned char*>(malloc(width * height * 3));
				
				for(int i=0;i<(width*height);++i)
				{
					decoded[(i * 3 + 0)] = rgba[(i * 4 + 0)];
					decoded[(i * 3 + 1)] = rgba[(i * 4 + 1)];
					decoded[(i * 3 + 2)] = rgba[(i * 4 + 2)];
				}
				
				WebPFree(rgba);
			}
			
			channels = 3;
		}
		else if(inputMimeType == "image/png" || inputMimeType == "image/bmp" || inputMimeType == "image/gif" || inputMimeType == "image/tiff")
		{
			decoded = stbi_load_from_memory(inputData.data(), static_cast<int>(inputData.size()), &width, &height, &channels, 3);
			
			if(!decoded)
				throw MAKE_EXCEPTION(Exception, "ERROR : Failed to decode " + inputMimeType + " image: " + std::string(stbi_failure_reason()));
		}
		else
			throw MAKE_EXCEPTION(Exception, "ERROR : Unsupported image format: " + inputMimeType);
		
		if(width <= 0 || height <= 0)
		{
			if(inputMimeType == "image/webp")
				WebPFree(decoded);
			else
				stbi_image_free(decoded);
			
			throw MAKE_EXCEPTION(Exception, "ERROR : Invalid image dimensions: " + std::to_string(width) + "x" + std::to_string(height));
		}
		
		std::vector<uint8_t> result;
		
		auto writeCallback = [](void* context, void* data, int size)
		{
			std::vector<uint8_t>* vec = static_cast<std::vector<uint8_t>*>(context);
			unsigned char const* bytes = static_cast<unsigned char const*>(data);
			
			vec->insert(vec->end(), bytes, bytes + size);
		};
		
		int success = stbi_write_jpg_to_func(writeCallback, &result, width, height, channels, decoded, quality);
		
		if(inputMimeType == "image/webp")
			WebPFree(decoded);
		else
			stbi_image_free(decoded);
		
		if(success == 0 || result.empty())
			throw MAKE_EXCEPTION(Exception, "Failed to encode image to JPEG");
		
		return result;
	}
}