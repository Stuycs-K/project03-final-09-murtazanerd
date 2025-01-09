#include "game.h"

//main
int main(){
  connect();
  return 0;
}
/*=========================
  connect
  args: none

  either connects to an existing WKP if it was made (second client/player), or creates the WKP and asleeps connection (first client/player)
  after connection, both clients run startRound(int plrNum, int turn)
  creates a struct as well, ri, which stores roundInfo.

  returns n/a
  =========================*/
void connect(){
  if (open("wkp", O_WRONLY) == -1){ //player 1
    if (mkfifo("wkp", 0650) == -1){ //create wkp
      printf("game.c: connect: mkfifo error: %d: %s\n", errno, strerror(errno)); //error if wkp isn't made 4 some reason
    }
    printf("Welcome. You are the first player. Awaiting a second player to accompany you...\n");
    int fd = open("wkp", O_RDONLY); //sleep for connect from player 2
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Connected!\n");
    close(fd); //close pipe
    //setup struct for the game
    struct roundInfo ri;
    ri.firstTurn = 0;
    ri.lives = 0;
    ri.blanks = 0;
    ri.plr1hp = 0;
    ri.plr2hp = 0;
    ri.turn = 0;
    startRound(0, ri);
  }else{ //player 2
    int fd = open("wkp", O_WRONLY); //connect to existing player 1
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Welcome. You have joined another player.\n");
    close(fd); //close pipe
    //setup struct for the game
    struct roundInfo ri;
    ri.firstTurn = 0;
    ri.lives = 0;
    ri.blanks = 0;
    ri.plr1hp = 0;
    ri.plr2hp = 0;
    ri.turn = 0;
    startRound(1, ri);
  }
  return;
}

/*=========================
  startRound
  args: plrNum, ri

  takes an argument plrNum which decides the players and sets up the game. runs playRound(int plrnum, struct roundInfo ri) after.
  takes an argument ri which is the struct created in connect() which is filled accordingly. if this is the first turn of the round, hp is set. if not, only shells are set.

  returns n/a
  =========================*/
void startRound(int plrNum, struct roundInfo ri){
  if (plrNum == ri.turn){ //the player that loads the gun. creates the shell order, and sends information regarding it to the wkp.
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
    //setup roundInfo
    ri.lives = lives;
    ri.blanks = blanks;
    if (ri.firstTurn == 0){ //only do hp changes should this be a new round. may want to add item changes here too [all the items removing themselves]
      ri.plr1hp = hp;
      ri.plr2hp = hp;
    }
    //create info to send to other plr
    char rIC[20];
    int bytes;
    bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn);
    //send info to other plr
    int fd = open("wkp", O_WRONLY);
    if (fd == -1){ //if during connetion error happens
      printf("startRound: open error: %d: %s\n", errno, strerror(errno));
    }
    write(fd, rIC, bytes);
    close(fd);
    //display info to current plr
    printf("You load the shells into the shotgun...\n");
    sleep(1);
    if (ri.lives == 1){
      printf("You load %d live bullet.\n", ri.lives);
    }else if (ri.lives != 0){
      printf("You load %d live bullets.\n", ri.lives);
    }
    sleep(1);
    if (ri.blanks == 1){
      printf("You load %d blank bullet.\n", ri.blanks);
    }else if (ri.blanks != 0){
      printf("You load %d blank bullets.\n", ri.blanks);
    }
    if (ri.firstTurn == 0){
      sleep(1);
      printf("The machine to your side lights up and makes a beep sound.\n");
      sleep(1);
      printf("It reads...\n");
      sleep(1);
      printf("YOU: %d | OTHER: %d\n", hp, hp);
    }
  }else{ //the player that recieves the shell order.
    //set up char to read
    char rIC[20];
    int fd = open("wkp", O_RDONLY); //read from other plr
    if (fd == -1){ //if during connetion error happens
      printf("startRound: open error: %d: %s\n", errno, strerror(errno));
    }
    read(fd, rIC, 20);
    close(fd); //close pipe
    //setup struct for playRound
    char *curr = rIC; //set to first val
    ri.firstTurn = (*curr - '0'); //set val to corresponding struct var
    strsep(&curr, "-"); //strsep to second val
    ri.lives = (*curr - '0');
    strsep(&curr, "-");
    ri.blanks = (*curr - '0');
    strsep(&curr, "-");
    ri.plr1hp = (*curr - '0');
    strsep(&curr, "-");
    ri.plr2hp = (*curr - '0');
    strsep(&curr, "-");
    ri.turn = (*curr - '0');
    //display info to current plr
    printf("You watch OTHER load the shells into the shotgun...\n");
    sleep(1);
    if (ri.lives == 1){
      printf("They load %d live bullet.\n", ri.lives);
    }else if (ri.lives != 0){
      printf("They load %d live bullets.\n", ri.lives);
    }
    sleep(1);
    if (ri.blanks == 1){
      printf("They load %d blank bullet.\n", ri.blanks);
    }else if (ri.blanks != 0){
      printf("They load %d blank bullets.\n", ri.blanks);
    }
    if (ri.firstTurn == 0){
      sleep(1);
      printf("The machine to your side lights up and makes a beep sound.\n");
      sleep(1);
      printf("It reads...\n");
      sleep(1);
      printf("YOU: %d | OTHER: %d\n", ri.plr1hp, ri.plr1hp);
    }
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

  recursive function that takes an argument turn which dictates whose turn it is. shows statistics and player options. one end of the WKP should be sleeping (the player whose turn it's not) whilst the other end should
  write after the player does something. plrNum helps seperate multiple instances of the game, and keeps turns in check. base case is when one player has 0 hp. pipes are closed at end of turn.

  returns n/a
  =========================*/
void playRound(int plrNum, struct roundInfo ri){
  //check if the current round is over (no shells left)
  if (ri.lives == 0 && ri.blanks == 0){
    startRound(plrNum, ri); //reload the shells
  }else{
    if (ri.turn == plrNum){ //the player whose turn it is
      //give player options
      printf("The circular machinery rotates the buck of the gun facing you, signalling that it is your turn.\n");
      sleep(1);
      while(ri.turn == plrNum){
        printf("What will you do?\n");
        sleep(1);
        printf("[ YOU: %d | OTHER: %d ]\n[ SHOOT ]\n", ri.plr1hp, ri.plr2hp); //display stats
        char input[20]; //increase for other things in future
        fgets(input, 20, stdin); //wait for player input
        if (strcmp(input, "SHOOT\n") == 0){ //if player selected SHOOT
          int ACTION = 0;
          //create info to send to other plr
          char rIC[22];
          int bytes;
          bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn, ACTION);
          //send info to other plr
          int fd = open("wkp", O_WRONLY);
          if (fd == -1){ //if during connetion error happens
            printf("playRound: open error: %d: %s\n", errno, strerror(errno));
          }
          write(fd, rIC, bytes);
          close(fd); //close pipe
          //display info to plr
          printf("You pick up the shotgun.\n");
          sleep(1);
          printf("There is no going back now.\n");
          sleep(1);
          printf("Who will you shoot?\n[OTHER] [SELF]\n(Shooting yourself with a blank skips OTHER's turn.)\n");

        }
      }
    }else{ //other player, waiting
      printf("The circular machinery rotates the buck of the gun facing OTHER, signalling that it is their turn.\n");
      sleep(1);
      while (ri.turn != plrNum){
        printf("OTHER is deciding...\n");
        //prepare char to read
        char rIC[22];
        int fd = open("wkp", O_RDONLY); //read from other plr
        if (fd == -1){ //if during connetion error happens
          printf("playRound: open error: %d: %s\n", errno, strerror(errno));
        }
        read(fd, rIC, 22);
        close(fd); //close pipe
        //update struct with new info
        char *curr = rIC; //set to first val
        ri.firstTurn = (*curr - '0'); //set val to corresponding struct var
        strsep(&curr, "-"); //strsep to second val
        ri.lives = (*curr - '0');
        strsep(&curr, "-");
        ri.blanks = (*curr - '0');
        strsep(&curr, "-");
        ri.plr1hp = (*curr - '0');
        strsep(&curr, "-");
        ri.plr2hp = (*curr - '0');
        strsep(&curr, "-");
        ri.turn = (*curr - '0');
        strsep(&curr, "-");
        int ACTION = (*curr - '0');
        //ACTIONS
        //0 - Other player picked up the gun.
        //1 - Other player aimed it at you.
        //2 - Other player aimed it at themself.
        //3 - Other player shot a BLANK.
        //4 - Other player shot a LIVE.
        //more soon for other commands.
        //display info to current plr
        if (ACTION == 0){
          printf("OTHER picks up the shotgun.\n");
          sleep(1);
        }
      }
    }
    if (plrNum == 0){
      //player 1 client code here
    }
    if (plrNum == 1){
      //player 2 client code here
    }
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
