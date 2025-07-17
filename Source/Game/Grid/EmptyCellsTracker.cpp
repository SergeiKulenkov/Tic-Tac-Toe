#include "EmptyCellsTracker.h"
#include <algorithm>

////////////////////

namespace Utils
{
	bool IsIndexInsideGrid(const int8_t row, const int8_t column, const uint8_t gridSize)
	{
		return (row >= 0) && (column >= 0) &&
				(row < gridSize) && (column < gridSize);
	}

	uint8_t GetIndex(const int8_t row, const int8_t column, const uint8_t gridSize)
	{
		return row * gridSize + column;
	}
}

////////////////////

void EmptyCellsTracker::Init(const uint8_t gridSize, bool twoPlayers)
{
	m_GridSize = gridSize;
	m_TwoPlayers = twoPlayers;

	Reset();
	m_BestIndexes.reserve(m_EmptyCells.size());
}

void EmptyCellsTracker::Reset()
{
	for (uint8_t i = 0; i < m_GridSize * m_GridSize; i++)
	{
		m_EmptyCells[i] = 0;
	}
}

std::vector<uint8_t> EmptyCellsTracker::GetBestIndexes()
{
	m_BestIndexes.clear();
	bool sameValue = true;
	for (const auto& [key, value] : m_EmptyCells)
	{
		if (value > 0)
		{
			m_BestIndexes.emplace_back(key);
			if (m_BestIndexes[0] != value) sameValue = false;
		}
	}

	if (!sameValue) std::sort(m_BestIndexes.begin(), m_BestIndexes.end(), [this](const uint8_t a, const uint8_t b) {return m_EmptyCells[a] > m_EmptyCells[b]; });

	return m_BestIndexes;
}

void EmptyCellsTracker::AddCellToMap(const uint8_t row, const uint8_t column)
{
	const uint8_t count = UpdateNeighboursInMap(row, column, false);
	m_EmptyCells[Utils::GetIndex(row, column, m_GridSize)] = count;
}

void EmptyCellsTracker::RemoveCellFromMap(const uint8_t row, const uint8_t column)
{
	m_EmptyCells.erase(Utils::GetIndex(row, column, m_GridSize));
	if (!m_TwoPlayers) UpdateNeighboursInMap(row, column, true);
}

uint8_t EmptyCellsTracker::UpdateNeighboursInMap(const uint8_t row, const uint8_t column, bool isIncreasing)
{
	uint8_t count = 0;
	for (int8_t i = -1; i <= 1; i++)
	{
		if (row + i >= 0 && row + i <= m_GridSize)
		{
			for (int8_t j = -1; j <= 1; j++)
			{
				if (((i != 0) || (j != 0)) &&
					Utils::IsIndexInsideGrid(row + i, column + j, m_GridSize))
				{
					const int8_t index = Utils::GetIndex(row + i, column + j, m_GridSize);
					if (m_EmptyCells.find(index) != m_EmptyCells.end())
					{
						m_EmptyCells[index] += isIncreasing ? 1 : (-1);
					}
					else count++;
				}
			}
		}
	}

	return count;
}
