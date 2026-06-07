#include "../include/ScoreManager.h"

ScoreManager::ScoreManager()
{
    filename = "data/highscores.txt";
    maxScores = 5;
    loadFromFile();
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::loadFromFile()
{
    highScores.clear();
    std::ifstream file(filename);

    if (file.is_open())
    {
        int score;
        while (file >> score)
            highScores.push_back(score);
        file.close();
    }
}

void ScoreManager::saveToFile()
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int s : highScores)
            file << s << "\n";
        file.close();
    }
}

void ScoreManager::addScore(int score)
{
    highScores.push_back(score);
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());

    if (highScores.size() > maxScores)
        highScores.resize(maxScores);

    saveToFile();
}

std::vector<int> ScoreManager::getHighScores() const
{
    return highScores;
}

bool ScoreManager::isHighScore(int score) const
{
    if (highScores.size() < maxScores) return true;
    return score > highScores.back();
}