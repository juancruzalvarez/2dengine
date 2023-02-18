#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include <glm.hpp>

class Window {

public:
	enum class ScreenMode {
		kFullscreen,
		kMaximized,
		kNormal,
	};

	enum class Action {
		kPressed,
		kReleased,
		kRepeat,
	};

	enum Modifiers {
		kShift = 1,
		kCtrl = 2,
		kAlt = 4,
		kSuper = 8,
		kCapsLock = 16,
		kNumLock = 32,
	};

	Window(std::string title, int width, int height, bool fullscreen = false);
	void set_key_callback(std::function<void(int key, int scancode, Action action, int modifiers)>callback);
	void set_char_callback(std::function<void(int utf32_character)> callback);
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
	std::function<void(int key, int scancode, Action action, int modifiers)>key_callback_;
	std::function<void(int utf32_character)>char_callback_;

};

#endif