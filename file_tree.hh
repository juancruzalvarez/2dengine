#ifndef FILE_TREE_H
#define FILE_TREE_H

#include <string>

#include <glm.hpp>

#include "file_utils.hh"
#include "renderer.hh"

class FileTree {

public:

FileTree();
// Sets the root folder 
void SetRootFolder(const std::string& path);
// Open folder, or open file.
// If open file, return the path to the file in @path, else @path = "".
void Enter(std::string& path);
// Move the current selected folder, direction
void Move(const int& direction);
// Render the FileTree at the position especified by @pos
void Render(graphics::Renderer* renderer);

void SetPosition(const glm::vec2& position);

void SetSize(const glm::vec2& size);

glm::vec2 Position();
glm::vec2 Size();

void SetTextSize(const int& size);

private:

struct Entry {
	std::string name;
	std::string path;
	bool is_directory;
	bool open;
	int indentation;
};

std::vector<Entry> m_entries;
std::string m_root_path;
glm::vec2 m_position;
glm::vec2 m_size;
int m_first_line;
int m_entries_size;
int m_text_size;
int m_pos;

int  get_absolute_entry_pos(const int& pos);

void generate_entries(
	const file_utils::File& file,
	const std::string& current_path,
	const int& current_indentation
);

void calculate_entries_size();


};

#endif