#include <DxLib.h>
#include "ImageMng.h"

std::unique_ptr<ImageMng, ImageMng::ImageDeleter> ImageMng::s_Instance(new ImageMng());

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}

const VEC_INT & ImageMng::GetID(const std::string& key)
{
	return GetID(key, key);
}

const VEC_INT & ImageMng::GetID(const std::string & key, const std::string & path)
{
	if (_imgMap.find(key) == _imgMap.end())
	{
		_imgMap[key].resize(1);
		_imgMap[key][0] = LoadGraph(path.c_str());
	}
	return _imgMap[key];
}

const VEC_INT & ImageMng::GetID(const std::string & key, const std::string & path, const Vector2 & divCnt, const Vector2 &divSize)
{
	if (_imgMap.find(key) == _imgMap.end())
	{
		_imgMap[key].resize(divCnt.x * divCnt.y);
		LoadDivGraph(path.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, 
								   divSize.x, divSize.y, &_imgMap[key][0]);
	}
	return _imgMap[key];
}
