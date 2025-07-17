#pragma once
#include "../App/App.h"
#include "../App/InputManager.h"

#include "Grid/Grid.h"
#include "GameMenu.h"

////////////////////

class Game
{
public:
	~Game();

	static void Init(std::string_view name = "");

	static void Shutdown();

	static Game& Get();

	void Run();

private:
	Game(std::string_view name);

	void GameOver(const CellType winner) { m_Menu->SetWinner(winner); }

	App m_App;

	InputManager m_InputManager;

	std::shared_ptr<Grid> m_Grid = std::make_shared<Grid>();

	std::shared_ptr<GameMenu> m_Menu = std::make_shared<GameMenu>();
};