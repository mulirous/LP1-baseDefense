# Base Defense 🏰

### Compilação
Basta digitar `make` ou `mingw32-make` para compilar o programa.
Contudo, caso não tenha o `make` para rodar o MAKEFILE em sua máquina, você pode compilar o programa de outras formas, como:
- Utilizando os atalhos de Build `Ctrl + Shift + B`.
- Compilando manualmente abrindo o seu terminal dentro do diretório do projeto e digitando:
                  `g++ -g "src/*.cpp" -I"sfml/include" -L"sfml/lib" -lsfml-graphics -lsfml-window -lsfml-system -o "bin/app"`

### Execução
Apenas abra seu terminal dentro do diretório do projeto e digite `./bin/app`.

### Requisitos
- Estar em um ambiente Windows.
- Possuir o GCC 13.1.0 ou versão superior.
- De preferência, estar utilizando o Visual Studio Code.
