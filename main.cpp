#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "include/Player.h"
#include "include/Bullet.h"
#include "include/Enemy.h"
#include "include/FastEnemy.h"
#include "include/BossEnemy.h"
#include "include/Menu.h"
#include "include/ScoreManager.h"
#include "include/PowerUp.h"

enum class GameState
{
    MENU,
    PLAYING,
    BOSS,
    GAME_OVER,
    HIGHSCORE
};

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Shooter");
    window.setFramerateLimit(60);

    // Background stars
    std::vector<sf::RectangleShape> stars;
    std::vector<float> starSpeeds;
    for (int i = 0; i < 100; i++)
    {
        sf::RectangleShape star(sf::Vector2f(2.0f, 2.0f));
        star.setFillColor(sf::Color::White);
        star.setPosition({(float)(rand() % 800), (float)(rand() % 600)});
        stars.push_back(star);
        starSpeeds.push_back(50.0f + rand() % 100);
    }

    GameState state = GameState::MENU;
    Menu menu(window);
    ScoreManager scoreManager;

    Player* player = new Player(375.0f, 500.0f);
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    std::vector<PowerUp*> powerups;
    BossEnemy* boss = nullptr;

    int score = 0;
    int enemyCount = 0;
    int bossesDefeated = 0;
    float spawnInterval = 2.0f;
    float powerupTimer = 0.0f;
    bool bossSpawned = false;
    bool showBossDefeated = false;
    int nextBossScore = 50;

    sf::Font font;
    font.openFromFile("assets/font.ttf");

    sf::Text scoreText(font, "Score: 0", 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.0f, 10.0f});

    sf::Text healthText(font, "Health: 100", 24);
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition({10.0f, 40.0f});

    sf::Text levelText(font, "Level: 1", 24);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition({10.0f, 70.0f});

    sf::Text exitHint(font, "ESC: Exit to Menu", 20);
    exitHint.setFillColor(sf::Color::White);
    exitHint.setPosition({580.0f, 10.0f});

    sf::Text gameOverText(font, "GAME OVER!", 48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({200.0f, 180.0f});

    sf::Text finalScoreText(font, "", 30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition({250.0f, 250.0f});

    sf::Text restartText(font, "R: Restart    M: Main Menu", 24);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition({180.0f, 520.0f});

    sf::Text bossDefeatedText(font, "BOSS DEFEATED! NEXT LEVEL!", 34);
    bossDefeatedText.setFillColor(sf::Color::Green);
    bossDefeatedText.setPosition({90.0f, 270.0f});

    sf::Clock spawnClock;
    sf::Clock clock;
    sf::Clock bossDefeatedClock;

    auto resetGame = [&]()
    {
        for (auto b : bullets) delete b;
        for (auto e : enemies) delete e;
        for (auto p : powerups) delete p;
        bullets.clear();
        enemies.clear();
        powerups.clear();
        if (boss) { delete boss; boss = nullptr; }
        score = 0;
        enemyCount = 0;
        bossesDefeated = 0;
        spawnInterval = 2.0f;
        nextBossScore = 50;
        bossSpawned = false;
        powerupTimer = 0.0f;
        showBossDefeated = false;
        delete player;
        player = new Player(375.0f, 500.0f);
        scoreText.setString("Score: 0");
        healthText.setString("Health: 100");
        levelText.setString("Level: 1");
    };

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Stars update
        for (int i = 0; i < (int)stars.size(); i++)
        {
            stars[i].move({0.0f, starSpeeds[i] * deltaTime});
            if (stars[i].getPosition().y > 600)
                stars[i].setPosition({(float)(rand() % 800), 0.0f});
        }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                auto* keyEvent = event->getIf<sf::Event::KeyPressed>();

                // MENU
                if (state == GameState::MENU)
                {
                    if (keyEvent->code == sf::Keyboard::Key::Up)
                        menu.moveUp();
                    if (keyEvent->code == sf::Keyboard::Key::Down)
                        menu.moveDown();
                    if (keyEvent->code == sf::Keyboard::Key::Enter)
                    {
                        if (menu.getSelectedIndex() == 0)
                            state = GameState::PLAYING;
                        if (menu.getSelectedIndex() == 1)
                            state = GameState::HIGHSCORE;
                        if (menu.getSelectedIndex() == 2)
                            window.close();
                    }
                }

                // HIGH SCORE
                if (state == GameState::HIGHSCORE)
                {
                    if (keyEvent->code == sf::Keyboard::Key::M)
                        state = GameState::MENU;
                }

                // PLAYING + BOSS
                if (state == GameState::PLAYING || state == GameState::BOSS)
                {
                    if (keyEvent->code == sf::Keyboard::Key::Escape)
                    {
                        scoreManager.addScore(score);
                        resetGame();
                        state = GameState::MENU;
                    }

                    if (keyEvent->code == sf::Keyboard::Key::Space)
                    {
                        if (player->canShoot())
                        {
                            Bullet* b = new Bullet(
                                player->getPosition().x + 22.0f,
                                player->getPosition().y
                            );
                            bullets.push_back(b);
                        }
                    }
                }

                // GAME OVER
                if (state == GameState::GAME_OVER)
                {
                    if (keyEvent->code == sf::Keyboard::Key::R)
                    {
                        resetGame();
                        state = GameState::PLAYING;
                    }
                    if (keyEvent->code == sf::Keyboard::Key::M)
                    {
                        resetGame();
                        state = GameState::MENU;
                    }
                }
            }
        }

        // =====================
        // PLAYING UPDATE
        // =====================
        if (state == GameState::PLAYING)
        {
            // Boss defeated message timer
            if (showBossDefeated && bossDefeatedClock.getElapsedTime().asSeconds() > 2.5f)
                showBossDefeated = false;

            // Health boxes drop — sirf boss defeat ke baad
            if (bossesDefeated > 0)
            {
                powerupTimer += deltaTime;
                if (powerupTimer >= 3.0f && powerups.size() == 0)
                {
                    powerups.push_back(new PowerUp(300.0f, -30.0f));
                    powerupTimer = 0.0f;
                }
                if (powerupTimer >= 5.0f && powerups.size() == 1)
                {
                    powerups.push_back(new PowerUp(450.0f, -30.0f));
                    powerupTimer = 999.0f; // done
                }
            }

            // Boss spawn check
            if (score >= nextBossScore && !bossSpawned)
            {
                for (auto e : enemies) delete e;
                enemies.clear();
                bossSpawned = true;
                powerupTimer = 999.0f;
                for (auto p : powerups) delete p;
                powerups.clear();
                state = GameState::BOSS;
                boss = new BossEnemy(350.0f, 80.0f);
            }

            // Enemy spawn
            if (spawnClock.getElapsedTime().asSeconds() > spawnInterval && !bossSpawned)
            {
                float randomX = rand() % 720;
                enemyCount++;

                if (enemyCount % 3 == 0)
                    enemies.push_back(new FastEnemy(randomX, -50.0f));
                else
                    enemies.push_back(new Enemy(randomX, -50.0f));

                spawnClock.restart();
            }

            player->update(deltaTime);
            for (auto b : bullets) b->update(deltaTime);
            for (auto e : enemies) e->update(deltaTime);
            for (auto p : powerups) p->update(deltaTime);

            // PowerUp-Player Collision
            for (auto p : powerups)
            {
                if (p->getIsActive())
                {
                    if (p->getBounds().findIntersection(player->getBounds()).has_value())
                    {
                        int newHealth = player->getHealth() + 20;
                        if (newHealth > player->getMaxHealth())
                            newHealth = player->getMaxHealth();
                        player->setHealth(newHealth);
                        healthText.setString("Health: " + std::to_string(player->getHealth()));
                        p->setIsActive(false);
                    }
                }
            }

            // Bullet-Enemy Collision
            for (auto b : bullets)
            {
                for (auto e : enemies)
                {
                    if (b->getIsActive() && e->getIsActive())
                    {
                        if (b->getBounds().findIntersection(e->getBounds()).has_value())
                        {
                            b->setIsActive(false);
                            e->takeDamage(b->getDamage());
                            if (e->isDead())
                            {
                                score += e->getPointValue();
                                scoreText.setString("Score: " + std::to_string(score));
                            }
                        }
                    }
                }
            }

            // Enemy-Player Collision
            for (auto e : enemies)
            {
                if (e->getIsActive())
                {
                    if (e->getBounds().findIntersection(player->getBounds()).has_value())
                    {
                        player->takeDamage(20);
                        e->setIsActive(false);
                        healthText.setString("Health: " + std::to_string(player->getHealth()));
                        if (player->isDead())
                        {
                            scoreManager.addScore(score);
                            state = GameState::GAME_OVER;
                            finalScoreText.setString("Final Score: " + std::to_string(score));
                        }
                    }
                }
            }

            // Cleanup
            for (int i = bullets.size() - 1; i >= 0; i--)
                if (!bullets[i]->getIsActive())
                { delete bullets[i]; bullets.erase(bullets.begin() + i); }

            for (int i = enemies.size() - 1; i >= 0; i--)
                if (!enemies[i]->getIsActive())
                { delete enemies[i]; enemies.erase(enemies.begin() + i); }

            for (int i = powerups.size() - 1; i >= 0; i--)
                if (!powerups[i]->getIsActive())
                { delete powerups[i]; powerups.erase(powerups.begin() + i); }
        }

        // =====================
        // BOSS UPDATE
        // =====================
        if (state == GameState::BOSS && boss)
        {
            player->update(deltaTime);
            boss->update(deltaTime);
            for (auto b : bullets) b->update(deltaTime);

            // Bullet-Boss Collision
            for (auto b : bullets)
            {
                if (b->getIsActive() && boss->getIsActive())
                {
                    if (b->getBounds().findIntersection(boss->getBounds()).has_value())
                    {
                        b->setIsActive(false);
                        boss->takeDamage(b->getDamage());

                        if (boss->isDefeated())
                        {
                            score += boss->getPointValue();
                            scoreText.setString("Score: " + std::to_string(score));
                            scoreManager.addScore(score);

                            delete boss;
                            boss = nullptr;

                            bossesDefeated++;

                            // Difficulty increase
                            spawnInterval -= 0.3f;
                            if (spawnInterval < 0.5f) spawnInterval = 0.5f;

                            // Next boss threshold
                            nextBossScore = score + 100 + (bossesDefeated * 50);
                            bossSpawned = false;
                            powerupTimer = 0.0f;

                            levelText.setString("Level: " + std::to_string(bossesDefeated + 1));
                            showBossDefeated = true;
                            bossDefeatedClock.restart();

                            state = GameState::PLAYING;
                        }
                    }
                }
            }

            // Boss-Player Collision
            if (boss && boss->getIsActive())
            {
                if (boss->getBounds().findIntersection(player->getBounds()).has_value())
                {
                    player->takeDamage(30);
                    healthText.setString("Health: " + std::to_string(player->getHealth()));
                    if (player->isDead())
                    {
                        scoreManager.addScore(score);
                        state = GameState::GAME_OVER;
                        finalScoreText.setString("Final Score: " + std::to_string(score));
                    }
                }
            }

            // Cleanup bullets
            for (int i = bullets.size() - 1; i >= 0; i--)
                if (!bullets[i]->getIsActive())
                { delete bullets[i]; bullets.erase(bullets.begin() + i); }
        }

        // =====================
        // DRAW
        // =====================
        window.clear(sf::Color::Black);

        // Stars background
        if (state == GameState::PLAYING || state == GameState::BOSS)
            for (auto& star : stars)
                window.draw(star);

        if (state == GameState::MENU)
        {
            menu.draw(window);
        }
        else if (state == GameState::HIGHSCORE)
        {
            sf::Text title(font, "HIGH SCORES", 48);
            title.setFillColor(sf::Color::Cyan);
            title.setPosition({230.0f, 80.0f});
            window.draw(title);

            auto scores = scoreManager.getHighScores();
            if (scores.empty())
            {
                sf::Text noScore(font, "No scores yet! Play first.", 24);
                noScore.setFillColor(sf::Color::White);
                noScore.setPosition({180.0f, 280.0f});
                window.draw(noScore);
            }
            else
            {
                for (int i = 0; i < (int)scores.size(); i++)
                {
                    sf::Text st(font, std::to_string(i+1) + ".   " + std::to_string(scores[i]), 36);
                    st.setFillColor(sf::Color::Yellow);
                    st.setPosition({300.0f, 180.0f + i * 60.0f});
                    window.draw(st);
                }
            }

            sf::Text back(font, "Press M to go back", 24);
            back.setFillColor(sf::Color::White);
            back.setPosition({270.0f, 530.0f});
            window.draw(back);
        }
        else if (state == GameState::PLAYING || state == GameState::BOSS)
        {
            player->draw(window);
            for (auto b : bullets) b->draw(window);
            for (auto e : enemies) e->draw(window);
            for (auto p : powerups) p->draw(window);
            if (state == GameState::BOSS && boss)
                boss->draw(window);

            window.draw(scoreText);
            window.draw(healthText);
            window.draw(levelText);
            window.draw(exitHint);

            if (showBossDefeated)
                window.draw(bossDefeatedText);
        }
        else if (state == GameState::GAME_OVER)
        {
            window.draw(gameOverText);
            window.draw(finalScoreText);
            window.draw(restartText);

            sf::Text hs(font, "TOP SCORES:", 28);
            hs.setFillColor(sf::Color::Cyan);
            hs.setPosition({300.0f, 320.0f});
            window.draw(hs);

            auto scores = scoreManager.getHighScores();
            for (int i = 0; i < (int)scores.size(); i++)
            {
                sf::Text st(font, std::to_string(i+1) + ".  " + std::to_string(scores[i]), 24);
                st.setFillColor(sf::Color::White);
                st.setPosition({310.0f, 360.0f + i * 35.0f});
                window.draw(st);
            }
        }

        window.display();
    }

    delete player;
    if (boss) delete boss;
    for (auto b : bullets) delete b;
    for (auto e : enemies) delete e;
    for (auto p : powerups) delete p;

    return 0;
}