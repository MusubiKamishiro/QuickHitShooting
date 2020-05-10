#pragma once
#include <memory>
#include <map>
#include <string>

class Loader;
class Data;

class FileSystem
{
private:
	std::unique_ptr<Loader> imageLoader;
	std::unique_ptr<Loader> soundLoader;

	std::map<std::string, std::shared_ptr<Loader>> loaders;

	///�g���q�̒��o
	///����𗘗p����,�ǂ�Load���g�p���邩�����肷��
	std::string GetExtension(const std::string& path);

public:
	FileSystem();
	~FileSystem();

	bool Load(const std::string& filePath, Data& data);
};

