#ifndef _PLAYER_
#define _PLAYER_
#include "constants.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

bool valid_move(char board_t[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player);
void add_move(char board_f[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player);
void get_player_move(char board_f[ROWS][COLS],char board_t[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player);
bool isPiece(char board_t[ROWS][COLS],int pieceX,int pieceY,char player);
void get_possible_moves(char board_f[ROWS][COLS],char board_t[ROWS][COLS], int pieceX, int pieceY, char player);
void countbw(char board_t[ROWS][COLS], int pieceX, int pieceY, char player, int& vertMove, int& horMove, int& odMove, int& diagMove);
void print_possible_moves(char board_t[ROWS][COLS]);
void loadPosition(Sprite f[24], int size, int offset);
void switchTurn(char& Player);
void guiCapture(Vector2f newPos,Sprite f[24], char currentPlayer);
void guiDraw();
void guiWinner(char currentPlayer);
void menu();
void resetBoard(char board_t[8][8], char board_p[8][8]);
void play_game();
#endif
