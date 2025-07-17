#pragma once
#include <stdint.h>
#include <unordered_map>
#include <vector>

////////////////////

namespace Utils
{
	bool IsIndexInsideGrid(const int8_t row, const int8_t column, const uint8_t gridSize);

	uint8_t GetIndex(const int8_t row, const int8_t column, const uint8_t gridSize);
}

////////////////////

class EmptyCellsTracker
{
public:
	EmptyCellsTracker() {}

	void Init(const uint8_t gridSize, bool twoPlayers);

	void Reset();

	size_t GetMapSize() const { return m_EmptyCells.size(); }

	std::vector<uint8_t> GetBestIndexes();

	void AddCellToMap(const uint8_t row, const uint8_t column);
	void AddCellToMap(const uint8_t index) { AddCellToMap(index / m_GridSize, index % m_GridSize); }

	void RemoveCellFromMap(const uint8_t row, const uint8_t column);
	void RemoveCellFromMap(const uint8_t index) { RemoveCellFromMap(index / m_GridSize, index % m_GridSize); }

	// returns a count of non empty neighbours
	uint8_t UpdateNeighboursInMap(const uint8_t row, const uint8_t column, bool isIncreasing);

private:
	uint8_t m_GridSize = 0;

	bool m_TwoPlayers = false;

	// mapping empty cell index and number of neighbours
	std::unordered_map<uint8_t, uint8_t> m_EmptyCells;

	// indexes with a neighbour count >= 1
	std::vector<uint8_t> m_BestIndexes;
};