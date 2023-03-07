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

	window_ = new Window{ "hello", 900, 700 };
	window_->make_context_current();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);




	renderer_ = new graphics::Renderer();
	renderer_->SetOrtho({ 900,700 });
	renderer_->LoadFont("res/fonts/lucon.ttf", 84);
	m_text_editor = new TextEditor();
	m_text_editor->SetLineHeight(20);
	m_text_editor->SetTitle("app.cc");
	m_text_editor->SetSize({ 800,700 });
	std::vector<std::string> content;
	if (file_utils::GetContent("app.cc", content) == file_utils::FileUtilErr::NO_ERR) {
		file_utils::ChangeTabsToSpaces(content, 3);
		m_text_editor->SetContent(content);
	}
	else
		m_text_editor->SetContent({ "" });
	window_->set_char_callback([&](uint8_t c) {
		m_text_editor->Write(c);
		});
	m_file_tree = new FileTree();
	m_file_tree->SetRootFolder("./src/");
	m_file_tree->SetSize({200,700});
	m_file_tree->SetTextSize(16);
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
	window_->set_key_callback([&](int glfw_key_code, int scancode, Input::Action action, int modifiers) {
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

}
void App::Run() {
	while (!window_->should_close())
	{
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		m_text_editor->Render(renderer_, {200,0});
		m_file_tree->Render(renderer_, { 0,0 });

		renderer_->Flush();
		if (window_->has_resized())
			std::cout << "\nRESIZED!!!!" << "\n";
		window_->update();
	}
}


void App::key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers){
}

App::~App() {
	glfwTerminate();
}