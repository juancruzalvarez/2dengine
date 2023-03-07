#ifndef APP_H
#define APP_H

#include <map>

#include "window.hh"
#include "input.hh"
#include "renderer.hh"
#include "texture.hh"
#include "font_loading.hh"
#include "text_editor.hh"
#include "file_tree.hh"

class App {
public:
	App();
    ~App();
    void Run();

private:
	Window *window_;
	void key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers);
	graphics::Renderer* renderer_;
	TextEditor* m_text_editor;
	FileTree* m_file_tree;
};

#endif