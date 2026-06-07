#include "../include/Menu.h"

Menu::Menu(sf::RenderWindow& window)
{
    font.openFromFile("assets/font.ttf");
    selectedIndex = 0;

    std::vector<std::string> items = {"Start Game", "High Scores", "Exit"};

    for (int i = 0; i < (int)items.size(); i++)
    {
        sf::Text text(font, items[i], 36);
        text.setPosition({300.0f, 280.0f + i * 80.0f});

        if (i == 0)
            text.setFillColor(sf::Color::Yellow);
        else
            text.setFillColor(sf::Color::White);

        menuItems.push_back(text);
    }
}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow& window)
{
    sf::Text title(font, "SPACE SHOOTER", 60);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition({150.0f, 100.0f});
    window.draw(title);

    for (auto& item : menuItems)
        window.draw(item);
}

void Menu::moveUp()
{
    menuItems[selectedIndex].setFillColor(sf::Color::White);
    selectedIndex--;
    if (selectedIndex < 0)
        selectedIndex = menuItems.size() - 1;
    menuItems[selectedIndex].setFillColor(sf::Color::Yellow);
}

void Menu::moveDown()
{
    menuItems[selectedIndex].setFillColor(sf::Color::White);
    selectedIndex++;
    if (selectedIndex >= (int)menuItems.size())
        selectedIndex = 0;
    menuItems[selectedIndex].setFillColor(sf::Color::Yellow);
}

int Menu::getSelectedIndex() const
{
    return selectedIndex;
}