#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Vector2.h"

#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))
#define SET_IMAGE_ID(KEY,  ...) (ImageMng::GetInstance().GetID(KEY, __VA_ARGS__))

using VEC_INT = std::vector<int>;

class ImageMng
{
public:
	static ImageMng & GetInstance()
	{
		return *s_Instance;
	}

	// ·°‚Ìî•ñ‚©‚çID‚Ìî•ñ‚ğæ“¾‚·‚é
	const VEC_INT& GetID(const std::string& key);

	/// ‰æ‘œ‚Ì·°‚ÆÊß½‚ğİ’è‚·‚éˆ—
	const VEC_INT& GetID(const std::string& key, const std::string& path);
	const VEC_INT& GetID(const std::string& key, const std::string& path,
						 const Vector2& divCnt, const Vector2& divSize);
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

	std::map<std::string, VEC_INT> _imgMap;
};

