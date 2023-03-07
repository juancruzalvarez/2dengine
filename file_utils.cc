#include "file_utils.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
namespace file_utils {

FileUtilErr Exists(const std::string& path, bool& exists) {
	try {
		exists = std::filesystem::exists(path);
	}
	catch (std::filesystem::filesystem_error err) {
		return FileUtilErr::OS_API_ERR;
	}
}

FileUtilErr GetContent(const std::string& path, std::vector<std::string>& content) {
	
	std::string tmp;
	std::ifstream file_stream;
	
	file_stream.open(path);
	if (!file_stream.is_open())
		return FileUtilErr::UNABLE_TO_OPEN_STREAM_ERR;

	while (file_stream) {
		std::getline(file_stream, tmp);
		content.push_back(tmp);
	}

	file_stream.close();

	return FileUtilErr::NO_ERR;
	
}

FileUtilErr SaveFile(const std::string& path, const std::vector<std::string>& content) {
	std::ofstream file_stream(path);

	if (!file_stream.is_open())
		return FileUtilErr::UNABLE_TO_OPEN_STREAM_ERR;

	for (const auto& line : content)
		file_stream << line << "\n";

	file_stream.close();
	return FileUtilErr::NO_ERR;
}

File GetFileTree(const std::string& path) {
	File file;
	file.name = std::filesystem::path(path).filename().string();
	file.is_directory = std::filesystem::is_directory(path);
	int cc = 0;
	if (file.is_directory) {
		for (const auto& child : std::filesystem::directory_iterator(path)) {
			cc++;
			auto child_name = std::filesystem::path(child.path().string()).filename().string();
			if (child_name[0] == '.') continue;
			file.children.push_back(GetFileTree(child.path().string()));
		}
	}
	return file;
}

void ChangeTabsToSpaces(std::vector<std::string>& content, int spaces_in_tab) {
	for (auto& line : content) {
		int i = 0;
		int tab_pos = std::string::npos;
		while ((tab_pos = line.find('\t', i)) != std::string::npos) {
			line.erase(line.begin()+tab_pos);
			line.insert(line.begin()+tab_pos, spaces_in_tab, ' ');
		}

	}
}




}