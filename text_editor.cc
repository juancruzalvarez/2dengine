#include "text_editor.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

TextEditor::TextEditor() {
	m_lines = {""};
	m_cursor_pos = glm::vec2{ 0,0 };
}

TextEditor::TextEditor(const std::string& title, int line_height): m_title(title), m_line_height(line_height) {
	m_lines = { "" };
	m_cursor_pos = glm::vec2{ 0,0 };
};


void TextEditor::Write(uint8_t c) {
	if (c == '\n') {
		NewLine();
		return;
	}
	m_lines[m_cursor_pos.y].insert(m_cursor_pos.x, 1, c);
	m_cursor_pos.x++;
	
}

void TextEditor::NewLine() {
	auto current_line = &m_lines[m_cursor_pos.y];
	auto first = current_line->substr(0, m_cursor_pos.x);
	auto rest = current_line->substr(m_cursor_pos.x, current_line->size()-m_cursor_pos.x);
	std::cout <<"f"<<first<< "Rest:" << rest << "\n";
	*current_line = first;
	m_lines.insert(m_lines.begin() + m_cursor_pos.y+1, rest);
	MoveCursor(Direction::DOWN);
	m_cursor_pos.x = 0;

}

void TextEditor::Erase() {
	if (m_cursor_pos.x == 0 ) {
		if (m_cursor_pos.y == 0) return;

		auto line_before_current = &m_lines[m_cursor_pos.y - 1];
		auto line_before_size = m_lines[m_cursor_pos.y - 1].size();
		line_before_current->insert(line_before_size, m_lines[m_cursor_pos.y]);
		m_lines.erase(m_lines.begin() + m_cursor_pos.y);
		m_cursor_pos.x = line_before_current->size()+1;
		MoveCursor(Direction::UP);
	}else {
		m_lines[m_cursor_pos.y].erase(m_lines[m_cursor_pos.y].begin()+m_cursor_pos.x-1);
		m_cursor_pos.x--;
	}
}

void TextEditor::Supr() {
	auto current_line = &m_lines[m_cursor_pos.y];
	auto next_line = m_cursor_pos.y + 1 < m_lines.size()  ?  &m_lines[m_cursor_pos.y + 1] : nullptr;
	if (m_cursor_pos.x == current_line->size() - 1 && next_line) {
		current_line->insert(current_line->size(), *next_line);
		m_lines.erase(m_lines.begin() + m_cursor_pos.y + 1);
	}else {
		current_line->erase(current_line->begin() + m_cursor_pos.x);
	}
}

void TextEditor::Tab() {
	m_lines[m_cursor_pos.y].insert(m_cursor_pos.x, 3, ' ');
	m_cursor_pos.x += 3;
}

void TextEditor::SetContent(std::vector<std::string> content) {
	m_lines = content;
	m_cursor_pos = { 0,0 };
	m_first_line = 0;
}


void TextEditor::MoveCursor(Direction dir) {
	switch (dir) {
	case Direction::UP: {
		if (m_cursor_pos.y > 0) {
			if (m_cursor_pos.y == m_first_line)
				m_first_line--;
			
			m_cursor_pos.y--;
		}
		m_cursor_pos.x = std::max(0, std::min(m_cursor_pos.x, (int)m_lines[m_cursor_pos.y].size()));
		break; 
	}
	case Direction::DOWN: {
		if (m_cursor_pos.y < m_lines.size() - 1) {
			int lines_size = m_size.y - m_box_size - m_line_height * 0.5;
			int lines_to_render = lines_size / m_line_height;
			int last_line_rendered = m_first_line + lines_to_render - 1;
			if (m_cursor_pos.y == last_line_rendered)
				m_first_line++;

			m_cursor_pos.y++;
		}
		m_cursor_pos.x = std::max(0, std::min(m_cursor_pos.x, (int)m_lines[m_cursor_pos.y].size()));

		break;
	}
	case Direction::RIGHT: {
		if (m_cursor_pos.x >= m_lines[m_cursor_pos.y].size()) {
			if (m_cursor_pos.y < m_lines.size() - 1) {
				m_cursor_pos.x = 0;
				MoveCursor(Direction::DOWN);
			}
		}else
			m_cursor_pos.x++;
		
		break;
	}
	case Direction::LEFT: {
		if (m_cursor_pos.x == 0  ) {
			if (m_cursor_pos.y > 0) {
				MoveCursor(Direction::UP);
				m_cursor_pos.x = m_lines[m_cursor_pos.y].size();
			}
		}
		else
			m_cursor_pos.x--;

		break;
	}

	}
}

void TextEditor::SetLineHeight(int line_height) {
	m_line_height = line_height;
}

void TextEditor::SetBoxSize(int box_size) {
	m_box_size = box_size;
}

void TextEditor::SetTitle(std::string title) {
	m_title = title;
}

void TextEditor::SetPosition(glm::vec2 position) {
	m_position = position;
}

void TextEditor::SetSize(glm::vec2 size) {
	m_size = size;
}


glm::vec2 TextEditor::Position() {
	return m_position;
}
glm::vec2 TextEditor::Size() {
	return m_size;
}


void TextEditor::Render(graphics::Renderer* renderer) {
	
	auto box_color = m_active ? glm::vec4(1,1,1,1) : glm::vec4(0.5,0.5,0.5,1);
	//Render box:
	auto title_width = renderer->GetTextSize(m_title, m_line_height*0.9).x;
	auto title_sidebar_width = (m_size.x - title_width - 4 * m_box_size) /2.f;
	renderer->RenderWhiteQuad({m_position, {m_box_size, m_size.y },{}, box_color });
	renderer->RenderWhiteQuad({ m_position +glm::vec2{m_size.x-m_box_size,0.f}, {m_box_size, m_size.y},{}, box_color });
	renderer->RenderWhiteQuad({ m_position, {m_size.x, m_box_size},{}, box_color });
	renderer->RenderWhiteQuad({ m_position + glm::vec2{0.f,m_size.y},{title_sidebar_width, m_box_size},{},box_color });
	renderer->RenderWhiteQuad({ m_position + glm::vec2{title_sidebar_width+4*m_box_size+ title_width,m_size.y},{title_sidebar_width, m_box_size},{}, box_color });
	renderer->RenderText(m_title, { m_position + glm::vec2{ title_sidebar_width + 2*m_box_size ,m_size.y - m_line_height / 2.f } }, m_line_height * 0.9, box_color);
	auto lines_position = m_position;
	auto lines_size = m_size;
	lines_size.y -= m_box_size;
	lines_size.y -= m_line_height*0.5;
	lines_size.x -= m_box_size * 2;
	lines_position.x += m_box_size;
	lines_position.y += m_box_size;
	//Render line numbering:
	auto line_numbering_text_width = renderer->GetTextSize(std::to_string(m_lines.size()-1), m_line_height).x;
	renderer->RenderWhiteQuad({ 
		m_position + glm::vec2{ line_numbering_text_width +  m_box_size*2.5f, 0.f},
		{m_box_size*0.3f, m_size.y},
		{},box_color
		});
	int lines_to_render = lines_size.y / m_line_height;
	auto line_number_pos = m_position;
	line_number_pos.y += m_size.y-1.5*m_line_height;
	line_number_pos.x += m_box_size*1.5f;
	for (int i = m_first_line; i < std::min((int)m_lines.size(), m_first_line+lines_to_render); i++) {
		auto curr_number_text_width = renderer->GetTextSize(std::to_string(i), m_line_height).x;
		renderer->RenderText(
			std::to_string(i),
			line_number_pos+glm::vec2{line_numbering_text_width-curr_number_text_width,0},
			m_line_height,
			i == m_cursor_pos.y ? box_color : box_color*0.7f);
		line_number_pos.y -= m_line_height;
	}
	lines_position.x += m_box_size*2.f + line_numbering_text_width;

	//Render lines
	render_lines(renderer, lines_position, lines_size);

	//Highlight current line
	auto current_line_y = lines_position.y + lines_size.y - m_line_height - (m_cursor_pos.y - m_first_line) * m_line_height;
	renderer->RenderWhiteQuad({ glm::vec2{lines_position.x-m_box_size, current_line_y}, {m_size.x - lines_position.x + m_position.x ,m_box_size * 0.3f, },{},box_color });
	renderer->RenderWhiteQuad({ glm::vec2{lines_position.x - m_box_size, current_line_y+m_line_height}, {m_size.x - lines_position.x + m_position.x,m_box_size * 0.3f, },{},box_color });

}

void TextEditor::SetActive(bool active) {
	m_active = active;
}

void TextEditor::render_lines(graphics::Renderer* renderer, glm::vec2 position, glm::vec2 size) {
	int lines_to_render = size.y / m_line_height;

	glm::vec2 pos = { position.x, position.y + size.y - m_line_height };
	for (int i = m_first_line; i < std::min((int)m_lines.size(), m_first_line + lines_to_render); i++) {
		auto line = m_lines[i];
		renderer->RenderText(line, pos, m_line_height, { 1.f, 1.f, 1.f, 1.f });
		pos.y -= m_line_height;
	}
	auto cursor_line = m_lines[m_cursor_pos.y];
	auto text_till_cursor = cursor_line.substr(0, std::max(0, m_cursor_pos.x));
	auto text_size = renderer->GetTextSize(text_till_cursor, m_line_height);
	auto cursor_pos = position;
	cursor_pos.x += text_size.x;
	cursor_pos.y += size.y - (m_cursor_pos.y - m_first_line) * m_line_height - m_line_height;
	renderer->RenderWhiteQuad({ cursor_pos, {m_line_height * 0.1f, m_line_height},{},{1,1,1,1} });
}

