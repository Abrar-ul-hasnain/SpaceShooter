#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
{
private:
    sf::Font font;
    int selectedIndex;
    std::vector<sf::Text> menuItems;

public:
    Menu(sf::RenderWindow& window);
    ~Menu();

    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
};

#endif