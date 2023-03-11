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
	enum class View {
		kTextEditor,
		kFileTree,
		kConsole,
	};

	enum class Mode {
		kText,
		kEditor,
	};

	static const int kDefaultFileTreeWidth = 300;

	void init_layout();
	void handle_window_resize();
	TextEditor* get_current_editor();
	void add_editor_col();
	void add_editor_in_current_col();
	void key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers);
	void text_editor_key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers);
	void text_editor_char_callback(uint32_t utf32_char);
	void file_tree_key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers);
	void file_tree_char_callback(uint32_t utf32_char);

	Window* m_window;
	graphics::Renderer* m_renderer;
	View m_current_view;
	Mode m_current_mode;
	glm::vec2 m_last_window_framebuffer_size;
	FileTree *m_file_tree;
	std::vector<std::vector<TextEditor*>> m_text_editors;
	glm::ivec2 m_current_text_editor_index;

};

#endif