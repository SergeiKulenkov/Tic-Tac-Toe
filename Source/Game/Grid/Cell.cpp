#include "Cell.h"
#include "Grid.h"

////////////////////

Cell::Cell()
{
}

void Cell::Init(const uint8_t row, const uint8_t column)
{
	m_Type = CellType::Empty;
	m_TopLeft = ImVec2(cellSize * column + offsetX, cellSize * row + offsetY);
	m_BottomRight = ImVec2(cellSize * (column + 1) + offsetX, cellSize * (row + 1) + offsetY);
	m_Center = ImVec2(cellSize / 2 + m_TopLeft.x, cellSize / 2 + m_TopLeft.y);
}

void Cell::Draw(ImDrawList& drawList, const CellType winner) const
{
	drawList.AddRect(m_TopLeft, m_BottomRight, Cell::m_CellColour, 0, 0, gridThickness);
	switch (m_Type)
	{
		case CellType::Circle:
		{
			drawList.AddCircle(m_Center, symbolSize, ((winner != CellType::Circle) ? circleColour : winnerColour), 0, symbolThickness);
			break;
		}
		case CellType::Cross:
		{
			ImVec2 symbolTopLeft = ImVec2(m_Center.x - symbolSize, m_Center.y - symbolSize);
			ImVec2 symbolBottomRight = ImVec2(m_Center.x + symbolSize, m_Center.y + symbolSize);
			drawList.AddLine(symbolTopLeft, symbolBottomRight, ((winner != CellType::Cross) ? crossColour : winnerColour), symbolThickness);

			symbolTopLeft = ImVec2(m_Center.x + symbolSize, m_Center.y - symbolSize);
			symbolBottomRight = ImVec2(m_Center.x - symbolSize, m_Center.y + symbolSize);
			drawList.AddLine(symbolTopLeft, symbolBottomRight, ((winner != CellType::Cross) ? crossColour : winnerColour), symbolThickness);
			break;
		}
		default:
			break;
	}
}