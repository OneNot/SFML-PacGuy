#pragma once
#include "GameState.h"
#include "UITextElement.h"
#include <string>
#include "AudioManager.h"
#include "Collectable.h"

static class GameManager
{
private:
	static int score;
	static int CollectablesCollected;
	static GameState gameState;
	static void EndGameScoreHandler();

public:
	static bool RunProgramLoop;
	static bool RunInnerGameLoop;
	static int scoreLoss;
	static float scoreLossTime;
	static sf::Clock* scoreLossClock;
	static UITextElement* UIScore;
	static UITextElement* UIWinLoseScore;
	static UITextElement* UIWinLoseScoreAnchorEl;
	static UITextElement* UICollected;
	static int TotalNumOfCollectables;
	static void AddToCollected();
	static void UpdateScore(int scoreUpdate);
	static GameState GetGameState();
	static void PauseGame();
	static void ResumeGame();
	static void TogglePauseGame();
	static void WinGame();
	static void LoseGame();
	static void HandleScoreLossTimer();
	static void ForceSetGameState(GameState state);
	static void RestartGame();
};

