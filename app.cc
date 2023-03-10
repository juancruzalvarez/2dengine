#include "app.hh"
#include "texture.hh"
#include "font_loading.hh"
#include "text_editor.hh"
#include "util.hh"
#include "file_utils.hh"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


App::App() {

	glfwInit();

	m_window = new Window{ "hello", 900, 700 };
	m_window->make_context_current();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);




	m_renderer = new graphics::Renderer();
	m_renderer->SetOrtho({ 900,700 });
	m_renderer->LoadFont("res/fonts/lucon.ttf", 84);
	
/*
	window_->set_key_callback([&](int glfw_key_code, int scancode, Input::Action action, int modifiers) {
		if (action == Input::Action::kPressed || action == Input::Action::kRepeat) {
			switch (glfw_key_code) {
			case GLFW_KEY_DELETE: {
				m_text_editor->Supr();
				break;
			}
			case GLFW_KEY_BACKSPACE: {
				m_text_editor->Erase();
				break;
			}
			case GLFW_KEY_ENTER: {
				m_text_editor->NewLine();
				break;
			}
			case GLFW_KEY_TAB: {
				m_text_editor->Tab();
				break;
			}
			case GLFW_KEY_RIGHT: {
				m_text_editor->MoveCursor(TextEditor::Direction::RIGHT);
				break;
			}
			case GLFW_KEY_LEFT: {
				m_text_editor->MoveCursor(TextEditor::Direction::LEFT);
				break;
			}
			case GLFW_KEY_UP: {
				m_text_editor->MoveCursor(TextEditor::Direction::UP);
				break;
			}
			case GLFW_KEY_DOWN: {
				m_text_editor->MoveCursor(TextEditor::Direction::DOWN);
				break;
			}

			}
		}
		
	});
	*/
	m_window->set_key_callback([&](int glfw_key_code, int scancode, Input::Action action, int modifiers) {
		if (action == Input::Action::kPressed || action == Input::Action::kRepeat) {
			switch (glfw_key_code) {
			case GLFW_KEY_DELETE: {
				//m_text_editor->Supr();
				break;
			}
			case GLFW_KEY_ENTER: {
				std::string path;
				m_file_tree->Enter(path);
				std::cout << "Path:" << path << "\n";
				break;
			}
			case GLFW_KEY_UP: {
				m_file_tree->Move(-1);
				break;
			}
			case GLFW_KEY_DOWN: {
				m_file_tree->Move(1);
				break;
			}

			}
		}

		});
	init_layout();
}

void App::Run() {
	while (!m_window->should_close())
	{
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		m_file_tree->Render(m_renderer);
		for (const auto& col : m_text_editors)
			for (const auto& text_editor : col)
				text_editor->Render(m_renderer);
		m_renderer->Flush();
		if (m_window->has_resized())
			handle_window_resize();
		m_window->update();
	}
}


void App::key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers){
}

void App::init_layout() {
	m_last_window_framebuffer_size = m_window->framebuffer_size();
	m_file_tree = new FileTree();
	m_file_tree->SetRootFolder("./src/");
	m_file_tree->SetSize({ kDefaultFileTreeWidth,m_last_window_framebuffer_size.y});
	m_file_tree->SetPosition({ 0,0 });
	m_file_tree->SetTextSize(16);
	m_text_editors = { {new TextEditor()}};
	m_current_text_editor_index = { 0,0 };
	get_current_editor()->SetPosition({kDefaultFileTreeWidth, 0});
	get_current_editor()->SetSize({ m_last_window_framebuffer_size.x - kDefaultFileTreeWidth, m_last_window_framebuffer_size.y});
	
}

TextEditor* App::get_current_editor() {
	return m_text_editors[m_current_text_editor_index.x][m_current_text_editor_index.y];
}

void App::handle_window_resize() {
	glm::vec2 new_size = m_window->framebuffer_size();
	glViewport(0, 0, new_size.x, new_size.y);
	m_renderer->SetOrtho(new_size);
	glm::vec2 scale_factor = new_size / m_last_window_framebuffer_size;

	m_file_tree->SetSize(m_file_tree->Size()*scale_factor);
	float current_x = m_file_tree->Size().x;
	for (const auto& col : m_text_editors) {
		float current_y = 0;
		for (const auto& text_editor : col) {
			text_editor->SetSize(text_editor->Size()*scale_factor);
			text_editor->SetPosition({ current_x, current_y });
			current_y += text_editor->Size().y;
		}
		current_x += col[0]->Size().x;
	}

}

App::~App() {
	glfwTerminate();
}