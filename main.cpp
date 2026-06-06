#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "include/Player.h"
#include "include/Bullet.h"
#include "include/Enemy.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Shooter");
    window.setFramerateLimit(60);

    Player player(375.0f, 500.0f);
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;

    int score = 0;

    // Font
    sf::Font font;
    bool fontLoaded = font.openFromFile("assets/font.ttf");

    // Score Text
    sf::Text scoreText(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.0f, 10.0f});
    scoreText.setString("Score: 0");

    // Health Text
    sf::Text healthText(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition({10.0f, 40.0f});
    healthText.setString("Health: 100");

    // Game Over Text
    sf::Text gameOverText(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({200.0f, 250.0f});
    gameOverText.setString("GAME OVER!");

    // Score Final Text
    sf::Text finalScoreText(font);
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition({250.0f, 320.0f});

    bool gameOver = false;

    sf::Clock spawnClock;
    sf::Clock clock;

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

                // Shoot
                if (keyEvent->code == sf::Keyboard::Key::Space && !gameOver)
                {
                    Bullet* b = new Bullet(
                        player.getPosition().x + 22.0f,
                        player.getPosition().y
                    );
                    bullets.push_back(b);
                }

                // R press karo restart ke liye
                if (keyEvent->code == sf::Keyboard::Key::R && gameOver)
                {
                    // Sab clear karo
                    for (auto b : bullets) delete b;
                    for (auto e : enemies) delete e;
                    bullets.clear();
                    enemies.clear();
                    score = 0;
                    gameOver = false;
                    player = Player(375.0f, 500.0f);
                    scoreText.setString("Score: 0");
                    healthText.setString("Health: 100");
                }
            }
        }

        if (!gameOver)
        {
            // Spawn enemies
            if (spawnClock.getElapsedTime().asSeconds() > 2.0f)
            {
                float randomX = rand() % 750;
                Enemy* e = new Enemy(randomX, -50.0f);
                enemies.push_back(e);
                spawnClock.restart();
            }

            // Update
            player.update(deltaTime);
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
                    if (e->getBounds().findIntersection(player.getBounds()).has_value())
                    {
                        player.takeDamage(20);
                        e->setIsActive(false);
                        healthText.setString("Health: " + 
                            std::to_string(player.getHealth()));

                        if (player.isDead())
                        {
                            gameOver = true;
                            finalScoreText.setString("Final Score: " + 
                                std::to_string(score));
                        }
                    }
                }
            }

            // Cleanup
            for (int i = bullets.size() - 1; i >= 0; i--)
            {
                if (!bullets[i]->getIsActive())
                {
                    delete bullets[i];
                    bullets.erase(bullets.begin() + i);
                }
            }

            for (int i = enemies.size() - 1; i >= 0; i--)
            {
                if (!enemies[i]->getIsActive())
                {
                    delete enemies[i];
                    enemies.erase(enemies.begin() + i);
                }
            }
        }

        // Draw
        window.clear(sf::Color::Black);

        if (!gameOver)
        {
            player.draw(window);
            for (auto b : bullets) b->draw(window);
            for (auto e : enemies) e->draw(window);

            if (fontLoaded)
            {
                window.draw(scoreText);
                window.draw(healthText);
            }
        }
        else
        {
            if (fontLoaded)
            {
                window.draw(gameOverText);
                window.draw(finalScoreText);

                sf::Text restartText(font);
                restartText.setCharacterSize(24);
                restartText.setFillColor(sf::Color::Yellow);
                restartText.setPosition({250.0f, 380.0f});
                restartText.setString("Press R to Restart");
                window.draw(restartText);
            }
        }

        window.display();
    }

    for (auto b : bullets) delete b;
    for (auto e : enemies) delete e;

    return 0;
}