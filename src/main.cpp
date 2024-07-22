#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

class Character {
    private:
    float w; // Largura
    float h; // Altura
    float spd; // Velocidade
    int life; // Vida
    RectangleShape shape;
    
    public:
    float posX; // Posição do personagem
    float posY; // Posição do personagem

    Character(float width, float height, float speed, int vida, float X, float Y) // Construtor
        : w(width),
        h(height),
        spd(speed),
        life(vida),
        shape(Vector2f(width, height)),
        posX(X),
        posY(Y) {
        shape.setFillColor(Color::Cyan);
        shape.setPosition(X, Y);
    }

    void setPosition(float x, float y) { // Atualização da posição do personagem
        posX = x;
        posY = y;
        shape.setPosition(posX, posY);
    }

    void updateMovement(float wWindow, float hWindow) {
        if (Keyboard::isKeyPressed(Keyboard::A) && posX > 0) {
            posX -= spd;
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && posX + w < wWindow) {
            posX += spd;
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && posY > 0) {
            posY -= spd;
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && posY + h < hWindow) {
            posY += spd;
        }
        shape.setPosition(posX, posY);  
    }


    void draw(RenderWindow& window) { // Desenhar o corpo do personagem na janela
        window.draw(shape);
    }
};

class Enemy {
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
        centerX(cX/2.0),
        centerY(cY/2.0){
        shape.setFillColor(Color::Red);
        shape.setPosition(X, Y);
    }

    void update(float deltaTime) {
        float directionX = centerX - posX;
        float directionY = centerY - posY;
        float magnitude = sqrt(directionX * directionX + directionY * directionY);

        if (magnitude > 0) {
            posX += (directionX / magnitude) * spd * deltaTime;
            posY += (directionY / magnitude) * spd * deltaTime;
            shape.setPosition(posX, posY);
        }
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    RenderWindow window(VideoMode(1200, 800), "Game Window");
    Character personagem(50.f, 50.f, 2, 100, 600.f, 400.f);
    Enemy inimigo(20.f, 10, 80, -50.f, -50.f, 600.f, 400.f);

    Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        Event event; // Inicialização da variável que captura eventos
        while (window.pollEvent(event)) {

            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;             
                case Event::KeyPressed: // Case para a captura de evento de teclas pressionadas
                    if (event.key.code == Keyboard::Escape) {  // Fechar a aplicação em caso de apertar 'Esc'
                        window.close();
                    }
                    
                    personagem.updateMovement(1200.f, 800.f);

                    break;
                case Event::KeyReleased: // Case para a captura de evento de teclas soltas
                    cout << "Key released: " << event.key.code << endl;
                    break;

                default:
                    break;
            }
        }

        inimigo.update(deltaTime);

        window.clear();
        personagem.draw(window);
        inimigo.draw(window);
        window.display();
    }
    return 0;
}
