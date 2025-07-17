#pragma once
#include <array>
#include <vector>
#include <functional>

#include "../../App/Object.h"
#include "Cell.h"
#include "EmptyCellsTracker.h"

class GameMenu;

////////////////////

enum class GridConfig
{
	None = -1,
	Config3,
	Config5,
	Config7,
	Count
};

////////////////////

enum class GameOverType
{
	None,
	Current_Player,
	Tie,
	Count
};

////////////////////

struct PairScoreDepth
{
	int16_t score = 0;
	uint16_t depth = 0;
};

////////////////////

class Grid : public IDrawableObject
{
public:
	Grid();

	using GameOverCallback = std::function<void(CellType)>;
	void SettGameOverCallback(const GameOverCallback& callback) { m_GameOverCallback = callback; }

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	void SetMenuEvents(GameMenu& menu);

	void Start(bool twoPlayers, bool humanFirst, const GridConfig config);

	void SetColour(const ImVec4& colour) { Cell::m_CellColour = ImGui::ColorConvertFloat4ToU32(colour); }

private:
	void InitCells();

	void Reset();

	void MakeMove(const uint8_t row, const uint8_t column);
	void MakeMove(const uint8_t index) { MakeMove(index / m_Size, index % m_Size); }

	uint8_t FindBotBestMove();

	void Finish(const GameOverType gameOverType);
 
	PairScoreDepth Minimax(const uint16_t depth, int16_t alpha, int16_t beta, bool isMaximizingPlayer, const CellType currentPlayer);

	uint8_t Evaluate(const CellType currentPlayer);

	bool IsGameOverPossible() const;
	bool IsRandomMoveAllowed() const;
	bool IsMouseInputAllowed() const;

	GameOverType CheckGameOver(const uint8_t row, const uint8_t column, const CellType currentPlayer);
	GameOverType CheckGameOver(const uint8_t index, const CellType currentPlayer) { return CheckGameOver(index / m_Size, index % m_Size, currentPlayer); }

	bool CheckCell(const uint8_t index, const CellType type);
	bool CheckRow(const uint8_t row, const CellType currentPlayer);
	bool CheckColumn(const uint8_t column, const CellType currentPlayer);
	bool CheckDiagonal(const uint8_t row, const uint8_t column, const CellType currentPlayer, bool isTopLeft);
	bool CheckTie();

	static constexpr uint8_t depth = 4;
	static constexpr uint8_t turnsToGameOver = 4;
	static constexpr std::array<uint8_t, static_cast<uint8_t>(GridConfig::Count)> sizes = { 3, 5, 7 };
	static constexpr std::array<uint8_t, static_cast<uint8_t>(GameOverType::Count)> scores = { 0, 10, 5 };

	uint8_t m_Size;
	std::vector<Cell> m_Cells;
	EmptyCellsTracker m_EmptyCellsTracker;

	bool m_IsNoMinimaxAllowed = true;
	bool m_MouseClicked = false;
	bool m_Running = false;
	bool m_TwoPlayers = false;
	bool m_HumanFirst = false;

	CellType m_CurrentPlayer = CellType::Empty;
	CellType m_Winner = CellType::Empty;

	GameOverCallback m_GameOverCallback;
};