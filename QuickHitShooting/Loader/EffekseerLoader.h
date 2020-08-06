#pragma once
#include <map>
#include "Loader.h"

class EffekseerLoader :
	public Loader
{
private:
	std::map<std::string, int> _table;
public:
	EffekseerLoader();
	~EffekseerLoader();

	bool Load(const std::string& path, Data& data)override final;

	void UnLoad(const std::string& path)override final;
};

class EffekseerData : public Data
{
	friend EffekseerLoader;
private:
	int _handle;

	bool IsAvailable();

public:
	int GetHandle();
};