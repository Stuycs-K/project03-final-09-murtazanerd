#include "game.h"

//main
int main(){
  connect();
  return 0;
}
/*=========================
  connect
  args: none

  either connects to an existing WKP if it was made (second client/player), or creates the WKP and awaits connection (first client/player)
  after connection, both clients run startRound(int plrNum, int turn)

  returns n/a
  =========================*/
void connect(){
  if (open("wkp", O_WRONLY) == -1){ //player 1
    if (mkfifo("wkp", 0650) == -1){ //create wkp
      printf("game.c: connect: mkfifo error: %d: %s\n", errno, strerror(errno)); //error if wkp isn't made 4 some reason
    }
    printf("Welcome. You are the first player. Awaiting a second player to accompany you...\n");
    int fd = open("wkp", O_RDONLY); //wait for connect from player 2
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Connected!\n");
    startRound(0, 0);
  }else{ //player 2
    int fd = open("wkp", O_WRONLY); //connect to existing player 1
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Welcome. You have joined another player.\n");
    receiveRound(1, 0);
  }
  return;
}

/*=========================
  startRound
  args: plrNum, turn

  takes an argument plrNum which decides the players and sets up the game. runs playRound(int plrnum, struct roundInfo ri) after.
  takes an argument turn which dictates if the other player is the first turn.

  returns n/a
  =========================*/
void startRound(int plrNum, int turn){
  if (plrNum == turn){ //the player that loads the gun
    //setup round stats
    int shellLen = 0 //blank val for now
    struct roundInfo ri; //setup roundInfo
    ri.firstTurn = 0;
    ri.lives = 0;
    ri.blanks = 0;
    ri.plr1hp = 0;
    r1.plr2hp = 0;
    r1.turn = turn;
  }else{ //the player that receives round info via wkp

  }
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
  //check common round info here
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
  struct roundInfo s; //blank
  s.firstTurn = 0;
  s.lives = 0;
  s.blanks = 0;
  s.plr1hp = 0;
  s.plr2hp = 0;
  s.turn = 0;
  return s;
}
