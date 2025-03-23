#include "binary_loader.h"
#include <filesystem>
#include <fstream>

void binary_loader::load(const std::filesystem::path &path)
{
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		data_.resize(static_cast<size_t>(size));
		
		file.read(&data_[0], size);
		
	}
}