#include "file_tree.hh"
#include <iostream>

FileTree::FileTree() {

}

// Sets the root folder 
void FileTree::SetRootFolder(const std::string& path) {
	m_root_path = path;
	file_utils::File f;
	std::cout << "hey";
	file_utils::GetFileTree(path, f);
	std::cout << "hey";
	generate_entries(f, path, 0);
	std::cout << "Printing entries:\n";
	for (const auto& entry : m_entries) {
		for (int i = 0; i < entry.indentation; i++)
			std::cout << " ";
		std::cout << entry.name << "\n";
	}
	calculate_entries_size();
	std::cout << m_entries_size;

}

// Open folder, or open file.
// If open file, return the path to the file in @path, else @path = "".
void FileTree::Enter(std::string& path) {
	if (m_entries[m_pos].is_directory) {
		m_entries[m_pos].open = !m_entries[m_pos].open;
		calculate_entries_size();
		path = "";
	}
	else {
		path = m_entries[m_pos].path;
	}
}

// Move the current selected folder, direction
void FileTree::Move(const int& direction) {
	m_pos = std::min(m_entries_size, std::max(m_pos + direction, 0));
}

// Render the FileTree at the position especified by @pos
void FileTree::Render(graphics::Renderer* renderer, const glm::vec2& pos) {
	int base_line_x = pos.x + 4;
	int indent_size = 2*renderer->GetTextSize(" ", m_text_size).x;
	int y = pos.y + m_size.y-m_text_size;
	int i = 0;
	while (i < m_entries.size()) {
		auto current = m_entries[i];
		renderer->RenderText(current.name, {pos.x + base_line_x+current.indentation*indent_size, y },m_text_size, {1,1,1,1});
		y -= m_text_size;
		i++;
		if (current.is_directory && !current.open) {
			while (i<m_entries.size() && m_entries[i].indentation > current.indentation)
				i++;
		}
	}
}

void FileTree::SetSize(const glm::vec2& size) {
	m_size = size;
}

void FileTree::SetTextSize(const int& size) {
	m_text_size = size;
}


void FileTree::generate_entries(
	const file_utils::File& file,
	const std::string& current_path,
	const int& current_indentation) {

	m_entries.push_back({
		file.name,
		current_path + file.name,
		file.is_directory,
		false,
		current_indentation,
		});
	auto next_path = current_path + file.name + '\\';
	if (file.is_directory) {
		for (const auto& child : file.children) {
			generate_entries(child, next_path, current_indentation + 1);
		}
	}
}

void FileTree::calculate_entries_size() {
	int i = 0, n = 0;
	while (i < m_entries.size()) {
		auto current = m_entries[i];
		i++;
		n++;
		if (current.is_directory && !current.open) {
			while (i<m_entries.size() && m_entries[i].indentation > current.indentation)
				i++;
		}
	}
	std::cout << "n:" << n << "\n";
	m_entries_size = n;
}

int FileTree::get_absolute_entry_pos(const int& pos) {
	int absolute_pos = 0;
	for (int i = 0; i < pos; i++) {
		auto current = m_entries[absolute_pos];
		if (current.is_directory && !current.open) {
			while (absolute_pos<m_entries.size() && m_entries[absolute_pos].indentation > current.indentation)
				absolute_pos++;
		}
	}
	return absolute_pos;
}




