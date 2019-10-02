// Sarah Monthofer, Jaewon Park
// 2317035, 2328614
// monthofer@chapman.edu, jaepark@chapman.edu
// CPSC 350
// Assignment 2 - The Game of Life

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include "GameOfLife.h"

using namespace std;

  Game::Game() {
    string outputType;
    string outFileName = "";
    string layoutFile = "";
    int generation;
  }

  Game::~Game() {

  }

  // Ask the user if they would like to pause, enter, or print to file w/ data checking
  // Ask the user if they would like to randomly generate the matrix or read from file for layout
  void Game::gameSettings() {
    cout << "Welcome to the Game of Life" << endl;
    cout << endl;

    string userOutputGen = "";
    string outputType = "";
    string outFileName = "";
    bool pause = false;
    bool enter = false;
    bool dataCheck = true;
    bool dataCheck2 = true;

    // prompt the user if they would like to pause between cell generations,
    // click enter to continue, or if they would like to print everything to a file
    while (dataCheck == true) {
      cout << "How would you like to View results of the program? " << endl;
      cout << "PAUSE - pause between generations" << endl;
      cout << "ENTER - press enter to move to next generation" << endl;
      cout << "FILE - output results into a file" << endl;
      cin >> outputType;
      // https://stackoverflow.com/questions/23418390/how-to-convert-a-c-string-to-uppercase
      transform(outputType.begin(), outputType.end(), outputType.begin(), ::toupper);
      cout << endl;

      if (outputType == "PAUSE") {
        pause = true;
        dataCheck = false;
      }

      else if (outputType == "ENTER") {
        enter = true;
        dataCheck = false;
      }

      else if (outputType == "FILE") {
        cout << "Enter the filename.txt you would like to print to" << endl;
        cin >> outFileName;
        dataCheck = false;
      }

      else {
        cout << endl;
        cout << "Incorrect Data Option - enter FILE, ENTER, or PAUSE" << endl;
        cout << endl;
        dataCheck = true;
      }
    }

    // prompt the user if they want to use a file to populate the board, or randomly generate the board
    while (dataCheck2 == true) {
      string outFileName = "";
      string line = "";

      cout << endl << "How would you like to generate the matrix?" << endl;
      cout << "RANDOM - Random Generation" << endl;
      cout << "FILE - Create Generation based on file" << endl;
      cin >> userOutputGen;
      transform(userOutputGen.begin(), userOutputGen.end(), userOutputGen.begin(), ::toupper);
      cout << endl;

      if (userOutputGen == "RANDOM") {
        int boardLength = 0;
        int boardWidth = 0;
        dataCheck2 = false;
        randomlyGenerateBoard(boardLength, boardWidth);
      }

      else if (userOutputGen == "FILE") {
        string layoutFile = "";
        cout << "Enter the filename.txt with board layout" << endl;
        cin >> layoutFile;

        // check if the file the user input exists
        // http://www.cplusplus.com/forum/beginner/8388/
        ifstream myfile;
        myfile.open(layoutFile);
        if (myfile.is_open()) {
          // while (!myfile.eof()) {
          //   getline(myfile, line);
          //   if (line.length() == 0) {
          //     cout << "ERROR - The file you entered is empty" << endl;
          //     dataCheck2 = false;
          //   }
            //
            // else {
              cout << "Reading from file " << layoutFile << endl;
              cout << endl;
              dataCheck2 = false;
              gameBoardFromFile(layoutFile);
          //   }
          // }
        }
        myfile.close();
      }

      else {
        cout << "Incorrect Data Option - enter RANDOM or FILE" << endl;
        dataCheck = true;
      }
    }
  }

  void Game::printOutputType(string outputType, int generation, char**& board, int boardLength, int boardWidth) {
    int sleepTime = 0;
    bool buttonClicked = false;

    // if the user chooses pause generate breaks between generations
    if (outputType == "PAUSE") {
      // choose to sleep for a number between 1 and 10 seconds
      sleepTime = rand() % 11;
      sleep(sleepTime);

      cout << endl;
      cout << "Current Generation: " << generation << endl;

      // print out the board after sleep time is done
      for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
          cout << board[i][j];
        }
        cout << endl;
      }
    }

    // if the user chooses enter then print the board when they click a button on keyboard
    else if (outputType == "ENTER") {
      cout << "Click the spacebar whenever you are ready to continue" << endl;
      cin.get();

      if (cin.get()) {
        buttonClicked = true;
      }

      if (buttonClicked = true) {
        cout << "Current Generation: " << generation << endl;

        // print out the board when the spacebar or any button is clicked
        for (int i = 0; i < boardLength; i++) {
          for (int j = 0; j < boardWidth; j++) {
            cout << board[i][j];
          }
          cout << endl;
        }
      }
    }
  }

  // // choose the mode of the game (either classic, donut or mirror) with error checking
  void Game::gameMode(int& boardLength, int& boardWidth, char**& board) {
    string mode = "";
    bool dataCheck3 = false;

    while (dataCheck3 == false) {
      cout << "Select which game mode you would like to play in" << endl;
      cout << "CLASSIC - Classic Mode" << endl;
      cout << "DONUT - Donut Mode" << endl;
      cout << "MIRROR - Mirror Mode" << endl;
      cin >> mode;
      transform(mode.begin(), mode.end(), mode.begin(), ::toupper);
      cout << endl;

      if (mode == "CLASSIC") {
        dataCheck3 = true;
        classicMode(boardLength, boardWidth, board);
      }

      else if (mode == "DONUT") {
        dataCheck3 = true;
        donutMode(boardLength, boardWidth, board);
      }

      else if (mode == "MIRROR") {
        dataCheck3 = true;
        // mirrorMode(boardLength, boardWidth, board);
      }

      else {
        cout << "Incorrect Data Option - enter DONUT, CLASSIC, or MIRROR" << endl;
        dataCheck3 = false;
      }
    }
  }

  // classic mode runs where all spaces on the end of the board are consciderd empty
  int Game::classicMode(int boardLength, int boardWidth, char**& board) {
    int generation = 0;
    int neighbors = 0;
    bool stable = false;
    int timesChecked = 0;
    string printBoard = "";

    // create a pointer to a pointer to a character of each element in the board array's
    // set these pointers to point to the next Generation of cells
    char** nextGen = new char*[boardLength];
    for (int i = 0; i < boardLength; i++) {
      nextGen[i] = new char[boardWidth];
    }

    do {
      // itterate through every line then every cell on the board
      // count the occurance of a cell next to the 8 surrounding cells
      for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
          neighbors = 0;

          if (board[j-1][i] == 'X') {
            neighbors++;
          }

          if (board[j-1][i-1] == 'X') {
            neighbors++;
          }

          if (board[j][i-1] == 'X') {
            neighbors++;
          }

          if (board[j-1][i+1] == 'X') {
            neighbors++;
          }

          if (board[j][i+1] == 'X') {
            neighbors++;
          }

          if (board[j+1][i+1] == 'X') {
            neighbors++;
          }

          if (board[j+1][i] == 'X') {
            neighbors++;
          }

          if (board[j+1][i-1] == 'X') {
            neighbors++;
          }

          // 1 neighbor or fewer - cell dies in the next generation
          if (neighbors >= 0 && neighbors <= 1) {
            nextGen[i][j] = '-';
          }

          // 2 neighbors - cell stays the same in the next generation
          if (neighbors == 2) {
            nextGen[i][j] = board[i][j];
          }

          // 3 neighbors - put a new cell next generation if the cell exists in current sqaure or not
          if (neighbors == 3) {
            nextGen[i][j] = 'X';
          }

          // 4 neighbors - cell dies in the next generation
          if (neighbors >= 4) {
            nextGen[i][j] = '-';
          }
        }
      }

      // check if the board becomes stable - meaning the squares stay the same after a few(3) generations
      do {
        for (int i = 0; i < boardLength; i++) {
          for (int j = 0; j < boardWidth; j++) {
            if (board[i][j] = nextGen[i][j]) {
              stable = true;
            }
            else {
              stable = false;
            }
          }
        }
        timesChecked++;
      } while (timesChecked < 2);

      if (stable == true) {
        cout << "Generations have Stabilized" << endl;
        cout << endl;
      }

      // if generations are not stable transfer everything into sting to print to file or terminal
      // if not stable transfer current generation into next generation
      if (stable == false) {
        cout << "Current Generation: " << generation << endl;

        // put the current generation board into a string
        if (generation == 0) {
          printBoard += "0";
          printBoard += '\n';
          for (int i = 0; i < boardLength; i++) {
            for (int j = 0; j < boardWidth; j++) {
              printBoard += board[i][j];
            }
            if (i == boardLength) {
              printBoard += '\n';
            }
          }

          printOutputType(outputType, generation, board, boardLength, boardWidth);

          // trasnfer current generation into the next generation
          for (int i = 0; i < boardLength; i++) {
            for (int j = 0; j < boardWidth; j++) {
              board[i][j] = nextGen[i][j];
            }
          }
        }

        // if generation 2 or above put the current generation board into a string
        else {
          printBoard += generation;
          printBoard += '\n';
          for (int i = 0; i < boardLength; i++) {
            for (int j = 0; j < boardWidth; j++) {
              printBoard += board[i][j];
            }
            if (i == boardLength) {
              printBoard += '\n';
            }
          }

          printOutputType(outputType, generation, board, boardLength, boardWidth);
        }
      }
      generation++;
    } while (stable == false);

    // if the user entered print to file you print results to the file
    // https://www.bgsu.edu/arts-and-sciences/computer-science/cs-documentation/writing-data-to-files-using-c-plus-plus.html
    if (outFileName != "") {
      ofstream outdata;
      outdata.open(outFileName);
      if (!outdata) {
        cout << "ERROR - file cannot be opened" << endl;
      }
      outdata << printBoard << endl;
      cout << endl;
      outdata.close();
    }
  }

  int Game::donutMode(int boardLength, int boardWidth, char**& board) {
    int neighbors = 0;
    int r = 0;
    int c = 0;

    //DOUGHNUT MODE

    if (r == 0)
    {
     if (c == 0) {
      if (board[1][1] == 'X')
       neighbors++;

      if (board[0][1] == 'X')
       neighbors++;

      if (board[1][0] == 'X')
       neighbors++;

      if (board[1][boardWidth - 1] == 'X')
       neighbors++;

      if (board[0][boardWidth - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][boardWidth - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][0] == 'X')
       neighbors++;

      if (board[boardLength - 1][1] == 'X')
       neighbors++;
     }

     if (c == boardWidth - 1) {
      if (board[1][boardWidth - 1] == 'X')
        neighbors++;

      if (board[1][boardWidth - 2] == 'X')
       neighbors++;

      if (board[0][boardWidth - 2] == 'X')
       neighbors++;

      if (board[boardLength - 1][boardWidth - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][boardWidth - 2] == 'X')
       neighbors++;

      if (board[boardLength - 1][0] == 'X')
       neighbors++;

      if (board[1][0] == 'X')
       neighbors++;

      if (board[0][0] == 'X')
       neighbors++;
     }

     else
     {
      if (board[0][c + 1] == 'X')
       neighbors++;

      if (board[0][c - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][c + 1] == 'X')
        neighbors++;

      if (board[boardLength - 1][c - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][c] == 'X')
       neighbors++;
     }
    }

    if (r == boardLength - 1)
    {
     if (c == 0)
     {
      if (board[boardLength - 2][0] == 'X')
       neighbors++;

      if (board[boardLength - 2][1] == 'X')
       neighbors++;

      if (board[boardLength - 1][1] == 'X')
       neighbors++;

      if (board[boardLength - 2][boardWidth - 1] == 'X')
       neighbors++;

      if (board[boardLength - 1][boardWidth - 1] == 'X')
       neighbors++;

      if (board[0][boardWidth - 1] == 'X')
       neighbors++;

      if (board[0][0] == 'X')
       neighbors++;

      if (board[0][1] == 'X')
       neighbors++;
     }

     if (c == boardWidth - 1)
     {
      if (board[boardLength - 2][boardWidth - 1] == 'X')
       neighbors++;

      if (board[boardLength - 2][boardWidth - 2] == 'X')
       neighbors++;

      if (board[boardLength - 1][boardWidth - 2] == 'X')
       neighbors++;

      if (board[0][boardWidth - 2] == 'X')
       neighbors++;

      if (board[0][boardWidth - 1] == 'X')
       neighbors++;

      if (board[0][0] == 'X')
       neighbors++;

      if (board[boardLength - 2][0] == 'X')
       neighbors++;

      if (board[boardLength - 1][0] == 'X')
       neighbors++;
     }

     else
     {
      if (board[boardLength - 1][c + 1] == 'X')
       neighbors++;

      if (board[boardLength = 1][c - 1] == 'X')
       neighbors++;

      if (board[0][c] == 'X')
       neighbors++;

      if (board[0][c - 1] == 'X')
       neighbors++;

      if (board[0][c + 1] == 'X')
       neighbors++;
     }
    }

    if ((c == 0) && (r > 0) && (r < boardLength - 1))
    {
     if (board[r - 1][0] == 'X')
      neighbors++;

     if (board[r + 1][0] == 'X')
      neighbors++;

     if (board[r][0] == 'X')
      neighbors++;

     if (board[r][boardWidth - 1] == 'X')
      neighbors++;

     if (board[r + 1][boardWidth - 1] == 'X')
      neighbors++;

     if (board[r - 1][boardWidth - 1] == 'X')
      neighbors++;
    }

    if ((c == boardWidth - 1) && (r > 0) && (r < boardLength - 1))
    {
     if (board[r - 1][boardWidth - 1] == 'X')
      neighbors++;

     if (board[r + 1][boardWidth - 1] == 'X')
      neighbors++;

     if (board[r][boardWidth - 1] == 'X')
      neighbors += 0;

     if (board[r - 1][0] == 'X')
      neighbors++;

     if (board[r + 1][0] == 'X')
      neighbors++;

     if (board[r][0] == 'X')
      neighbors++;
    }

    // cout << "Total Neighbors" << endl;

    // then we should have printed according to the user requests - pause, enter or to file
   }

   int Game::mirrorMode(int boardLength, int boardWidth, char**& board)
   {
     int neighbors = 0;
     int r = 0;
     int c = 0;

     if (r == 0)
     {
       if (c == 0)    //top left
       {
         if (board[1][1] == 'X')
           neighbors += 1;

         if (board[0][1] == 'X')
           neighbors += 2;

         if (board[1][0] == 'X')
           neighbors += 2;

         if (board[0][0] == 'X')
           neighbors +=  3;
       }

       if (c == boardWidth - 1) //top right
       {
         if (board[1][boardWidth - 2] == 'X')
           neighbors += 1;

         if (board[0][boardWidth - 2] == 'X')
           neighbors += 2;

         if (board[1][boardWidth - 1] == 'X')
           neighbors += 2;

         if (board[0][boardWidth - 1] == 'X')
           neighbors += 3;
       }

       else     //top
       {
         if (board[0][c] == 'X')
           neighbors++;

         if (board[0][c + 1] == 'X')
           neighbors += 2;

         if (board[0][c - 1] == 'X')
           neighbors += 2;
        }
     }

     if (r == boardLength - 1)
     {
       if (c == 0)    //bottom left
       {
         if (board[boardLength - 2][1] == 'X')
           neighbors += 1;

         if (board[boardLength - 1][1] == 'X')
           neighbors += 2;

         if (board[boardLength - 2][0] == 'X')
           neighbors += 2;

         if (board[boardLength - 1][0] == 'X')
           neighbors += 3;
       }

       if (c == boardWidth - 1) //bottom right row
       {
         if (board[boardLength - 2][boardWidth - 2] == 'X')
           neighbors += 1;

         if (board[boardLength - 1][boardWidth - 2] == 'X')
           neighbors += 2;

         if (board[boardLength - 2][boardWidth - 1] == 'X')
           neighbors += 2;

         if (board[boardLength - 1][boardWidth - 1] == 'X')
           neighbors += 3;
       }

       else
       {
         if (board[boardLength - 1][c] == 'X')
           neighbors++;

         if (board[boardLength - 1][c + 1] == 'X')
           neighbors += 2;

         if (board[boardLength - 1][c - 1] == 'X')
           neighbors += 2;
       }

     if ((c == 0) && (r > 0) && (r < boardLength - 1))  //LEFT SIDE
     {
       if (board[r - 1][0] == 'X')
         neighbors += 2;

       if (board[r + 1][0] == 'X')
         neighbors += 2;

       if (board[r][0] == 'X')
         neighbors++;
      }

     if ((c == boardWidth - 1) && (r > 0) && (r < boardLength - 1))
     {
       if (board[r - 1][boardWidth - 1] == 'X')
         neighbors += 2;

       if (board[r + 1][boardWidth - 1] == 'X')
         neighbors +=2;

       if (board[r][boardWidth - 1] == 'X')
         neighbors++;
      }
    }
  }

  // this part has errors
  void Game::gameBoardFromFile(string file) {
    int boardLength = 0;
    int boardWidth = 0;
    int curLine = 0;
    string thisLine = "";
    string line = "";

    // uses input file from
    // http://www.cplusplus.com/doc/tutorial/files/
    // ifstream myfile;
    // myfile.open(layoutFile);
    // while (!myfile.eof()) {
    //   line = getline(myfile, thisLine);
    ifstream myfile(layoutFile);
    if (myfile.is_open()) {
      while(getline(myfile, line)) {
        thisLine = line;
        cout << "thisLine" << endl;
        if (curLine == 0) {
          // https://fresh2refresh.com/c-programming/c-type-casting/c-atoi-function/
          // atoi turns the string of line length into an integer - std::stoi
          boardLength = std::stoi(thisLine);
          cout << boardLength;
        }

        else if (curLine == 1) {
          boardWidth = std::stoi(thisLine);
          cout << boardWidth;
        }

        else {
          break;
        }
        curLine++;
      }
      myfile.close();
    }


    // reset everything for the next time your read from the file
    curLine = 0;
    myfile.clear();
    myfile.seekg(0, std::ios::beg);

    // char** is a pointer to a pointer to a character in the board
    // char* is a pointer to the character boardLength
    char** transferBoard = new char*[boardLength];

    // create a matrix where for each length of the row there is a column made
    // with the designated column length
    for (int i = 0; i < boardLength; ++i) {
      transferBoard[i] = new char[boardWidth];
    }

    // set every item the arrays columns and rows to -
    for (int i = 0; i < boardLength; i++) { // row
      for (int j = 0; j < boardWidth; j++) {
        transferBoard[i][j] = '-';
        cout << transferBoard[i][j] << endl;
      }
    }

    int row = 0;
    myfile.clear();
    myfile.seekg(0, std::ios::beg);

    if (myfile.is_open()) {
      while(getline(myfile, line)) {
        thisLine = line;
        if (curLine = 0) {
          // do nothing to this line
        }

        else if (curLine = 1) {
          // do nothing to this line
        }

        else {
          for (int i = 0; i < boardWidth; i++) {
            char element = line[i];
            transferBoard[row][i] = element;
          }
          row++;
        }
        curLine++;
      }
    }
    gameMode(boardLength, boardWidth, transferBoard);
  }


  // randomly generate the board density and location of population
  void Game::randomlyGenerateBoard(int& boardLength, int& boardWidth) {
    // declare variables
    double ranDensity = 0.0;
    int boardSize = 0;
    int populationDensity = 0;
    int randXCord = 0;
    int randYCord = 0;
    bool notX = true;

    cout << "Length of Board: \t" << endl;
    cin >> boardLength;

    cout << "Width of Board: \t" << endl;
    cin >> boardWidth;

    // prompt the user for a population density from 1 to 0
    do {
      cout << "Choose a random number between 0 and 1: \t" << endl;
      cin >> ranDensity;
    } while ((ranDensity <= 0.0) || (ranDensity >= 1.0));

    // create a pointer from the current board to the next generations board
    char** transferBoard = new char*[boardLength];
    for (int i = 0; i < boardLength; ++i) {
      transferBoard[i] = new char[boardWidth];
      cout << transferBoard[i];
    }

    // deafult, put - on all indeces of the matrix
    for (int i = 0; i < boardLength; i++) { // row
      for (int j = 0; j < boardWidth; j++) { // column
        transferBoard[i][j] = '-';
      }
    }

    // calcualte the amount of sqaures that will be populated based on density and size of the board
    boardSize = boardLength * boardWidth;
    populationDensity = (boardSize * ranDensity);

    // claculate where to place the X's according to the populationDensity
    for (int i = 0; i < populationDensity; i++) {
      // determine the random coridnates
      // http://www.cplusplus.com/reference/cstdlib/rand/
      randXCord = rand() % (boardLength);
      randYCord = rand() % (boardWidth);

      // place X according to random population
      while (notX = true) {
        if (transferBoard[randXCord][randYCord] = '-') {
          // place an X on the cordinate if its not already occupied
          transferBoard[randXCord][randYCord] = 'X';
          notX = false;
        }
        else {
          // generate another cordinate if the cordinate is already occupied by an X
          randXCord = rand() % (boardLength);
          randYCord = rand() % (boardWidth);
          notX = true;
        }
      }
    }
    gameMode(boardLength, boardWidth, transferBoard);
  }


// german class notes on declaring matrix
// char **myGrid;
// myGrid = new char*[row]
// for (        )
//     myGrid[i][j] = '-';
// create an array of column sizes for each row which is the actual array
