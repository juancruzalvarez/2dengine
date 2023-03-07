#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdint.h>

#include <vector>

#include <glm.hpp>

#include "renderer.hh"

class TextEditor {

public:

enum class Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

TextEditor();
void Render(graphics::Renderer *renderer, glm::vec2 position);
void Write(uint8_t c);
void Tab();
void NewLine();
void Erase();
void Supr();
void MoveCursor(Direction dir);
void SetLineHeight(int line_height);
void SetBoxSize(int box_size);
void SetTitle(std::string title);
void SetSize(glm::vec2 size);
void SetContent(std::vector<std::string> content);

private:
int m_line_height;
int m_box_size = 4;
int m_first_line;
glm::vec2 m_size;
std::string m_title;
glm::ivec2 m_cursor_pos;
std::vector<std::string> m_lines; 

void RenderLines(graphics::Renderer *renderer, glm::vec2 position, glm::vec2 size);

};
#endif