#include "GameMenu.h"

////////////////////

void GameMenu::Draw()
{
	if (!m_Started)
	{
		ImGui::SetNextWindowPos(ImVec2(m_Position.x - startSize.x / 2, m_Position.y - startSize.y / 2));
		ImGui::SetNextWindowSize(startSize);

		ImGui::Begin(startSettingsText.data(), NULL, windowFlags);
		ImGui::Checkbox(twoPlayersText.data(), &m_TwoPlayers);

		if (m_TwoPlayers) ImGui::BeginDisabled();
		ImGui::Checkbox(humanFirstText.data(), &m_HumanFirst);
		if (m_TwoPlayers) ImGui::EndDisabled();

		for (uint8_t i = 0; i < m_GridConfigSelection.size(); i++)
		{
			if (m_GridConfigSelected && !m_GridConfigSelection[i])
			{
				ImGui::BeginDisabled();
			}

			m_GridConfigText[0] = '0' + (i * 2 + 3);
			m_GridConfigText[2] = '0' + (i * 2 + 3);
			if (ImGui::Checkbox(m_GridConfigText, &m_GridConfigSelection[i]))
			{
				m_GridConfig = m_GridConfigSelection[i] ? static_cast<GridConfig>(i) : GridConfig::None;
				UpdateGridConfigSelection();
			}

			if (m_GridConfigSelected && !m_GridConfigSelection[i])
			{
				ImGui::EndDisabled();
			}

			if (i != m_GridConfigSelection.size() - 1)
			{
				ImGui::SameLine();
			}
		}

		ImGui::NewLine();
		ImGui::Text(gridColourPickerText.data());
		ImGui::SameLine();
		ImGui::ColorEdit4(gridColourPickerText.data(), (float*) &m_GridColour, colourPickerFlags);
		if (ImGui::Button(applyColourText.data()))
		{
			m_GridColourCallback(m_GridColour);
		}

		ImGui::NewLine();
		if (ImGui::Button(startButtonText.data()))
		{
			if (m_GridConfigSelected)
			{
				m_Started = true;
				m_StartGridCallback(m_TwoPlayers, m_HumanFirst, m_GridConfig);
			}
		}

		ImGui::End();
	}

	if (m_IsGameOver)
	{
		ImGui::SetNextWindowPos(ImVec2(m_Position.x - gameOverSize.x / 2, m_Position.y - gameOverSize.y / 2));
		ImGui::SetNextWindowSize(gameOverSize);
		ImGui::Begin(gameOverText.data());

		if (m_Winner != CellType::Empty)
		{
			ImGui::Text(winnerText.data(), ((m_Winner == CellType::Cross) ? "X" : "O"));
		}
		else
		{
			ImGui::Text(tieText.data());
		}

		if (ImGui::Button(restartButtonText.data()))
		{
			m_IsGameOver = false;
			m_Started = false;
			m_Winner = CellType::Empty;
		}

		ImGui::End();
	}
}

void GameMenu::Init(const ImVec2& windowSize, const ImVec4& gridColour)
{
	m_Position = ImVec2(windowSize.x / 2, windowSize.y / 2 - offsetY);
	m_GridColour = gridColour;
}

void GameMenu::SetWinner(const CellType winner)
{
	m_IsGameOver = true;
	m_Winner = winner;
}

void GameMenu::UpdateGridConfigSelection()
{
	const int8_t index = static_cast<uint8_t>(m_GridConfig);
	m_GridConfigSelected = (index != -1);

	for (uint8_t i = 0; i < m_GridConfigSelection.size(); i++)
	{
		m_GridConfigSelection[i] = (i == index);
	}
}