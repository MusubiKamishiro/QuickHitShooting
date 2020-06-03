#pragma once

#include <memory>
#include <map>
#include <string>

class ImageMng
{
public:
	static ImageMng & Instance()
	{
		return *s_Instance;
	}

	// ·°‚Ìî•ñ‚©‚çID‚Ìî•ñ‚ğæ“¾‚·‚é
	const int& GetID(const std::string& key);
private:
	ImageMng();
	~ImageMng();
	struct ImageDeleter
	{
		void operator()(ImageMng * imageMng) const
		{
			delete imageMng;
		}
	};
	static std::unique_ptr<ImageMng, ImageDeleter> s_Instance;

	std::map<std::string, int> _imgMap;
};

