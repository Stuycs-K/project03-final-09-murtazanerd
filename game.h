#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>
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
};
void startRound(int plrNum, struct roundInfo ri);
void playRound(int plrNum, struct roundInfo ri);
struct roundInfo translate(char * buff);
int chooseBullet(int lives, int blanks);
#endif
