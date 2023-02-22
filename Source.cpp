#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Temple Runner");

    sf::Texture backgroundtexture;
    window.setFramerateLimit(60); // fps

    if (!backgroundtexture.loadFromFile("background.png")) {
        cout << "Background load error!\n";
    }
    sf::Sprite backgroundsprite(backgroundtexture);

    // Create player character
    sf::Texture playerTexture;
    playerTexture.loadFromFile("player.png");
    sf::Sprite player(playerTexture);
    player.setPosition(30, 500);

    // Create obstacle
    sf::Texture obstacleTexture;
    obstacleTexture.loadFromFile("obstacle.png");
    sf::Sprite obstacle(obstacleTexture);
    obstacle.setPosition(250, 510);

    // Create obstacle 2
    sf::Texture obstacle2Texture;
    obstacle2Texture.loadFromFile("obstacle.png");
    sf::Sprite obstacle2(obstacle2Texture);
    obstacle2.setPosition(600, 510);

    // Set up gravity variables    
    float gravity = 0.80f;
    bool isJumping = false;
    float jumpVelocity = 0.0f;

    // Game loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up && !isJumping)
                {
                    // Set jump velocity and flag as jumping
                    jumpVelocity = -11.0f;
                    isJumping = true;
                }
            }
        }

        // Update player position based on gravity and jump velocity
        if (isJumping)
        {
            if (player.getPosition().x <= 900) {
                player.move(0.01, jumpVelocity);
                jumpVelocity += gravity;
            }
        }

        // Check if player has landed and reset jump variables
        if (player.getPosition().y + player.getGlobalBounds().height >= window.getSize().y)
        {
            isJumping = false;
            jumpVelocity = 0.0f;
        }

        // Move player left or right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (player.getPosition().x >= -20) {
                player.move(-7, 0);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (player.getPosition().x <= 900) {
                player.move(7, 0);
            }
        }

        // Draw player character and other game objects
        window.clear();
        window.draw(backgroundsprite);
        window.draw(player);
        window.draw(obstacle);
        window.draw(obstacle2);
        window.display();
    }

    return 0;
}
