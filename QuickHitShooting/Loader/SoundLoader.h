#pragma once
#include "Loader.h"
#include <map>


class SoundLoader : public Loader
{
private:
	std::map<std::string, int> table;

public:
	SoundLoader();
	~SoundLoader();

	int Load(const std::string& path)override final;

	void UnLoad(const std::string& path)override final;
};
