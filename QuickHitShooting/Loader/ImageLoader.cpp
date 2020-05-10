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
	auto it = _table.find(path.c_str());
	if (it == _table.end())
	{
		img._handle = DxLib::LoadGraph(path.c_str());
		if (img._handle != -1)
		{
			_table.emplace(path, img._handle);
			return true;
		}
		return false;
	}
	else
	{
		// 見つかったらハンドルを返す
		img._handle = _table[path.c_str()];
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
	return _handle;
}
