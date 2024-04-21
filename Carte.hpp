#include <SFML/Graphics.hpp>

class Button {
private:
    sf::Text text;
    sf::Sprite sprite;
    bool visible;

public:
    Button(sf::Texture& texture, const std::string& label, sf::Font& font, float x, float y, float targetWidth, float targetHeight) : visible(true)
    {
        sprite.setTexture(texture);
        float scaleX = targetWidth / sprite.getLocalBounds().width;
        float scaleY = targetHeight / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
        sprite.setPosition(x, y);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(100);
        text.setFillColor(sf::Color::Black);  // Changer selon le besoin
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x + targetWidth / 2.0f, y + targetHeight / 2.0f);
    }

    void draw(sf::RenderWindow& window)
    {
        if (visible)
        {
            window.draw(sprite);
            window.draw(text);
        }
    }

    bool isClicked(float mouseX, float mouseY)
    {
        if (visible && sprite.getGlobalBounds().contains(mouseX, mouseY))
        {
            visible = false;
            return true;
        }
        return false;
    }

    void reset()
    {
        visible = true;
    }

    bool isVisible() const
    {
        return visible;
    }

    void updateTextPosition()
    {
        // Assurez-vous que les dimensions du texte sont recalculées
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

        // Calculez la position centrale basée sur la nouvelle position et taille du sprite
        float x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.0f;
        float y = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.0f;

        text.setPosition(x, y);
    }

    void setPosition(float x, float y)
    {
        sprite.setPosition(x, y);
    // Recalculer la position du texte
        updateTextPosition();
    }

    void setSize(float width, float height)
    {
        float scaleX = width / sprite.getLocalBounds().width;
        float scaleY = height / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
        text.setScale(scaleX, scaleY);
        updateTextPosition();
    }

    std::string getLabel() const
    {
        return text.getString();
    }
};

