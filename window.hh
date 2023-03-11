#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <functional>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "input.hh"

class Window {

public:
	enum class ScreenMode {
		kFullscreen,
		kMaximized,
		kNormal,
	};


	Window(std::string title, int width, int height, bool fullscreen = false);
	void SetCharCallback(input::CharCallbackFn callback);
	void SetKeyCallback(input::KeyCallbackFn callback);
	void SetScreenMode(ScreenMode screen_mode);
	void MakeContextCurrent();
	void SetVisible(bool visible);
	glm::vec2 FramebufferSize();
	glm::vec2 WindowSize();
	bool ShouldClose();
	bool HasResized();
	void Update();

private:
	GLFWwindow *m_window_handle;
	ScreenMode m_screen_mode;
	glm::vec2 m_framebuffer_size;
	glm::vec2 m_window_size;
	bool m_has_resized;
	input::CharCallbackFn m_char_callback;
	input::KeyCallbackFn m_key_callback;

};

#endif