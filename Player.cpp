#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Player.h"
using namespace sf;
using namespace std;

Player::Player() {
    self = RectangleShape(Vector2f(30, 66));
    self.setOrigin(15, 66);
    self.setFillColor(Color::Green);
    self.setPosition(START_POSITION_X, START_POSITION_Y);
    ySpeed = 0;
    isJumping = false;
    isCrouching = false;
    isAlive = true;
    score = 0;

    font.loadFromFile("D:/OPP/SFML/strikefighter.ttf");
    scoreText.setFont(font);
    scoreText.setPosition(50, 80);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setString("Score: 0");


    gameOverText.setFont(font);
    gameOverText.setPosition(60, 150);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(Color::White);
    gameOverText.setString("Game Over !! \n \"SPACE\" to restart!");
    spritesheet.loadFromFile("D:/OPP/SFML/player.png");
}

void Player::jump(float dt) {
    if (Keyboard::isKeyPressed(Keyboard::Up) && !isJumping) {
        isJumping = true;
        ySpeed = -JUMP_POWER;
    }
    if (isJumping && !isCrouching) {
        ySpeed += FALL_SPEED * dt;
    }
}
void Player::move(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (self.getPosition().x >= -20) {
            self.move(-50, 0);
        }

    }
}

void Player::crouch(float dt) {
    isCrouching = Keyboard::isKeyPressed(Keyboard::Down);

    if (isCrouching) {
        if (isJumping) {
            ySpeed += (FALL_SPEED * 2) * dt;
        } else {
            self.setSize(Vector2f(30, HEIGHT_CROUCHING));
            self.setOrigin(15, HEIGHT_CROUCHING);
        }
    } else {
        self.setSize(Vector2f(30, HEIGHT_STAND));
        self.setOrigin(15, HEIGHT_STAND);
    }
}


void Player::normalise() {
    if (self.getPosition().y > START_POSITION_Y) {
        isJumping = false;
        ySpeed = 0;
        self.setPosition(self.getPosition().x, START_POSITION_Y);
    }
}

void Player::animation() {
    if (isGameOver()) {
        texture.loadFromImage(spritesheet, IntRect(SPRITE_DEAD * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    else if (isJumping) {
        texture.loadFromImage(spritesheet, IntRect(SPRITE_JUMP * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    else {
        if (isCrouching) {
            texture.loadFromImage(spritesheet, IntRect(SPRITE_CROUNCH * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
        } else {
            int animSpeed = 100 - (int)clock.getElapsedTime().asSeconds();
            int frame = animClock.getElapsedTime().asMilliseconds() / (animSpeed < 30 ? 30 : animSpeed);
            int spriteFrame = SPRITE_WALK_START + frame;

            if (spriteFrame > SPRITE_WALK_END) {
                animClock.restart();
                spriteFrame = SPRITE_WALK_START;
            }
            texture.loadFromImage(spritesheet, IntRect(spriteFrame * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
            sprite.setTexture(texture);
            sprite.setScale(Vector2f(4, 4));
            sprite.setOrigin(12.5f, 21);
        }
    }
}

FloatRect Player::getGlobalBounds() {
    return self.getGlobalBounds();
}

void Player::kill() {
    this->isAlive = false;
}

bool Player::isGameOver() {
    return !this->isAlive;
}

void Player::update(RenderTarget &render, float dt) {
    if (isGameOver()) {
        animation();
        return;
    }

    //Score Update 
    if (scoreClock.getElapsedTime().asSeconds() ) {
        score += 10;
    }
    scoreText.setString("Score: "+ to_string(score/10)  );

    crouch(dt);
    jump(dt);
    animation();

    self.move(0, ySpeed * dt);
    normalise();
}

void Player::draw(RenderWindow &window) {
    sprite.setPosition(self.getPosition());
    window.draw(sprite);
    window.draw(scoreText);
    window.draw(direction);
    if (isGameOver())
    {
        window.draw(gameOverText);
    }

}

void Player::restart() {
    self = RectangleShape(Vector2f(50, HEIGHT_STAND));
    self.setOrigin(25, HEIGHT_STAND);
    self.setFillColor(Color::Green);
    self.setPosition(START_POSITION_X, START_POSITION_Y);
    ySpeed = 0;
    isJumping = false;
    isCrouching = false;
    isAlive = true;
    score = 0;
    clock.restart();
    animClock.restart();
}
