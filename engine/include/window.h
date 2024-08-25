#ifndef JENJIN_WINDOW_H
#define JENJIN_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Jenjin {
class Window {
public:
	// Create a new window
	bool NewWindow(int width, int height, const char* title);

	// Setting callbacks
	void setResizeCallback(GLFWframebuffersizefun callback);
	void setMouseCallback(GLFWcursorposfun callback);
	void setKeyCallback(GLFWkeyfun callback);

	// Getter for window
	GLFWwindow* getWindow(void) { return m_window; }

private:
	GLFWwindow* m_window;
};
};

#endif // JENJIN_WINDOW_H
