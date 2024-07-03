# Introduction

The Snaze game is a C++ implementation of the classic Snake arcade game with a twist: the snake is trapped inside a maze! This program simulates the game by loading maze levels from an input text file, provided on the configuration file, and controlling the snake's movements. The main challenge is to design a basic artificial intelligence (AI) engine that guides the snake to food pellets that pop out at random places inside the maze without running into the maze’s walls or itself.

# Author(s)

- Nome: Bianca Maciel Medeiros e Marina Medeiros Araujo Leite
- Email: *<bianca.medeiros.701@ufrn.edu.br e marinamaleite@gmail.com>*
- Turma: DIM0176

# Grading Criteria

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                                                         | Valor máximo | Valor esperado |
| ---------------------------------------------------------------------------- | :----------: | :------------: |
| Lê e valida corretamente um arquivo de entrada de nível                      |      20      |       20       |
| Sugere direções aleatórias para a cobra seguir                               |      10      |       10       |
| Determina corretamente um conjunto de direções para a cobra ir até a comida  |      50      |        0       |
| Exibe corretamente os vários estados do jogo                                 |      30      |       30       |


# Compiling and Running

To compile and run the game, follow these steps:

1. Ensure you have CMake installed on your system.
2. Clone the repository to your local machine.
3. Navigate to the project directory in your terminal.
5. Create a build directory: `mkdir build && cd build`.
6. Generate the build files with CMake: `cmake ..`.
7. Compile the project: `cmake --build .`.
8. Run the compiled executable: `./snaze` followed by the path of the config file (e.g. `./snaze ../config/snaze.ini`).
