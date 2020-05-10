#include "FileSystem.h"
#include "Loader.h"
#include "ImageLoader.h"
#include "SoundLoader.h"


FileSystem::FileSystem()
{
	imageLoader = std::unique_ptr<ImageLoader>();
	soundLoader = std::unique_ptr<SoundLoader>();

	loaders["bmp"] = loaders["png"] = loaders["jpg"] = std::make_shared<ImageLoader>();
	loaders["mp3"] = loaders["wav"] = std::make_shared<SoundLoader>();
}


FileSystem::~FileSystem()
{
}

std::string FileSystem::GetExtension(const std::string& path)
{
	auto idx = path.rfind('.') + 1;
	return path.substr(idx, path.length() - idx);
}

bool FileSystem::Load(const std::string& filePath, Data& data)
{
	auto ext = GetExtension(filePath.c_str());
	return loaders[ext]->Load(filePath.c_str(), data);
}
