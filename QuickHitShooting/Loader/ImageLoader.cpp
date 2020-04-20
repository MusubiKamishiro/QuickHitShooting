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

	// �f�[�^��������Ȃ�������ǂݍ���
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
		// ����������n���h����Ԃ�
		return table[path.c_str()];
	}
	return false;
}

void ImageLoader::UnLoad(const std::string& path)
{
}
