#include "jenjin/targets/default.h"
#include "jenjin/engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Jenjin::Targets;

void DefaultTarget::PreRender() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	static int i = 0; if (i < 5) { i++;
		Jenjin::EngineRef->GetCurrentScene()->GetCamera()->SetPosition(glm::vec3(GetSize().x / 2, GetSize().y / 2, 0));
	}
}

glm::vec2 DefaultTarget::GetSize() {
	int width, height;
	static auto ctx = glfwGetCurrentContext();
	glfwGetWindowSize(ctx, &width, &height);

	return glm::vec2(width, height);
}

void DefaultTarget::Resize(glm::vec2 size) {
	static auto ctx = glfwGetCurrentContext();
	Jenjin::EngineRef->GetCurrentScene()->GetCamera()->Resize(size);
	glfwSetWindowSize(ctx, (int)size.x, (int)size.y);
	glViewport(0, 0, (int)size.x, (int)size.y);
}

glm::vec2 DefaultTarget::GetMousePosition() {
	static auto ctx = glfwGetCurrentContext();
	static double x, y;
	glfwGetCursorPos(ctx, &x, &y);
	return glm::vec2(x, y);
}
