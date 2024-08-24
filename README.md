# Spell Guard 🏰

### Compilação
Basta digitar `make` ou `mingw32-make` para compilar o programa.
Contudo, caso não tenha o `make` para rodar o MAKEFILE em sua máquina, você pode compilar o programa de outras formas, como:
- Utilizando os atalhos de Build `Ctrl + Shift + B`, no isual Studio Code.
- Compilando manualmente abrindo o seu terminal dentro do diretório do projeto e digitando:
                  `g++ -g "src/*.cpp" -I"sfml/include" -L"sfml/lib" -lsfml-graphics -lsfml-window -lsfml-system -o "bin/app"`

### Execução
Apenas abra seu terminal dentro do diretório do projeto e digite `./bin/app`.

### Requisitos
- Ambiente Windows.
- Possuir o GCC 13.1.0 ou versão superior.
- De preferência, estar utilizando o Visual Studio Code.

### Sobre o Jogo
Spell Guard é um jogo de defesa de base em que inimigos surgem no mapa com o objetivo de destruir a base. Seu objetivo é proteger a base durante um tempo determinado.

#### Recursos do Herói
- Vida: 100 pontos fixos.
- Mana: 50 pontos, usados para lançar projéteis.
- Movimento: O herói se move em direção ao ponto clicado com o botão direito do mouse. Se o botão direito for mantido pressionado, o herói segue o cursor do mouse.
- Sprint: Ao clicar o botão esquerdo do mouse, o herói realiza um sprint.
- Área de Ação: O herói pode se mover livremente dentro e fora da base.
- Ataque: O herói pode disparar projéteis pressionando a tecla Q, com alcance limitado, na direção do cursor do mouse.
#### Inimigos
- Spawn: Surgem aleatoriamente nas bordas do mapa em intervalos fixos de tempo, que podem diminuir conforme a dificuldade aumenta.
- Movimento: Os inimigos se movem em direção ao centro da base ou ao herói.
- Ataque: Disparam projéteis em direção ao herói.
- Interações: Os projéteis podem colidir com o herói, com outros inimigos, ou com a base.
- Drops: Ao serem derrotados, os inimigos deixam itens (mana) que podem ser coletados pelo herói. Esses itens desaparecem após certo tempo se não forem coletados.
#### Base
- Vida: A base tem sua vida determinada pela dificuldade do jogo.
- Regeneração: A base regenera sua vida ao longo do tempo.
- Condição de Derrota: O jogo termina se a base for destruída ou se o herói morrer.
- Condição de Vitória: O jogo termina se a base for protegida durante o tempo determinado.
#### Dificuldades
Antes de iniciar o jogo, o usuário é direcionado para a escolha de dificuldade do jogo

##### Fácil:
- Vida da Base: 1000 pontos
- Regeneração: 10 segundos
- Intervalo de Spawn: 5 segundos
- Duração do Jogo: 91 segundos
- Velocidade do Inimigo: 30 unidades
- Vida do Inimigo: 50 pontos
- Dano do Inimigo: 5 pontos

##### Médio:
- Vida da Base: 800 pontos
- Regeneração: 15 segundos
- Intervalo de Spawn: 3 segundos
- Duração do Jogo: 121 segundos
- Velocidade do Inimigo: 40 unidades
- Vida do Inimigo: 70 pontos
- Dano do Inimigo: 10 pontos

##### Difícil:
- Vida da Base: 600 pontos
- Regeneração: 20 segundos
- Intervalo de Spawn: 3 segundos
- Duração do Jogo: 181 segundos
- Velocidade do Inimigo: 40 unidades
- Vida do Inimigo: 90 pontos
- Dano do Inimigo: 15 pontos

#### Como Jogar
###### Menu: Ao iniciar o jogo, você será levado ao menu principal, onde pode selecionar a dificuldade e iniciar o jogo.
###### Movimentação: Use o botão direito do mouse para definir o destino do herói. O botão esquerdo aciona o sprint.
###### Ataque: Pressione Q para disparar projéteis em direção ao cursor do mouse.
###### Proteger a Base: Derrote inimigos e colete mana para se fortalecer.
###### Sobrevivência: Proteja a base pelo tempo determinado ou até que todos os inimigos sejam derrotados.

### Estrutura do Código
O código está organizado em classes e módulos, incluindo:

Game: Controla o fluxo principal do jogo, como o estado atual, inicialização de objetos, renderização e lógica de jogo.
Hero: Representa o herói controlado pelo jogador.
Enemy: Representa os inimigos que surgem no mapa.
Base: Representa a base que deve ser protegida.
Drop: Itens que podem ser coletados pelo herói.

### Criação de Objetos e Lógica de Jogo
Inicialização: Cada objeto é inicializado com atributos específicos, como vida, velocidade e posição.
Atualização: Durante o jogo, todos os objetos são atualizados em tempo real, incluindo movimento, ataques e colisões.
Colisões: A detecção de colisões é gerenciada para interações entre projéteis, herói, inimigos e base.

### Considerações Finais
Aproveite o jogo e defenda sua base com estratégia e habilidade! Se tiver sugestões ou encontrar algum bug, sinta-se à vontade para contribuir no repositório.