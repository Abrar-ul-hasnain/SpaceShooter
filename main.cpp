#include <SFML/Graphics.hpp>
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

enum class GameState
{
    MENU,
    PLAYING,
    BOSS,
    VICTORY,
    GAME_OVER
};

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Shooter");
    window.setFramerateLimit(60);

    GameState state = GameState::MENU;
    Menu menu(window);

    Player* player = new Player(375.0f, 500.0f);
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    BossEnemy* boss = nullptr;

    int score = 0;
    int enemyCount = 0;

    sf::Font font;
    font.openFromFile("assets/font.ttf");

    sf::Text scoreText(font, "Score: 0", 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.0f, 10.0f});

    sf::Text healthText(font, "Health: 100", 24);
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition({10.0f, 40.0f});

    sf::Text gameOverText(font, "GAME OVER!", 48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({200.0f, 250.0f});

    sf::Text victoryText(font, "YOU WIN! 🎉", 48);
    victoryText.setFillColor(sf::Color::Green);
    victoryText.setPosition({180.0f, 250.0f});

    sf::Text finalScoreText(font, "", 30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition({250.0f, 320.0f});

    sf::Text restartText(font, "R: Restart    M: Main Menu", 24);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition({180.0f, 380.0f});

    sf::Text bossWarning(font, "!! BOSS INCOMING !!", 40);
    bossWarning.setFillColor(sf::Color::Red);
    bossWarning.setPosition({150.0f, 270.0f});

    sf::Clock spawnClock;
    sf::Clock clock;
    sf::Clock bossWarningClock;
    bool showBossWarning = false;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                auto* keyEvent = event->getIf<sf::Event::KeyPressed>();

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
                        if (menu.getSelectedIndex() == 2)
                            window.close();
                    }
                }

                if (state == GameState::PLAYING || state == GameState::BOSS)
                {
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

                if (state == GameState::GAME_OVER || state == GameState::VICTORY)
                {
                    if (keyEvent->code == sf::Keyboard::Key::R)
                    {
                        for (auto b : bullets) delete b;
                        for (auto e : enemies) delete e;
                        bullets.clear();
                        enemies.clear();
                        if (boss) { delete boss; boss = nullptr; }
                        score = 0;
                        enemyCount = 0;
                        delete player;
                        player = new Player(375.0f, 500.0f);
                        scoreText.setString("Score: 0");
                        healthText.setString("Health: 100");
                        state = GameState::PLAYING;
                    }
                    if (keyEvent->code == sf::Keyboard::Key::M)
                    {
                        for (auto b : bullets) delete b;
                        for (auto e : enemies) delete e;
                        bullets.clear();
                        enemies.clear();
                        if (boss) { delete boss; boss = nullptr; }
                        score = 0;
                        enemyCount = 0;
                        delete player;
                        player = new Player(375.0f, 500.0f);
                        scoreText.setString("Score: 0");
                        healthText.setString("Health: 100");
                        state = GameState::MENU;
                    }
                }
            }
        }

        // PLAYING UPDATE
        if (state == GameState::PLAYING)
        {
            if (spawnClock.getElapsedTime().asSeconds() > 2.0f)
            {
                float randomX = rand() % 720;
                enemyCount++;

                // Score 100 pe boss spawn
                if (score >= 100 && enemies.empty())
                {
                    showBossWarning = true;
                    bossWarningClock.restart();
                    state = GameState::BOSS;
                    boss = new BossEnemy(350.0f, 80.0f);
                }
                else if (enemyCount % 3 == 0)
                {
                    enemies.push_back(new FastEnemy(randomX, -50.0f));
                }
                else
                {
                    enemies.push_back(new Enemy(randomX, -50.0f));
                }

                spawnClock.restart();
            }

            player->update(deltaTime);
            for (auto b : bullets) b->update(deltaTime);
            for (auto e : enemies) e->update(deltaTime);

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
                            state = GameState::GAME_OVER;
                            finalScoreText.setString("Final Score: " + std::to_string(score));
                        }
                    }
                }
            }

            // Cleanup
            for (int i = bullets.size() - 1; i >= 0; i--)
            {
                if (!bullets[i]->getIsActive())
                { delete bullets[i]; bullets.erase(bullets.begin() + i); }
            }
            for (int i = enemies.size() - 1; i >= 0; i--)
            {
                if (!enemies[i]->getIsActive())
                { delete enemies[i]; enemies.erase(enemies.begin() + i); }
            }
        }

        // BOSS UPDATE
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
                            finalScoreText.setString("Final Score: " + std::to_string(score));
                            state = GameState::VICTORY;
                        }
                    }
                }
            }

            // Boss-Player Collision
            if (boss->getIsActive())
            {
                if (boss->getBounds().findIntersection(player->getBounds()).has_value())
                {
                    player->takeDamage(30);
                    healthText.setString("Health: " + std::to_string(player->getHealth()));
                    if (player->isDead())
                    {
                        state = GameState::GAME_OVER;
                        finalScoreText.setString("Final Score: " + std::to_string(score));
                    }
                }
            }

            // Cleanup bullets
            for (int i = bullets.size() - 1; i >= 0; i--)
            {
                if (!bullets[i]->getIsActive())
                { delete bullets[i]; bullets.erase(bullets.begin() + i); }
            }
        }

        // DRAW
        window.clear(sf::Color::Black);

        if (state == GameState::MENU)
        {
            menu.draw(window);
        }
        else if (state == GameState::PLAYING)
        {
            player->draw(window);
            for (auto b : bullets) b->draw(window);
            for (auto e : enemies) e->draw(window);
            window.draw(scoreText);
            window.draw(healthText);
        }
        else if (state == GameState::BOSS)
        {
            player->draw(window);
            for (auto b : bullets) b->draw(window);
            if (boss) boss->draw(window);
            window.draw(scoreText);
            window.draw(healthText);
        }
        else if (state == GameState::VICTORY)
        {
            window.draw(victoryText);
            window.draw(finalScoreText);
            window.draw(restartText);
        }
        else if (state == GameState::GAME_OVER)
        {
            window.draw(gameOverText);
            window.draw(finalScoreText);
            window.draw(restartText);
        }

        window.display();
    }

    delete player;
    if (boss) delete boss;
    for (auto b : bullets) delete b;
    for (auto e : enemies) delete e;

    return 0;
}