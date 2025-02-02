#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#ifndef PIPE_H
#define PIPE_H
void connect();
struct roundInfo {
  int firstTurn;
  int lives;
  int blanks;
  int plr1hp;
  int plr2hp;
  int turn;
  int roundNum;
  char * plr1;
  char * plr2;
  int plr1pid;
  int plr2pid;
};
void startRound(int plrNum, struct roundInfo ri);
void playRound(int plrNum, struct roundInfo ri, int sameTurn);
void gameOver();
int chooseBullet(int lives, int blanks);
void nameSetup(int plrNum);
#endif
