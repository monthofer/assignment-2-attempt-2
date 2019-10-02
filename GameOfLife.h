// Sarah Monthofer, Jaewon Park
// 2317035, 2328614
// monthofer@chapman.edu, jaepark@chapman.edu
// CPSC 350
// Assignment 2 - The Game of Life

#include <iostream>
#include <fstream>

using namespace std;

class Game {
  private:
    string outputType;
    string outFileName = "";
    string layoutFile = "";
    int generation;


  public:
    Game();
    ~Game();

    void gameSettings();
    void gameMode(int& boardLength, int& boardWidth, char**& board);
    int classicMode(int boardLength, int boardWidth, char**& board);
    int donutMode(int boardLength, int boardWidth, char**& board);
    int mirrorMode(int boardLength, int boardWidth, char**& board);
    void randomlyGenerateBoard(int& boardLength, int& boardWidth);
    void gameBoardFromFile(string file);
    void printOutputType(string outputType, int generation, char**& board, int boardLength, int boardWidth);
};
