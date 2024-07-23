#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <list>
#include "domain/interfaces/Hero.hpp"

using namespace sf;
using namespace std;

class Enemy
{
private:
    float w;
    float spd;
    int life;
    CircleShape shape;

public:
    float posX;
    float posY;
    float centerX;
    float centerY;

    Enemy(float width, float speed, int vida, float X, float Y, float cX, float cY)
        : w(width),
          spd(speed),
          life(vida),
          shape(width),
          posX(X),
          posY(Y),
          centerX(cX / 2.0),
          centerY(cY / 2.0)
    {
        shape.setFillColor(Color::Red);
        shape.setPosition(X, Y);
    }

    void update(float deltaTime)
    {
        float directionX = centerX - posX;
        float directionY = centerY - posY;
        float magnitude = sqrt(directionX * directionX + directionY * directionY);

        if (magnitude > 0.5f)
        {
            posX += (directionX / magnitude) * spd * deltaTime;
            posY += (directionY / magnitude) * spd * deltaTime;
            shape.setPosition(posX, posY);
        }
    }

    void draw(RenderWindow &window)
    {
        window.draw(shape);
    }
};

Enemy spawnEnemy(float centerX, float centerY)
{
    float spawnX;
    float spawnY;

    int side = rand() % 4;

    switch (side)
    {
    case 0: // Topo
        spawnX = rand() % 1201;
        spawnY = -50.f;
        break;

    case 1: // Direita
        spawnX = 1250;
        spawnY = rand() % 801;
        break;

    case 2: // Baixo
        spawnX = rand() % 1201;
        spawnY = 850;
        break;

    case 3: // Esquerda
        spawnX = -50;
        spawnY = rand() % 801;
    }

    return Enemy(20.f, 10, 80, spawnX, spawnY, centerX, centerY);
}

int main()
{
    RenderWindow window(VideoMode(1200, 800), "Game Window");

    Hero character(50.f, 50.f, 2, 100, 600.f, 400.f);

    list<Enemy> enemies;
    float centerX = 1050.f;
    float centerY = 700.f;

    Clock clock;
    float spawnTimer = 0.f;
    float spawnInterval = 2.f; // Intervalo de spawn em segundos

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        spawnTimer += deltaTime;

        Event event; // Inicialização da variável que captura eventos
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed: // Case para a captura de evento de teclas pressionadas
                if (event.key.code == Keyboard::Escape)
                { // Fechar a aplicação em caso de apertar 'Esc'
                    window.close();
                }

                break;
            case Event::KeyReleased: // Case para a captura de evento de teclas soltas
                cout << "Key released: " << event.key.code << endl;
                break;

            default:
                break;
            }
        }

        character.move(1200.f, 800.f);

        if (spawnTimer >= spawnInterval)
        { // Spawnar novos inimigos
            enemies.push_back(spawnEnemy(centerX, centerY));
            spawnTimer = 0.f;
        }

        for (auto it = enemies.begin(); it != enemies.end();)
        { // Atualizar inimigos
            it->update(deltaTime);
            if (abs(it->posX - centerX) < 5.f && abs(it->posY - centerY) < 5.f)
            { // Condição de remoção do inimigo (exemplo: inimigo alcançou o centro)
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }

        window.clear(Color::White);
        window.draw(character.getShape());
        for (auto &enemy : enemies)
        {
            enemy.draw(window);
        }

        window.display();
    }
    return 0;
}
