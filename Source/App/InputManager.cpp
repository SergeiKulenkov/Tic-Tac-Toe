#include "InputManager.h"
#include "glfw3.h"

void InputManager::Init(GLFWwindow* window)
{
	m_Window = window;
}

bool InputManager::IsKeyDown(KeyCode keycode)
{
	assert(m_Window);
	int state = glfwGetKey(m_Window, (int)keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputManager::IsMouseButtonDown(MouseButton button)
{
	assert(m_Window);
	int state = glfwGetMouseButton(m_Window, (int)button);
	return state == GLFW_PRESS;
}

bool InputManager::IsMouseButtonUp(MouseButton button)
{
	assert(m_Window);
	int state = glfwGetMouseButton(m_Window, (int)button);
	return state == GLFW_RELEASE;
}

glm::vec2 InputManager::GetMousePosition()
{
	assert(m_Window);
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	return { (float)x, (float)y };
}
