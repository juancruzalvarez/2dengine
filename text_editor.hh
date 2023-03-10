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
void Render(graphics::Renderer *renderer);

void Write(uint8_t c);
void Tab();
void NewLine();
void Erase();
void Supr();
void MoveCursor(Direction dir);

void SetTitle(std::string title);
void SetSize(glm::vec2 size);
void SetPosition(glm::vec2 position);
glm::vec2 Position();
glm::vec2 Size();

void SetLineHeight(int line_height);
void SetBoxSize(int box_size);

void SetContent(std::vector<std::string> content);

private:

std::string m_title;
glm::vec2 m_position;
glm::vec2 m_size;

int m_line_height;
int m_box_size = 4;
int m_first_line;

glm::ivec2 m_cursor_pos;
std::vector<std::string> m_lines; 

void render_lines(graphics::Renderer *renderer, glm::vec2 position, glm::vec2 size);

};
#endif