#pragma once
#include <stdint.h>

#include "imgui.h"

////////////////////

enum class CellType
{
	Empty,
	Circle,
	Cross
};

////////////////////

class Grid;

class Cell
{
public:
	Cell();

	friend class Grid;

	void Init(const uint8_t row, const uint8_t column);

	void Draw(ImDrawList& drawList, const CellType winner) const;

	CellType GetType() const { return m_Type; }
	void SetType(const CellType type) { m_Type = type; }

	static uint32_t GetColour() { return m_CellColour; }
	
private:
	static constexpr float cellSize = 153.f;
	static constexpr float offsetX = 5.f;
	static constexpr float offsetY = 5.f;
	static constexpr float gridThickness = 2.5f;
	static constexpr float symbolSize = 62.f;
	static constexpr float symbolThickness = 4.5f;

	static constexpr uint32_t circleColour = 4290037800; // ImVec4(40 / 255.f, 200 / 255.f, 180 / 255.f, 1.f);
	static constexpr uint32_t crossColour = 4285452835; // ImVec4(35 / 255.f, 210 / 255.f, 110 / 255.f, 1.f);
	static constexpr uint32_t winnerColour = 4280870640; // ImVec4(240 / 255.f, 230 / 255.f, 40 / 255.f, 1.f);

	inline static uint32_t m_CellColour = 4289996408; // ImVec4(120 / 255.f, 38 / 255.f, 180 / 255.f, 1.f);

	CellType m_Type = CellType::Empty;
	ImVec2 m_TopLeft = ImVec2(0, 0);
	ImVec2 m_BottomRight = ImVec2(0, 0);
	ImVec2 m_Center = ImVec2(0, 0);
};