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

	static const int kDefaultFileTreeWidth = 300;

	void set_view(View view);
	void init_layout();
	void handle_window_resize();
	TextEditor* get_current_editor();

	void key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers);
	Window* m_window;
	graphics::Renderer* m_renderer;
	glm::vec2 m_last_window_framebuffer_size;
	View m_current_view;
	FileTree *m_file_tree;
	std::vector<std::vector<TextEditor*>> m_text_editors;
	glm::ivec2 m_current_text_editor_index;
};

#endif