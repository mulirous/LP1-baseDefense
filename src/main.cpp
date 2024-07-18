#include <SFML/Graphics.hpp>
#include <iostream>

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
        shape(Vector2f(width, height)) {
        shape.setFillColor(Color::Cyan);
        shape.setPosition(posX, posY);
    }

    void setPosition(float x, float y) { // Atualização da posição do personagem
        posX = x;
        posY = y;
        shape.setPosition(posX, posY);
    }

    void goRight(){ // Ir para direita
        if (posX > 0) { // Comparação para não sair da tela
            posX -= spd;
            shape.setPosition(posX, posY);
        }
    }

    void goLeft(float wWindow){ // Ir para esquerda
        if (posX + w < wWindow) { // Comparação para não sair da tela 
            posX += spd;
            shape.setPosition(posX, posY);
        }
    }

    void goUp(){ // Ir para cima
        if (posY > 0) { // Comparação para não sair da tela
            posY -= spd;
            shape.setPosition(posX, posY);
        }
    }

    void goDown(float hWindow){ // Ir para baixo
        if (posY + h< hWindow) { // Comparação para não sair da tela
            posY += spd;
            shape.setPosition(posX, posY);
        }
    }

    void draw(RenderWindow& window) { // Desenhar o corpo do personagem na janela
        window.draw(shape);
    }
};

int main() {
    RenderWindow window(VideoMode(800, 600), "Game Window");
    Character personagem(50.f, 50.f, 8, 100, 100.f, 100.f);

    while (window.isOpen()) {
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
                    cout << "Key pressed: " << event.key.code << endl;
                    if (event.key.code == 0) { // Apertar 'A'
                        personagem.goRight();
                    }
                    if (event.key.code == 3) { // Apertar 'D'
                        personagem.goLeft(800.f);
                    }
                    if (event.key.code == 18) { // Apertar 'S'
                        personagem.goDown(600.f);
                    }
                    if (event.key.code == 22) { // Apertar 'W'
                        personagem.goUp();
                    }
                    break;
                case Event::KeyReleased: // Case para a captura de evento de teclas soltas
                    cout << "Key released: " << event.key.code << endl;
                    break;
            }
        }

        window.clear();
        personagem.draw(window);
        window.display();
    }
    return 0;
}
