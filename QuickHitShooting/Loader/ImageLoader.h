#pragma once
#include "Loader.h"
#include <map>


class ImageLoader : public Loader
{
private:
	std::map<std::string, int> table;

public:
	ImageLoader();
	~ImageLoader();

	int Load(const std::string& path)override final;

	void UnLoad(const std::string& path)override final;
};
