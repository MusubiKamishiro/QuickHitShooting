#include "ImageLoader.h"
#include <DxLib.h>


ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

bool ImageLoader::Load(const std::string& path, Data& data)
{
	ImageData& img = dynamic_cast<ImageData&>(data);

	// データが見つからなかったら読み込む
	auto it = table.find(path.c_str());
	if (it == table.end())
	{
		img.handle = DxLib::LoadGraph(path.c_str());
		if (img.handle != -1)
		{
			table.emplace(path, img.handle);
			return true;
		}
		return false;
	}
	else
	{
		// 見つかったらハンドルを返す
		img.handle = table[path.c_str()];
		return true;
	}
	return false;
}

void ImageLoader::UnLoad(const std::string& path)
{
}


bool ImageData::IsAvailable()
{
	return true;
}

int ImageData::GetHandle() const
{
	return handle;
}
