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

	bool Load(const std::string& path, Data& data)override final;

	void UnLoad(const std::string& path)override final;
};

class SoundData : public Data
{
	friend SoundLoader;
private:
	int handle;
	bool IsAvailable();

public:
	int GetHandle()const;
};