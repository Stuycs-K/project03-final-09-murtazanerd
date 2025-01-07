#ifndef PIPE_H
#define PIPE_H
int connect();
struct roundInfo {
  int firstTurn;
  int lives;
  int blanks;
  int plr1hp;
  int plr2hp;
  int turn;
};
void setup(int plrNum);
void playRound(int turn);
#endif
