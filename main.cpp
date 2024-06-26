#include "Carte.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>

void adjustButtonLayout(std::vector<Button>& buttons, int windowWidth, int windowHeight, float Ratio) {
    int numButtons = buttons.size();
    int numRows = std::sqrt(numButtons);
    int numCols = (numButtons + numRows - 1) / numRows;  // Arrondi à l'entier supérieur

    // Calculer les dimensions de base des cartes
    float buttonHeight = (windowHeight / numRows);
    float buttonWidth = buttonHeight / Ratio;

    // Ajuster les dimensions pour s'assurer qu'elles ne dépassent pas la largeur disponible
    while (buttonWidth > (windowWidth / numCols)) {
        buttonHeight -= 1;
        buttonWidth = buttonHeight / Ratio;
    }

    // Calculer l'espacement restant après la disposition des cartes
    float totalUsedWidth = buttonWidth * numCols;
    float totalUsedHeight = buttonHeight * numRows;

    float horizontalSpacing = (windowWidth - totalUsedWidth) / (numCols + 1);
    float verticalSpacing = (windowHeight - totalUsedHeight) / (numRows + 1);

    // Positionner les cartes avec l'espacement calculé
    for (int i = 0; i < numButtons; ++i) {
        int row = i / numCols;
        int col = i % numCols;

        float x = horizontalSpacing + col * (buttonWidth + horizontalSpacing);
        float y = verticalSpacing + row * (buttonHeight + verticalSpacing);

        buttons[i].setPosition(x + 10, y + 10);
        buttons[i].setSize(buttonWidth - 20, buttonHeight - 20);
    }
}

std::string addNewlinesEverySixChars(const std::string& input)
{
    std::string output;
    int count = 0;

    for (char c : input)
    {
        output += c;
        count++;
        if (count == 6)
        {  // À chaque 6e caractère, ajoutez un saut de ligne
            output += '\n';
            count = 0;  // Réinitialisez le compteur après l'ajout du saut de ligne
        }
    }

    // Vérifiez si un saut de ligne est ajouté en dernier sans caractère après
    if (output.back() == '\n')
    {
        output.pop_back();  // Supprimez le saut de ligne final s'il n'y a rien après
    }

    return output;
}

// Assurez-vous que la classe Button et la fonction adjustButtonLayout sont correctement définies et incluses.

bool isFileEmpty(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return file.peek() == std::ifstream::traits_type::eof();
}

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 800), "Jeu avec fond et boutons", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("textures/font.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    // Current input text
    sf::Text inputText("", font, 20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(20, 50);

    std::string userInput;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("textures/background.png")) {
        std::cerr << "Failed to load background texture!\n";
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    float scaleX = window.getSize().x / backgroundSprite.getLocalBounds().width;
    float scaleY = window.getSize().y / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scaleX, scaleY);

    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("textures/carte.png")) {
        std::cerr << "Failed to load texture!\n";
        return -1;
    }

    // Créer un rectangle sombre pour l'assombrissement de l'écran
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));  // Noir avec transparence

    // Créer le texte "Entrez texte"
    sf::Text pause_text("Ecrivez le contenu\n        de la carte", font, 50);
    pause_text.setFillColor(sf::Color::White);
    pause_text.setPosition(window.getSize().x / 2 - pause_text.getLocalBounds().width / 2,
                     window.getSize().y / 2 - pause_text.getLocalBounds().height / 2 - 200);

    bool allHidden;
    bool click;
    bool addingCard = false;

    std::vector<Button> buttons;

    if(isFileEmpty("data/data.txt"))
        addingCard = true;
    else
    {
        std::ifstream file("data/data.txt");
        std::string line;

        while (getline(file, line))
        {
            buttons.emplace_back(buttonTexture, line, font, 0, 0, 150, 200);
        }
        adjustButtonLayout(buttons, window.getSize().x, window.getSize().y, 1.3);
    }


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                return 0;
            }

            if (event.type == sf::Event::TextEntered)
            {
                if (addingCard)
                {
                    if (event.text.unicode == '\b' && !userInput.empty())
                    { // Handle backspace
                        userInput.pop_back();
                    }
                    else if (event.text.unicode < 128)
                    { // Ignore non-ASCII characters
                        if (userInput.length() < 30)
                        {
                            userInput += static_cast<char>(event.text.unicode);
                        }
                    }
                    inputText.setString(userInput);
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    return 0;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (!addingCard)
                    {
                        addingCard = true; // Start adding a new card
                        userInput = ""; // Clear previous input
                        inputText.setString(userInput); // Clear the display text
                    }
                    else
                    {
                        if (userInput[0] == '!' || userInput[1] == '!')
                        {
                            int i = 0;
                            while(userInput[i] != '!')
                                i++;
                            std::string numberPart = userInput.substr(i + 1);
                            int carte_num = std::atoi(numberPart.c_str());
                            buttons.erase(buttons.begin() + carte_num - 1);
                            adjustButtonLayout(buttons, window.getSize().x, window.getSize().y, 1.3);
                            addingCard = false; // Stop adding a card
                        }
                        // Add the new card with the user input
                        else
                        {
                            userInput = addNewlinesEverySixChars(userInput);
                            buttons.emplace_back(buttonTexture, userInput, font, 0, 0, 150, 200);
                            adjustButtonLayout(buttons, window.getSize().x, window.getSize().y, 1.3);
                            addingCard = false; // Stop adding a card
                        }
                    }
                }
            }

            click = false;
            if (event.type == sf::Event::MouseButtonPressed && !addingCard)
            {
                if (allHidden)
                    click = true;

                for (auto& button : buttons)
                {
                    button.isClicked(event.mouseButton.x, event.mouseButton.y);
                }

                allHidden = true;
                for (auto& button : buttons)
                {
                    if (button.isVisible())
                    {
                        allHidden = false;
                        break;
                    }
                }

            }

            if (allHidden && click && !addingCard)
            {
                for (auto& button : buttons)
                {
                    button.reset();
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        for (auto& button : buttons)
        {
            button.draw(window);
        }
        if (addingCard)
        {
            sf::Text actual_input(addNewlinesEverySixChars(userInput), font, 50);
            actual_input.setFillColor(sf::Color::White);
            actual_input.setPosition(window.getSize().x / 2 - actual_input.getLocalBounds().width / 2,
                     window.getSize().y / 2 - actual_input.getLocalBounds().height / 2);
             // Afficher l'overlay et le texte
            window.draw(overlay);
            window.draw(pause_text);
            window.draw(actual_input);
        }
        window.display();
    }

    return 0;
}




