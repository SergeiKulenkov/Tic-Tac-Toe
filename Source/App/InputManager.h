#pragma once
#include "glm/glm.hpp"
#include "KeyCodes.h"

struct GLFWwindow;

////////////////////

class InputManager
{
public:
	void Init(GLFWwindow* window);

	static bool IsKeyDown(KeyCode keycode);

	static bool IsMouseButtonDown(MouseButton button);

	static bool IsMouseButtonUp(MouseButton button);

	static glm::vec2 GetMousePosition();

private:
	inline static GLFWwindow* m_Window = nullptr;
};