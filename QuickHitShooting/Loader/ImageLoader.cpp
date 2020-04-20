#include "ImageLoader.h"
#include <DxLib.h>


ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

int ImageLoader::Load(const std::string& path)
{
	int img = 0;

	// データが見つからなかったら読み込む
	auto it = table.find(path.c_str());
	if (it == table.end())
	{
		img = DxLib::LoadGraph(path.c_str());
		if (img != -1)
		{
			table.emplace(path, img);
		}
		return img;
	}
	else
	{
		// 見つかったらハンドルを返す
		return table[path.c_str()];
	}
	return false;
}

void ImageLoader::UnLoad(const std::string& path)
{
}
