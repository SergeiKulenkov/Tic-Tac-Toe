#pragma once
#include <vector>
#include <memory>
#include <string_view>
#include "Object.h"

struct GLFWwindow;

////////////////////

class App
{
public:
	App();
	App(std::string_view name);
	~App();

	App(const App& app) = delete;
	App& operator=(const App& app) = delete;

	void Run();

	void AddObjectToDraw(std::shared_ptr<IDrawableObject> object) { m_ObjectsToDraw.emplace_back(object); }

	GLFWwindow* GetWindow() { return m_Window; }

	ImVec2 GetWindowSize() const { return ImVec2(width, height); }

private:
	void Init(std::string_view name = "");

	static constexpr uint16_t width = 1920;
	static constexpr uint16_t height = 1080;
	static constexpr ImVec4 clearColour = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);

	bool m_Running = false;

	float m_TimeStep = 0.0f;
	float m_FrameTime = 0.0f;
	float m_LastFrameTime = 0.0f;

	GLFWwindow* m_Window = nullptr;
	std::vector<std::shared_ptr<IDrawableObject>> m_ObjectsToDraw;
};