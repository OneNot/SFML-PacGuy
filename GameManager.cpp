#include "GameManager.h"

GameState GameManager::gameState = GameState::Menu;
int GameManager::score = 0;
UITextElement* GameManager::UIScore = nullptr;
UITextElement* GameManager::UIWinScore = nullptr;
UITextElement* GameManager::UICollected = nullptr;
sf::Clock* GameManager::scoreLossClock = nullptr;
int GameManager::CollectablesCollected = 0;
int GameManager::TotalNumOfCollectables = 0;
int GameManager::scoreLoss = 1;
float GameManager::scoreLossTime = 500;


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
	if (AudioManager::sounds["waka"]->sound.getStatus() == sf::SoundSource::Status::Playing)
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
	UIWinScore->text.setString(std::to_string(score));//TODO: needs to be re-centered
	gameState = GameState::Won;
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
