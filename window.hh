#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>

#include <GLFW/glfw3.h>
#include <glm.hpp>

class Window {

public:
	enum class ScreenMode {
		kFullscreen,
		kMaximized,
		kNormal,
	};
	Window(std::string title, int width, int height, bool fullscreen = false);
	void set_screen_mode(ScreenMode screen_mode);
	void set_visible(bool visible);
	glm::vec2 framebuffer_size();
	void make_context_current();
	glm::vec2 window_size();
	bool should_close();
	bool has_resized();
	void update();

private:
	GLFWwindow *window_handle_;
	ScreenMode screen_mode_;
	bool has_resized_;
	glm::vec2 framebuffer_size_;
	glm::vec2 window_size_;

};

#endif