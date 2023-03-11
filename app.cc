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

	m_window = new Window{ "vs code?", 900, 700 };
	m_window->MakeContextCurrent();
	m_window->SetKeyCallback(
		[&](int glfw_keycode, int scancode, input::Action action, int mods) {
			//Mode switching
			if (action == input::Action::kReleased && glfw_keycode == GLFW_KEY_ESCAPE) {
				if (m_current_mode == Mode::kEditor) m_current_mode = Mode::kText;
				else m_current_mode = Mode::kEditor;
			}

			switch (m_current_mode) {
			case Mode::kEditor: {
				if (action == input::Action::kReleased || action == input::Action::kRepeat) {
					if (glfw_keycode == GLFW_KEY_TAB) {
						//Change between editor, console or file tree.
						if (m_current_view == View::kFileTree) m_current_view = View::kTextEditor;
						else if (m_current_view == View::kTextEditor) m_current_view = View::kConsole;
						else m_current_view = View::kFileTree;
					}
					if (glfw_keycode == GLFW_KEY_C) {
						//Add column of editors
						add_editor_col();
					}
					if (glfw_keycode == GLFW_KEY_R) {
						add_editor_in_current_col();
					}
					if (glfw_keycode == GLFW_KEY_D) {
						//Change selected editor, move right.
						get_current_editor()->SetActive(false);
						m_current_text_editor_index.x = std::max(0, std::min((int)m_text_editors.size() - 1, m_current_text_editor_index.x + 1));
						m_current_text_editor_index.y = std::max(0, std::min((int)m_text_editors[m_current_text_editor_index.x].size() - 1, m_current_text_editor_index.y));
						get_current_editor()->SetActive(true);

					}
					if (glfw_keycode == GLFW_KEY_A) {
						//Change selected editor, move left.
						get_current_editor()->SetActive(false);
						m_current_text_editor_index.x = std::max(0, std::min((int)m_text_editors.size() - 1, m_current_text_editor_index.x - 1));
						m_current_text_editor_index.y = std::max(0, std::min((int)m_text_editors[m_current_text_editor_index.x].size() - 1, m_current_text_editor_index.y));
						get_current_editor()->SetActive(true);
					}
					if (glfw_keycode == GLFW_KEY_S) {
						//Change selected editor, move left.
						get_current_editor()->SetActive(false);
						m_current_text_editor_index.y = std::max(0, std::min((int)m_text_editors[m_current_text_editor_index.x].size() - 1, m_current_text_editor_index.y + 1));
						get_current_editor()->SetActive(true);

					}
					if (glfw_keycode == GLFW_KEY_W) {
						//Change selected editor, move left.
						get_current_editor()->SetActive(false);
						m_current_text_editor_index.y = std::max(0, std::min((int)m_text_editors[m_current_text_editor_index.x].size() - 1, m_current_text_editor_index.y - 1));
						get_current_editor()->SetActive(true);

					}
				}
				break;
			}
			case Mode::kText: {
				switch (m_current_view) {
				case View::kFileTree: file_tree_key_callback(glfw_keycode, scancode, action, mods); break;
				case View::kTextEditor: text_editor_key_callback(glfw_keycode, scancode, action, mods); break;
				}
				break;
			}
			}
	});
	m_window->SetCharCallback(
		[&](uint32_t utf32_char) {
			if (m_current_mode == Mode::kEditor) return;
			switch (m_current_view) {
			case View::kFileTree: file_tree_char_callback(utf32_char); break;
			case View::kTextEditor: text_editor_char_callback(utf32_char); break;
			}
		}
	);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);




	m_renderer = new graphics::Renderer();
	m_renderer->SetOrtho({ 900,700 });
	m_renderer->LoadFont("res/fonts/lucon.ttf", 84);
	

	init_layout();
}

void App::add_editor_col() {
	auto new_editor = new TextEditor("", {});
	auto current_col = m_current_text_editor_index.x;
	auto new_width = m_text_editors[current_col][0]->Size().x * 0.5f;
	auto new_pos = m_text_editors[current_col][0]->Position() + glm::vec2{ new_width, 0.f };
	new_pos.y = 0;
	new_editor->SetSize({new_width, m_last_window_framebuffer_size.y});
	new_editor->SetPosition(new_pos);
	new_editor->SetLineHeight(16);
	m_text_editors.insert(m_text_editors.begin()+current_col+1, {new_editor});

	for (auto text_editor : m_text_editors[current_col]) {
		text_editor->SetSize({ new_width, text_editor->Size().y });
	}
}

void App::add_editor_in_current_col() {
	auto new_size = get_current_editor()->Size() * glm::vec2{ 1.f, 0.5f };
	auto new_editor = new TextEditor("", {});
	new_editor->SetSize(new_size);
	new_editor->SetPosition(get_current_editor()->Position());
	new_editor->SetLineHeight(16);
	auto new_pos = get_current_editor()->Position() + glm::vec2{0, new_size.y};
	get_current_editor()->SetPosition(new_pos);
		get_current_editor()->SetSize(new_size);
	m_text_editors[m_current_text_editor_index.x].push_back(new_editor);
}

void App::Run() {
	while (!m_window->ShouldClose())
	{
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		m_file_tree->Render(m_renderer);
		for (const auto& col : m_text_editors)
			for (const auto& text_editor : col)
				text_editor->Render(m_renderer);
		m_renderer->Flush();
		if (m_window->HasResized())
			handle_window_resize();
		m_window->Update();
	}
}


void App::key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers){
}

void App::init_layout() {
	m_last_window_framebuffer_size = m_window->FramebufferSize();
	m_file_tree = new FileTree();
	m_file_tree->SetRootFolder("./src/");
	m_file_tree->SetSize({ kDefaultFileTreeWidth,m_last_window_framebuffer_size.y});
	m_file_tree->SetPosition({ 0,0 });
	m_file_tree->SetTextSize(16);
	m_text_editors = { {new TextEditor("", 16)}};
	m_current_text_editor_index = { 0,0 };
	get_current_editor()->SetPosition({kDefaultFileTreeWidth, 0});
	get_current_editor()->SetSize({ m_last_window_framebuffer_size.x - kDefaultFileTreeWidth, m_last_window_framebuffer_size.y});
	get_current_editor()->SetActive(true);
	m_current_view = View::kTextEditor;
}

TextEditor* App::get_current_editor() {
	return m_text_editors[m_current_text_editor_index.x][m_current_text_editor_index.y];
}

void App::handle_window_resize() {
	glm::vec2 new_size = m_window->FramebufferSize();
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
	m_last_window_framebuffer_size = new_size;
}

void App::text_editor_key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers) {
	auto text_editor = get_current_editor();
	if (action == input::Action::kPressed || action == input::Action::kRepeat) {
		switch (glfw_key_code) {
		case GLFW_KEY_DELETE: {
			text_editor->Supr();
			break;
		}
		case GLFW_KEY_BACKSPACE: {
			text_editor->Erase();
			break;
		}
		case GLFW_KEY_ENTER: {
			text_editor->NewLine();
			break;
		}
		case GLFW_KEY_TAB: {
			text_editor->Tab();
			break;
		}
		case GLFW_KEY_RIGHT: {
			text_editor->MoveCursor(TextEditor::Direction::RIGHT);
			break;
		}
		case GLFW_KEY_LEFT: {
			text_editor->MoveCursor(TextEditor::Direction::LEFT);
			break;
		}
		case GLFW_KEY_UP: {
			text_editor->MoveCursor(TextEditor::Direction::UP);
			break;
		}
		case GLFW_KEY_DOWN: {
			text_editor->MoveCursor(TextEditor::Direction::DOWN);
			break;
		}

		}
	}
}

void App::text_editor_char_callback(uint32_t utf32_char) {
	get_current_editor()->Write(utf32_char);
}

void App::file_tree_key_callback(int glfw_key_code, int scancode, input::Action action, int modifiers) {
	if (action == input::Action::kPressed || action == input::Action::kRepeat) {
		switch (glfw_key_code) {
		case GLFW_KEY_DELETE: {
			//m_text_editor->Supr();
			break;
		}
		case GLFW_KEY_ENTER: {
			std::string path;
			m_file_tree->Enter(path);
			if (path != "") {
				//Open file to current text editor.
				std::vector<std::string> content;
				auto err = file_utils::GetContent(path, content);
				if (err == file_utils::FileUtilErr::NO_ERR) {
					file_utils::ChangeTabsToSpaces(content, 3);
					auto title = file_utils::GetFileNameFromPath(path);
					get_current_editor()->SetContent(content);
					get_current_editor()->SetTitle(title);
				}
			}
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
}

void App::file_tree_char_callback(uint32_t utf32_char){}

App::~App() {
	glfwTerminate();
}