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
    close(fd); //close pipe
    startRound(0, 0);
  }else{ //player 2
    int fd = open("wkp", O_WRONLY); //connect to existing player 1
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Welcome. You have joined another player.\n");
    close(fd); //close pipe
    startRound(1, 0);
  }
  return;
}

/*=========================
  startRound
  args: plrNum, turn

  takes an argument plrNum which decides the players and sets up the game. runs playRound(int plrnum, struct roundInfo ri) after.
  takes an argument turn which dictates if the other player is the first turn.
  creates round information, and sends it via the wkp.

  returns n/a
  =========================*/
void startRound(int plrNum, int turn){
  printf("startRound ran\n");
  if (plrNum == turn){ //the player that loads the gun. creates the shell order, and sends information regarding it to the wkp.
    printf("You are the player that creates the shell order.\n");
    //set up # of lives and # of blanks
    srand(time(NULL));
    int lives = 0;
    int blanks = 0;
    for (int i = 0; i < rand() % (8 - 2 + 1) + 2; i++){ //create shell order.
        int lbSetter = rand() % (1 - 0 + 1) + 0;
        if (lbSetter == 0){
            lives += 1;
        }else if (lbSetter == 1){
            blanks += 1;
        }else{
            printf("Issue with creating shell! lbSetter got value that is not 0 nor 1.\n"); //error
        }
    }
    //hp for current round
    int hp = rand() % (6 - 2 + 1) + 2;
    struct roundInfo ri; //setup roundInfo
    ri.firstTurn = 0;
    ri.lives = lives;
    ri.blanks = blanks;
    ri.plr1hp = hp;
    ri.plr2hp = hp;
    ri.turn = turn;
    //create info to send to other plr
    char rIC[20];
    int bytes;
    bytes = sprintf(rIC, "%d %d %d %d %d %d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn);
    printf("info to send: %s\n", rIC);
    //send info to other plr
    printf("writing to other player\n");
    int fd = open("wkp", O_WRONLY);
    if (fd == -1){ //if during connetion error happens
      printf("startRound: open error: %d: %s\n", errno, strerror(errno));
    }
    write(fd, rIC, bytes);
    close(fd);
    //display info to current plr
    printf("The number of lives this round are: %d.\nThe number of blanks this round are: %d.\nBoth players have %d charges.\n", lives, blanks, hp);
  }else{ //the player that recieves the shell order.
    printf("You are the player that recieves the shell order.\n");
    //set up char to read
    char rIC[20];
    printf("reading for round info\n");
    int fd = open("wkp", O_RDONLY); //read from other plr
    if (fd == -1){ //if during connetion error happens
      printf("startRound: open error: %d: %s\n", errno, strerror(errno));
    }
    read(fd, rIC, 20);
    printf("info obtained: %s\n", rIC);
    close(fd); //close pipe
  }
  return;
}

/*=========================
  recieveRound
  args: n/a

  displays round information via wkp from sending client

  returns n/a
  =========================*/
void recieveRound(){
  printf("recieve round ran\n");
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
