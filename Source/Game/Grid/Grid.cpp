#include "Grid.h"

#include <random>
#include "glm/glm.hpp"

#include "../../App/InputManager.h"
#include "../Game.h"

////////////////////

Grid::Grid()
{
	m_Size = sizes[0];
	InitCells();
}

void Grid::SetMenuEvents(GameMenu& menu)
{
	menu.SetStartGridCallback([this](bool twoPlayers, bool humanFirst, const GridConfig config) { Start(twoPlayers, humanFirst, config); });
	menu.SetGridColourCallback([this](const ImVec4& colour) { SetColour(colour); });
}

void Grid::Start(bool twoPlayers, bool humanFirst, const GridConfig config)
{
	Reset();
	m_TwoPlayers = twoPlayers;

	const uint8_t newSize = sizes[static_cast<uint8_t>(config)];
	if (m_Size != newSize)
	{
		m_Size = newSize;
		InitCells();
	}
	
	if (m_EmptyCellsTracker.GetMapSize() == m_Cells.size()) m_EmptyCellsTracker.Reset();
	else m_EmptyCellsTracker.Init(m_Size, m_TwoPlayers);

	m_CurrentPlayer = CellType::Cross;
	if (!m_TwoPlayers)
	{
		m_HumanFirst = humanFirst;
		if (!m_HumanFirst) MakeMove(FindBotBestMove());
	}

	m_Running = true;
}

void Grid::Finish(const GameOverType gameOverType)
{
	m_Running = false;
	m_Winner = (gameOverType != GameOverType::Tie) ? m_CurrentPlayer : CellType::Empty;
	m_GameOverCallback(m_Winner);
}

void Grid::InitCells()
{
	m_Cells.resize(m_Size * m_Size);
	for (uint8_t row = 0; row < m_Size; row++)
	{
		for (uint8_t column = 0; column < m_Size; column++)
		{
			m_Cells[Utils::GetIndex(row, column, m_Size)].Init(row, column);
		}
	}
}

void Grid::Reset()
{
	for (Cell& cell : m_Cells) cell.SetType(CellType::Empty);
	m_Running = false;
	m_IsNoMinimaxAllowed = true;
	m_Winner = CellType::Empty;
}

void Grid::MakeMove(const uint8_t row, const uint8_t column)
{
	m_Cells[Utils::GetIndex(row, column, m_Size)].SetType(m_CurrentPlayer);
	const GameOverType gameOverType = IsGameOverPossible() ? CheckGameOver(row, column, m_CurrentPlayer) : GameOverType::None;

	if (gameOverType != GameOverType::None)
	{
		Finish(gameOverType);
	}
	else
	{
		if (!m_TwoPlayers || !IsGameOverPossible())
		{
			if (m_IsNoMinimaxAllowed) m_IsNoMinimaxAllowed = false;
			m_EmptyCellsTracker.RemoveCellFromMap(row, column);
		}
		m_CurrentPlayer = (m_CurrentPlayer == CellType::Cross) ? CellType::Circle : CellType::Cross;
	}
}

void Grid::Update(float deltaTime)
{
	if (m_Running)
	{
		if (IsMouseInputAllowed())
		{
			if (!m_MouseClicked && InputManager::IsMouseButtonDown(MouseButton::Left))
			{
				m_MouseClicked = true;
				const glm::vec2 m_MousePosition = InputManager::GetMousePosition();

				if ((m_MousePosition.x < Cell::cellSize * m_Size) &&
					(m_MousePosition.y < Cell::cellSize * m_Size))
				{
					const uint8_t row = static_cast<uint8_t>(m_MousePosition.y / Cell::cellSize);
					const uint8_t column = static_cast<uint8_t>(m_MousePosition.x / Cell::cellSize);

					if (m_Cells[Utils::GetIndex(row, column, m_Size)].GetType() == CellType::Empty)
					{
						MakeMove(row, column);
					}
				}
			}
			else if (InputManager::IsMouseButtonUp(MouseButton::Left))
			{
				m_MouseClicked = false;
			}
		}
		else
		{
			MakeMove(FindBotBestMove());
		}
	}
}

void Grid::Draw()
{
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	if (drawList != nullptr)
	{
		for (Cell& cell : m_Cells)
		{
			cell.Draw(*drawList, m_Winner);
		}
	}
}

uint8_t Grid::FindBotBestMove()
{
	uint8_t index = 0;
	if (IsRandomMoveAllowed())
	{
		std::random_device device;
		std::mt19937 random(device());
		std::uniform_int_distribution<std::mt19937::result_type> distribution(0, static_cast<int>( m_Cells.size() - 1));
		index = distribution(random);
	}
	else
	{
		PairScoreDepth bestResult(INT16_MIN, INT16_MIN);
		GameOverType gameOverType;
		PairScoreDepth result;

		for (const uint8_t bestIndex : m_EmptyCellsTracker.GetBestIndexes())
		{
			m_Cells[bestIndex].SetType(m_CurrentPlayer);
			m_EmptyCellsTracker.RemoveCellFromMap(bestIndex);
			gameOverType = IsGameOverPossible() ? CheckGameOver(bestIndex, m_CurrentPlayer) : GameOverType::None;

			if (gameOverType == GameOverType::Current_Player)
			{
				index = bestIndex;
				break;
			}
			else if (gameOverType == GameOverType::Tie)
			{
				result.score = scores[static_cast<uint8_t>(gameOverType)];
				result.depth = depth;
			}
			else
			{
				result = Minimax(depth, INT16_MIN, INT16_MAX, false, (m_CurrentPlayer == CellType::Cross) ? CellType::Circle : CellType::Cross);
			}

			m_Cells[bestIndex].SetType(CellType::Empty);
			m_EmptyCellsTracker.AddCellToMap(bestIndex);

			if (result.score > bestResult.score)
			{
				bestResult.score = result.score;
				bestResult.depth = result.depth;
				index = bestIndex;
			}
			else if (result.score == bestResult.score)
			{
				if (((result.score > 0) && (result.depth > bestResult.depth)) ||
					((result.score < 0) && (result.depth < bestResult.depth)))
				{
					bestResult.depth = result.depth;
					index = bestIndex;
				}
			}
		}
	}

	return index;
}

PairScoreDepth Grid::Minimax(const uint16_t depth, int16_t alpha, int16_t beta, bool isMaximizingPlayer, const CellType currentPlayer)
{
	PairScoreDepth result;
	if (depth != 0)
	{
		GameOverType gameOverType;
		PairScoreDepth currentResult;
		PairScoreDepth bestResult(0, INT16_MIN);
		bestResult.score = isMaximizingPlayer ? INT16_MIN : INT16_MAX;

		for (const uint8_t bestIndex : m_EmptyCellsTracker.GetBestIndexes())
		{
			m_Cells[bestIndex].SetType(currentPlayer);
			m_EmptyCellsTracker.RemoveCellFromMap(bestIndex);
			gameOverType = IsGameOverPossible() ? CheckGameOver(bestIndex, currentPlayer) : GameOverType::None;

			if (gameOverType != GameOverType::None)
			{
				m_Cells[bestIndex].SetType(CellType::Empty);
				m_EmptyCellsTracker.AddCellToMap(bestIndex);
				result.depth = depth - 1;
				result.score = scores[static_cast<uint8_t>(gameOverType)];

				if (!isMaximizingPlayer && (gameOverType != GameOverType::Tie)) result.score *= -1;
				break;
			}
			else
			{
				currentResult = Minimax(depth - 1, alpha, beta, !isMaximizingPlayer, (currentPlayer == CellType::Cross) ? CellType::Circle : CellType::Cross);
				m_Cells[bestIndex].SetType(CellType::Empty);
				m_EmptyCellsTracker.AddCellToMap(bestIndex);

				if ((isMaximizingPlayer && currentResult.score > bestResult.score) ||
					(!isMaximizingPlayer && currentResult.score < bestResult.score))
				{
					bestResult.score = currentResult.score;
					bestResult.depth = currentResult.depth;
				}
				else if (currentResult.score == bestResult.score)
				{
					if (((currentResult.score > 0) && (currentResult.depth > bestResult.depth)) ||
						((currentResult.score < 0) && (currentResult.depth < bestResult.depth)))
					{
						bestResult.depth = currentResult.depth;
					}
				}

				if (isMaximizingPlayer) alpha = std::max(alpha, bestResult.score);
				else beta = std::min(beta, bestResult.score);

				if (alpha >= beta) break;
			}
		}

		if (result.score == 0)
		{
			result = bestResult;
		}	
	}
	else
	{
		result.score = Evaluate(currentPlayer);
		if (!isMaximizingPlayer) result.score *= -1;
	}
	
	return result;
}

uint8_t Grid::Evaluate(const CellType currentPlayer)
{
	uint8_t score = 0;
	for (uint8_t index = 0; index < m_Cells.size(); index++)
	{
		if (m_Cells[index].GetType() == CellType::Empty)
		{
			m_Cells[index].SetType(currentPlayer);
			const GameOverType gameOverType = CheckGameOver(index, currentPlayer);
			score = scores[static_cast<uint8_t>(gameOverType)];

			m_Cells[index].SetType(CellType::Empty);
			if (score != 0) break;
		}
	}

	if (score == 0)
	{
		score = scores[static_cast<uint8_t>(GameOverType::Tie)];
	}
	return score;
}

////////////////////
// Conditions

bool Grid::IsGameOverPossible() const
{
	// game over can be on the 5th turn
	return (m_EmptyCellsTracker.GetMapSize() <= m_Cells.size() - turnsToGameOver);
}

bool Grid::IsRandomMoveAllowed() const
{
	return (m_IsNoMinimaxAllowed && !m_TwoPlayers && (m_CurrentPlayer == CellType::Cross));
}

bool Grid::IsMouseInputAllowed() const
{
	return (m_TwoPlayers || (m_HumanFirst && (m_CurrentPlayer == CellType::Cross)) ||
			(!m_HumanFirst && (m_CurrentPlayer == CellType::Circle)));
}

////////////////////
// Checks

GameOverType Grid::CheckGameOver(const uint8_t row, const uint8_t column, const CellType currentPlayer)
{
	bool gameOver = CheckRow(row, currentPlayer);
	if (!gameOver) gameOver = CheckColumn(column, currentPlayer);
	if (!gameOver) gameOver = CheckDiagonal(row, column, currentPlayer, true);
	if (!gameOver) gameOver = CheckDiagonal(row, column, currentPlayer, false);

	GameOverType type = (gameOver) ? GameOverType::Current_Player : GameOverType::None;
	if ((type == GameOverType::None) && CheckTie())
	{
		type = GameOverType::Tie;
	}
	return type;
}

bool Grid::CheckCell(const uint8_t index, const CellType type)
{
	return (m_Cells[index].GetType() == type);
}

bool Grid::CheckRow(const uint8_t row, const CellType currentPlayer)
{
	bool isGameOver = false;
	for (uint8_t i = row * m_Size + 1; i < (row + 1) * m_Size - 1; i += 1)
	{
		if (CheckCell(i - 1, currentPlayer) && CheckCell(i, currentPlayer) && CheckCell(i + 1, currentPlayer))
		{
			isGameOver = true;
			break;
		}
	}
	return isGameOver;
}

bool Grid::CheckColumn(const uint8_t column, const CellType currentPlayer)
{
	bool isGameOver = false;
	for (uint8_t i = column + m_Size; i < m_Size * (m_Size - 1); i += m_Size)
	{
		if (CheckCell(i - m_Size, currentPlayer) && CheckCell(i, currentPlayer) && CheckCell(i + m_Size, currentPlayer))
		{
			isGameOver = true;
			break;
		}
	}
	return isGameOver;
}

bool Grid::CheckDiagonal(const uint8_t row, const uint8_t column, const CellType currentPlayer, bool isTopLeft)
{
	bool isGameOver = false;
	const uint8_t offset = (isTopLeft) ? (m_Size + 1) : (m_Size - 1);

	for (int8_t i = row - 1; i < m_Size - 1; i++)
	{
		for (int8_t j = column - 1; j < m_Size - 1; j++)
		{
			if (Utils::IsIndexInsideGrid(i - 1, j - 1, m_Size) && Utils::IsIndexInsideGrid(i, j, m_Size) && Utils::IsIndexInsideGrid(i + 1, j + 1, m_Size))
			{
				const int8_t index = Utils::GetIndex(i, j, m_Size);
				if (CheckCell(index - offset, currentPlayer) && CheckCell(index, currentPlayer) && CheckCell(index + offset, currentPlayer))
				{
					isGameOver = true;
					break;
				}
			}
		}
		if (isGameOver) break;
	}
	return isGameOver;
}

bool Grid::CheckTie()
{
	bool tie = true;
	for (const Cell& cell : m_Cells)
	{
		tie = (cell.GetType() != CellType::Empty);
		if (!tie) break;
	}

	return tie;
}
