#include "game.h"

/*=========================
  connect
  args: none

  either connects to an existing WKP if it was made (second client/player), or creates the WKP and awaits connection (first client/player)
  after connection, both clients run setup(int plrNum)

  returns n/a
  =========================*/
void connect(){
  return;
}

/*=========================
  setup
  args: plrNum

  takes an argument plrNum which decides the players and sets up the game. runs playRound(int plrnum, struct roundInfo ri) after.

  returns n/a
  =========================*/
void startRound(int plrNum){
  return;
}

/*=========================
  playRound
  args: turn

  recursive function that takes an argument turn which dictates whose turn it is. shows statistics and player options. one end of the WKP should be waiting (the player whose turn it's not) whilst the other end should
  write after the player does something. plrNum helps seperate multiple instances of the game, and keeps turns in check. base case is when one player has 0 hp. pipes are closed at end of turn.

  returns n/a
  =========================*/
void playRound(int plrNum, struct roundInfo ri){
  if (plrNum == 0){
    //player 1 client code here
  }
  if (plrNum == 1){
    //player 2 client code here
  }
  return;
}

/*=========================
  translate
  args: buff

  translates messages sent by the WKPs into structs that can be read. translate should be used at the end of receiving clients' READ buffer.

  returns struct roundInfo
  =========================*/
struct roundInfo translate(char * buff){
  return NULL;
}
