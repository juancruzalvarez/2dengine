#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

namespace file_utils {

enum class FileUtilErr {
	NO_ERR,
	OS_API_ERR,
	UNABLE_TO_OPEN_STREAM_ERR,
};

// Structure to hold a file tree
struct File {
	std::string name;
	bool is_directory;
	std::vector<File> children;
};

// Returns true or false depending of if the file at path exists.
FileUtilErr Exists(const std::string& path, bool& exists);

// Returns the content of the file at path. 
// The content is returned as a vector of strings, where each string is a line on the original file.
FileUtilErr GetContent(const std::string& path, std::vector<std::string>& content);

// Saves content to a file at path.
FileUtilErr SaveFile(const std::string& path, const std::vector<std::string>& content);

// Returns the file at the given path, if the file is a directory it will recursibly call itself to find all children.
File GetFileTree(const std::string& path);

void ChangeTabsToSpaces(std::vector<std::string>& content, int spaces_in_tab);


}

#endif