#pragma once
#include <string_view>
#include <array>

#include "../App//Object.h"
#include "Grid/Grid.h"

////////////////////

class GameMenu : public IDrawableObject
{
public:
	virtual void Update(float deltaTime) override {}
	virtual void Draw() override;

	void Init(const ImVec2& windowSize, const ImVec4& gridColour);

	void SetWinner(const CellType winner);

	using StartGridCallback = std::function<void(bool, bool, GridConfig)>;
	void SetStartGridCallback(const StartGridCallback& callback) { m_StartGridCallback = callback; }

	using GridColourCallback = std::function<void(const ImVec4&)>;
	void SetGridColourCallback(const GridColourCallback& callback) { m_GridColourCallback = callback; }

private:
	void UpdateGridConfigSelection();

	static constexpr float offsetY = 100.f;
	static constexpr ImVec2 startSize = ImVec2(220, 220);
	static constexpr ImVec2 gameOverSize = ImVec2(220, 220);

	static constexpr std::string_view startSettingsText = "Settings";
	static constexpr std::string_view twoPlayersText = "Two Players";
	static constexpr std::string_view humanFirstText = "Human First";
	static constexpr std::string_view startButtonText = "Start";

	static constexpr std::string_view winnerText = "Winner - %s!";
	static constexpr std::string_view tieText = "Tie!";
	static constexpr std::string_view gameOverText = "Game Over";
	static constexpr std::string_view restartButtonText = "Restart";

	static constexpr std::string_view gridColourPickerText = "Gird Colour";
	static constexpr std::string_view applyColourText = "Apply Colour";

	static constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	static constexpr ImGuiColorEditFlags colourPickerFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;

	ImVec2 m_Position = ImVec2(0, 0);
	ImVec4 m_GridColour = ImVec4(0, 0, 0, 0);

	std::array<bool, static_cast<uint8_t>(GridConfig::Count)> m_GridConfigSelection = { true, false, false };
	char m_GridConfigText[3] = { '3', 'x', '3' };
	GridConfig m_GridConfig = GridConfig::Config3;

	CellType m_Winner = CellType::Empty;
	bool m_TwoPlayers = false;
	bool m_HumanFirst = false;
	bool m_GridConfigSelected = true;
	bool m_Started = false;
	bool m_IsGameOver = false;

	StartGridCallback m_StartGridCallback;
	GridColourCallback m_GridColourCallback;
};