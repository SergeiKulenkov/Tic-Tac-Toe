#include "Grid.h"
#include "Game.h"
#include "../App/InputManager.h"

Grid::Grid()
{
	for (int i = 0; i < m_Cells.size(); i++)
	{
		m_Cells[i] = Cell(CellType::Empty, i);
	}
}

void Grid::Update(float deltaTime)
{
	if ((m_Winner == CellType::Empty) &&
		!m_MouseClicked && InputManager::IsMouseButtonDown(MouseButton::Left))
	{
		m_MouseClicked = true;
		m_MousePosition = InputManager::GetMousePosition();

		if ((m_MousePosition.x < m_CellSize * m_Size) &&
			(m_MousePosition.y < m_CellSize * m_Size))
		{
			const int row = m_MousePosition.y / m_CellSize;
			const int column = m_MousePosition.x / m_CellSize;
			if (m_Cells[row * m_Size + column].cellType == CellType::Empty)
			{
				m_Cells[row * m_Size + column].cellType = m_FirstPlayer ? CellType::Circle : CellType::Cross;

				CheckGameOver(row, column);
				m_FirstPlayer = !m_FirstPlayer;
			}
		}
	}
	else if (InputManager::IsMouseButtonUp(MouseButton::Left))
	{
		m_MouseClicked = false;
	}
}

void Grid::Draw()
{
	ImVec2 topLeft;
	ImVec2 bottomRight;
	ImVec2 center;

	for (int row = 0; row < m_Size; row++)
	{
		for (int column = 0; column < m_Size; column++)
		{
			topLeft = ImVec2(m_CellSize * column + m_OffsetX, m_CellSize * row + m_OffsetY);
			bottomRight = ImVec2(m_CellSize * (column + 1) + m_OffsetX, m_CellSize * (row + 1) + m_OffsetY);

			m_DrawList = ImGui::GetBackgroundDrawList();
			m_DrawList->AddRect(topLeft, bottomRight, ImGui::GetColorU32(m_GridColour), 0, 0, m_GridThickness);

			center = ImVec2(m_CellSize / 2 + topLeft.x, m_CellSize / 2 + topLeft.y);
			switch (m_Cells[row * m_Size + column].cellType)
			{
				case CellType::Circle:
				{
					ImVec4 colour = (m_Winner != CellType::Circle) ? m_CircleColour : m_WinnerColour;
					m_DrawList->AddCircle(center, m_SymbolSize / 2, ImGui::GetColorU32(colour), 0, m_SymbolThickness);
					break;
				}
				case CellType::Cross:
				{
					ImVec4 colour = (m_Winner != CellType::Cross) ? m_CrossColour : m_WinnerColour;
					topLeft = ImVec2(center.x - m_SymbolSize / 2, center.y - m_SymbolSize / 2);
					bottomRight = ImVec2(center.x + m_SymbolSize / 2, center.y + m_SymbolSize / 2);
					m_DrawList->AddLine(topLeft, bottomRight, ImGui::GetColorU32(colour), m_SymbolThickness);

					topLeft = ImVec2(center.x + m_SymbolSize / 2, center.y - m_SymbolSize / 2);
					bottomRight = ImVec2(center.x - m_SymbolSize / 2, center.y + m_SymbolSize / 2);
					m_DrawList->AddLine(topLeft, bottomRight, ImGui::GetColorU32(colour), m_SymbolThickness);
					break;
				}
				default:
					break;
			}
		}
	}

}

void Grid::CheckGameOver(const int row, const int column)
{
	const CellType currentPlayer = m_FirstPlayer ? CellType::Circle : CellType::Cross;
	if (CheckRow(row, currentPlayer) || CheckColumn(column, currentPlayer) || CheckDiagonals(row, column, currentPlayer))
	{
		m_Winner = currentPlayer;
		// send game over event
	}
}

bool Grid::CheckCell(const int index, const CellType type)
{
	return (m_Cells[index].cellType != CellType::Empty) &&
			(m_Cells[index].cellType == type);
}

bool Grid::CheckRow(const int row, const CellType currentPlayer)
{
	bool isGameOver = true;
	for (int i = row * m_Size; i < (row + 1) * m_Size; i++)
	{
		isGameOver = CheckCell(i, currentPlayer);
		if (!isGameOver) break;
	}
	return isGameOver;
}

bool Grid::CheckColumn(const int column, const CellType currentPlayer)
{
	bool isGameOver = true;
	for (int i = column; i < m_Size * m_Size; i = i + m_Size)
	{
		isGameOver = CheckCell(i, currentPlayer);
		if (!isGameOver) break;
	}
	return isGameOver;
}

bool Grid::CheckDiagonals(const int row, const int column, const CellType currentPlayer)
{
	// false because the cell may not be on a diagonal
	bool isGameOver = false;
	if ((row == 0 && column == 0) ||
		(row == (m_Size - 1) && column == (m_Size - 1)))
	{
		isGameOver = CheckTopLeftDiagonal(currentPlayer);
	}
	else if ((row == 0 && column == (m_Size - 1)) ||
			(row == (m_Size - 1) && column == 0))
	{
		isGameOver = CheckTopRightDiagonal(currentPlayer);
	}
	else if ((row == m_Size / 2) &&
			(column == m_Size / 2))
	{
		isGameOver = CheckTopLeftDiagonal(currentPlayer) || CheckTopRightDiagonal(currentPlayer);
	}

	return isGameOver;
}

bool Grid::CheckTopLeftDiagonal(const CellType currentPlayer)
{
	bool isGameOver = true;
	for (int i = 0; i < m_Size * m_Size; i = i + (m_Size + 1))
	{
		isGameOver = CheckCell(i, currentPlayer);
		if (!isGameOver) break;
	}
	return isGameOver;
}

bool Grid::CheckTopRightDiagonal(const CellType currentPlayer)
{
	bool isGameOver = true;
	for (int i = m_Size - 1; i <= m_Size * (m_Size - 1); i = i + (m_Size - 1))
	{
		isGameOver = CheckCell(i, currentPlayer);
		if (!isGameOver) break;
	}
	return isGameOver;
}
