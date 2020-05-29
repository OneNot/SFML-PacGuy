#include "GameManager.h"

GameState GameManager::gameState = GameState::Menu;
int GameManager::score = 0;
UITextElement* GameManager::UIScore = nullptr;
UITextElement* GameManager::UIWinLoseScore = nullptr;
UITextElement* GameManager::UIWinLoseScoreAnchorEl = nullptr;
UITextElement* GameManager::UICollected = nullptr;
sf::Clock* GameManager::scoreLossClock = nullptr;
int GameManager::CollectablesCollected = 0;
int GameManager::TotalNumOfCollectables = 0;
int GameManager::scoreLoss = 1;
float GameManager::scoreLossTime = 500;
bool GameManager::RunProgramLoop = true;
bool GameManager::RunInnerGameLoop = true;


void GameManager::EndGameScoreHandler()
{
	UIWinLoseScore->text.setString(std::to_string(score));
	UIWinLoseScore->RePosition(*UIWinLoseScoreAnchorEl, UIAnchor::LowMid);
}

void GameManager::AddToCollected()
{
	CollectablesCollected++;
	UICollected->text.setString("Collected: " + std::to_string(CollectablesCollected) + "/" + std::to_string(TotalNumOfCollectables));
	if (CollectablesCollected == TotalNumOfCollectables)
	{
		WinGame();
	}
}

void GameManager::UpdateScore(int scoreUpdate)
{
	score += scoreUpdate;
	UIScore->text.setString("Score: " + std::to_string(score));
}

GameState GameManager::GetGameState()
{
	return gameState;
}

void GameManager::PauseGame()
{
	gameState = GameState::Paused;
	AudioManager::sounds["waka"]->sound.stop();
}

void GameManager::ResumeGame()
{
	gameState = GameState::Playing;
}

void GameManager::TogglePauseGame()
{
	if (gameState == GameState::Paused)
		ResumeGame();
	else if (gameState == GameState::Playing)
		PauseGame();
}

void GameManager::WinGame()
{
	UpdateScore(1000);
	EndGameScoreHandler();
	AudioManager::sounds["waka"]->sound.stop();
	gameState = GameState::Won;
}

void GameManager::LoseGame()
{
	EndGameScoreHandler();
	AudioManager::sounds["waka"]->sound.stop();
	AudioManager::music.stop();
	AudioManager::sounds["death"]->sound.play();
	gameState = GameState::Lost;
}

void GameManager::HandleScoreLossTimer()
{
	if (scoreLossClock->getElapsedTime().asMilliseconds() >= scoreLossTime)
	{
		UpdateScore(-scoreLoss);
		scoreLossClock->restart();
	}
}

void GameManager::ForceSetGameState(GameState state)
{
	gameState = state;
}

void GameManager::RestartGame()
{
	score = CollectablesCollected = TotalNumOfCollectables = 0;
	Collectable::WipeCollectables();
	RunInnerGameLoop = false;
}
