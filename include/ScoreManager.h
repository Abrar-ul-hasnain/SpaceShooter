#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

class ScoreManager
{
private:
    std::vector<int> highScores;
    std::string filename;
    int maxScores;

public:
    ScoreManager();
    ~ScoreManager();

    void addScore(int score);
    void saveToFile();
    void loadFromFile();
    std::vector<int> getHighScores() const;
    bool isHighScore(int score) const;
};

#endif