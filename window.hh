#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>

#include <glm.hpp>

class Window {

public:
	Window(std::string title, int width, int height, bool fullscreen = false);
	void MakeContextCurrent();
	bool ShouldClose();
	void Update();
	void SetFullscreen(bool fullscreen);
	glm::vec2 FramebufferSize();
	glm::vec2 WindowSize();
};

#endif