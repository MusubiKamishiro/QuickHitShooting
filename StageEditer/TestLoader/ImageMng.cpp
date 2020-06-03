#include <DxLib.h>
#include "ImageMng.h"

std::unique_ptr<ImageMng, ImageMng::ImageDeleter> ImageMng::s_Instance(new ImageMng());

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}

const int & ImageMng::GetID(const std::string& path)
{
	if (_imgMap.find(path) == _imgMap.end())
	{
		_imgMap[path] = LoadGraph(path.c_str());
	}
	return _imgMap[path];
}