#pragma once
#include "../App/App.h"
#include "../App/InputManager.h"
#include "Grid.h"

////////////////////

class Game
{
public:
	~Game();

	static void Init(std::string name = "");

	static void Shutdown();

	static Game& Get();

	void Run();

private:
	Game(std::string name);

	App m_App;

	InputManager m_InputManager;

	std::shared_ptr<Grid> m_Grid = std::make_shared<Grid>();
};