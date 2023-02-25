#ifndef APP_H
#define APP_H

#include "window.hh"
#include "input.hh"
#include "renderer.hh"
#include "texture.hh"

class App {
public:
	App();
    ~App();
    void Run();

private:
	Window *window_;
	void key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers);
	graphics::Renderer* renderer_;
	graphics::Texture t;
};

#endif