# Base Defense 🏰

### Compilação
    Basta digitar <code>make</code> ou <code>mingw32-make</code> ara compilar o programa.
    Contudo, caso não tenha o <code>make</code> para rodar o MAKEFILE em sua máquina, você pode compilar o programa de outras formas, como:
    <ul>
        <li> Utilizando os atalhos de Build <code>Ctrl + Shift + B</code>.</li>
        <li> Compilando manualmente abrindo o seu teminal dentro do diretório<br>       do projeto e digitando <br>     <code>g++ -g "src/*.cpp" -I"sfml/include" -L"sfml/lib" -lsfml-graphics -lsfml-window -lsfml-system -o "bin/app"</code>.</li>
    </ul>

### Execução
    Apenas abra seu terminal dentro do diretório do projeto e digite <code>./bin/app</code>.

### Requisitos
    <ul>
        <li>Estar em um ambiente Windows.</li>
        <li>Possuir o GCC 13.1.0 ou versão superior.</li>
        <li>De preferência, estar utilizando o Visual Studio Code.</li>
    </ul>
