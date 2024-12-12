#pragma once
#include <array>
#include "glm/glm.hpp"
#include "../App/Object.h"

////////////////////

enum class CellType
{
	Empty,
	Circle,
	Cross
};

////////////////////

struct Cell
{
	CellType cellType;
	int index;
};

////////////////////

class Grid : public IDrawableObject
{
public:
	Grid();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	void CheckGameOver(const int row, const int column);

	bool CheckCell(const int index, const CellType type);
	bool CheckRow(const int row, const CellType currentPlayer);
	bool CheckColumn(const int column, const CellType currentPlayer);
	bool CheckDiagonals(const int row, const int column, const CellType currentPlayer);
	bool CheckTopLeftDiagonal(const CellType currentPlayer);
	bool CheckTopRightDiagonal(const CellType currentPlayer);

	static constexpr int m_Size = 3;
	const float m_OffsetX = 5;
	const float m_OffsetY = 5;
	const int m_CellSize = 200;
	const float m_GridThickness = 2.5f;
	const int m_SymbolSize = 150;
	const float  m_SymbolThickness = 4.5f;
	const ImVec4 m_GridColour = ImVec4(120 / 255.f, 38 / 255.f, 180 / 255.f, 1.f);
	const ImVec4 m_CircleColour = ImVec4(40 / 255.f, 200 / 255.f, 180 / 255.f, 1.f);
	const ImVec4 m_CrossColour = ImVec4(35 / 255.f, 210 / 255.f, 110 / 255.f, 1.f);
	const ImVec4 m_WinnerColour = ImVec4(240 / 255.f, 230 / 255.f, 40 / 255.f, 1.f);

	std::array<Cell, m_Size * m_Size> m_Cells;

	glm::vec2 m_MousePosition;

	bool m_FirstPlayer = true;
	bool m_MouseClicked = false;
	CellType m_Winner = CellType::Empty;

	ImDrawList* m_DrawList;
};