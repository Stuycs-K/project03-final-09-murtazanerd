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
    ri.roundNum = 0;
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
    ri.roundNum = 0;
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
  //both players
  if (ri.firstTurn == 0){
    ri.roundNum += 1;
    if (ri.roundNum >= 4){
      gameOver();
      return;
    }
    printf("A monitor to your side beeps.\n");
    sleep(1);
    printf("It reads...\n");
    sleep(1);
    printf("[[ ROUND %d ]]\n", ri.roundNum);
    sleep(1);
  }
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
    playRound(plrNum, ri, 0);
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
    playRound(plrNum, ri, 0);
  }
  return;
}

/*=========================
  gameOver
  args: n/a

  finishes the game, displaying text.

  returns n/a.
  =========================*/
void gameOver(){
  printf("3 rounds. That's enough.\n");
  sleep(1);
  printf("You've both played well.\n");
  sleep(1);
  printf("You pat each other on the back, and part ways.\n");
  sleep(1);
  printf("When you walk out the entrance of the club...\n");
  sleep(1);
  printf("You can see the sun start to rise.\n");
  sleep(1);
  printf("Thank you for playing my version of Buckshot Roulette!\n");
  sleep(1);
  printf("Hope you had at least a little bit of fun.\n");
  return;
}

/*=========================
  chooseBullet
  args: lives, blanks

  returns an int that determines whether a blank or live was chosen among lives and blanks (0 = LIVE, 1 = BLANK)

  returns int
  =========================*/
int chooseBullet(int lives, int blanks){
  //if one is zero, just don't bother doing random
  if (lives == 0){
    return 1;
  }else if (blanks == 0){
    return 0;
  }
  int total = lives + blanks; //set total
  srand(time(NULL));
  int randNum = rand() % (total - 1 + 1) + 1; //set random number between 1 and total, inclusive
  if (randNum > lives){ //BLANK
    return 1;
  }else{
    return 0; //LIVE
  }
}

/*=========================
  playRound
  args: plrNum, ri, sameTurn

  recursive function that plays the turns out. displays most dialouge, allows player actions, and sends messages through WKP to both clients about what is happening.
  plrNum is used to help seperate the clients. ri is used to keep roundInfo with each turn. sameTurn is used for specific dialouge.

  returns n/a
  =========================*/
void playRound(int plrNum, struct roundInfo ri, int sameTurn){
  //check if someone is dead
  if (ri.plr1hp == 0 || ri.plr2hp == 0){
    if (plrNum == ri.turn){ //winner
      printf("The monitor beeps.\n");
      sleep(1);
      printf("It plays a jingle, and text is displayed: \"YOU WON!\"\n");
      sleep(1);
      printf("A mechanism pulls down a briefcase from the top of the room, and shoves it towards you...\n");
      sleep(1);
      printf("You open it to see the wads of cash you were expecting, then put it aside.\n");
      sleep(1);
      printf("The night is still young.\n");
      sleep(1);
      ri.firstTurn = 0;
      startRound(plrNum, ri);
    }else{ //loser
      printf("The monitor beeps.\n");
      sleep(1);
      printf("It plays a jingle, and text is displayed: \"OTHER WON!\"\n");
      sleep(1);
      printf("A mechanism pulls down a briefcase from the top of the room, and shoves it towards OTHER...\n");
      sleep(1);
      printf("You grudgingly watch OTHER enjoy his rewards.\n");
      sleep(1);
      printf("But it's okay.\n");
      sleep(1);
      printf("The night is still young.\n");
      sleep(1);
      ri.firstTurn = 0;
      startRound(plrNum, ri);
    }
  }
  //check if the current round is over (no shells left)
  if (ri.lives == 0 && ri.blanks == 0){
    startRound(plrNum, ri); //reload the shells
  }else{
    if (ri.turn == plrNum){ //the player whose turn it is
      //give player options
      sleep(1);
      if (sameTurn == 1){ //this is the same turn, plr hasnt changed
        printf("The circular machinery does not move. It is still your turn.\n");
      }else{
        printf("The circular machinery rotates the buck of the gun facing you, signalling that it is your turn.\n");
      }
      sleep(1);
      while(ri.turn == plrNum){
        printf("The table is yours.\n");
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
          int invalid = 0; //in case invalid command is typed
          printf("You pick up the shotgun.\n");
          sleep(1);
          printf("There is no going back now.\n");
          while (invalid == 0){
            sleep(1);
            printf("Who will you shoot?\n[OTHER] [SELF]\n(Shooting yourself with a blank skips OTHER's turn.)\n");
            char pickInput[20];
            fgets(pickInput, 20, stdin); //wait for player input
            if (strcmp(pickInput, "OTHER\n") == 0){ //player shot at OTHER
              //send info that gun is being aimed at other.
              ACTION = 1;
              bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn, ACTION);
              fd = open("wkp", O_WRONLY);
              if (fd == -1){ //err during connection
                printf("playRound: open error: %d: %s\n", errno, strerror(errno));
              }
              write(fd, rIC, bytes);
              close(fd); //close pipe
              //dialouge
              printf("You aim the shotgun at OTHER...\n");
              sleep(1);
              printf("Your hands tremble as you pull the trigger...\n");
              sleep(1);
              printf("...\n");
              sleep(1);
              invalid = 1; //valid command
              //create info to send to other player
              ri.firstTurn = 1; //no longer first turn, if it was
              //pick bullet to be shot
              int bullet = chooseBullet(ri.lives, ri.blanks);
              //change vars accordingly
              if (bullet == 0){ //LIVE
                if (plrNum == 0){ //plr2 is shot
                  ri.plr2hp -= 1;
                }else{ //plr1 is shot
                  ri.plr1hp -= 1;
                }
                ri.lives -= 1;
                ACTION = 4;
              }else{ //BLANK
                ri.blanks -= 1; //no one takes damage. but a blank is lost.
                ACTION = 3;
              }
              if (ri.turn == 0){ //set turn (must happen either way, as you are shooting OTHER)
                ri.turn = 1;
              }else{
                ri.turn = 0;
              }
              //send data
              bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn, ACTION);
              fd = open("wkp", O_WRONLY);
              if (fd == -1){ //err during connection
                printf("playRound: open error: %d: %s\n", errno, strerror(errno));
              }
              write(fd, rIC, bytes);
              close(fd); //close pipe
              //show dialouge
              if (ACTION == 4){
                printf("BAM!\n");
                sleep(1);
                printf("OTHER falls to the floor.\n");
                sleep(1);
                printf("The blood of OTHER spills onto the table.\n");
                sleep(1);
                printf("Suddenly, the defilibrators activate and restart OTHER's heartbeat.\n");
                sleep(1);
                printf("OTHER gets up.\n");
                sleep(1);
                printf("You rack the shotgun.\n");
                sleep(1);
                printf("A used live shell pops out.\n");
                sleep(1);
                printf("The monitor beeps.\n");
                sleep(1);
                printf("OTHER has lost a charge.\n");
              }else if (ACTION == 3){
                printf("Click...\n");
                sleep(1);
                printf("Silence fills the room.\n");
                sleep(1);
                printf("You rack the shotgun.\n");
                sleep(1);
                printf("A blank shell pops out.\n");
              }
              playRound(plrNum, ri, 0); //restart the turn order
            }else if (strcmp(pickInput, "SELF\n") == 0){ //player shoots self
              invalid = 1; //valid command
              //send info that gun is being aimed at self.
              ACTION = 2;
              bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn, ACTION);
              fd = open("wkp", O_WRONLY);
              if (fd == -1){ //err during connection
                printf("playRound: open error: %d: %s\n", errno, strerror(errno));
              }
              write(fd, rIC, bytes);
              close(fd); //close pipe
              //dialouge
              printf("You aim the shotgun right below your chin. You can almost see down the barrel..\n");
              sleep(1);
              printf("Your hands tremble as you pull the trigger...\n");
              sleep(1);
              printf("...\n");
              sleep(1);
              invalid = 1; //valid command
              //create info to send to other player
              ri.firstTurn = 1; //no longer first turn, if it was
              //pick bullet to be shot
              int bullet = chooseBullet(ri.lives, ri.blanks);
              //change vars accordingly
              if (bullet == 0){ //LIVE
                if (plrNum == 0){ //plr1 is shot
                  ri.plr1hp -= 1;
                }else{ //plr2 is shot
                  ri.plr2hp -= 1;
                }
                if (ri.turn == 0){ //change turn
                  ri.turn = 1;
                }else{
                  ri.turn = 0;
                }
                ri.lives -= 1;
                ACTION = 4;
              }else{ //BLANK [turn stays]
                ri.blanks -= 1; //no one takes damage. but a blank is lost.
                ACTION = 3;
              }
              //send data
              bytes = sprintf(rIC, "%d-%d-%d-%d-%d-%d-%d", ri.firstTurn, ri.lives, ri.blanks, ri.plr1hp, ri.plr2hp, ri.turn, ACTION);
              fd = open("wkp", O_WRONLY);
              if (fd == -1){ //err during connection
                printf("playRound: open error: %d: %s\n", errno, strerror(errno));
              }
              write(fd, rIC, bytes);
              close(fd); //close pipe
              //show dialouge
              if (ACTION == 4){
                printf("BAM!\n");
                sleep(1);
                printf("Silence.\n");
                sleep(1);
                printf("You feel the possible sweet release of death coming towards you...\n");
                sleep(1);
                printf("But as you were about to embrace it...\n");
                sleep(1);
                printf("BZZT!\n");
                sleep(1);
                printf("You are brought back to life as you feel a shock within your whole body.\n");
                sleep(1);
                printf("In front of you, lies a used live shell.\n");
                sleep(1);
                printf("The monitor beeps.\n");
                sleep(1);
                printf("You have lost a charge.\n");
                playRound(plrNum, ri, 0);
              }else if (ACTION == 3){
                printf("Click...\n");
                sleep(1);
                printf("You take a deep breath of relief.\n");
                sleep(1);
                printf("You rack the shotgun.\n");
                sleep(1);
                printf("A blank bullet pops out.\n");
                sleep(1);
                printf("You have been given another turn.\n");
                playRound(plrNum, ri, 1);
              }
            }else{ //invalid command
              printf("Invalid command. Current available commands:\n[SELF] - Point and shoot the shotgun at yourself.\n[OTHER] - Point and shoot the shotgun at OTHER.\n");
            }
          }
        }else{
          printf("Invalid command. Current available commands:\n[SHOOT] - Pick up the shotgun, and get ready to shoot someone. (You can't put the shotgun down if you have picked it up.)\n");
        }
      }
    }else{ //other player, waiting
      //reset whoShot
      int whoShot; //0 = OTHER, 1 = SELF
      if (sameTurn == 1){ //still other players turn
        printf("The circular machinery does not move.\n");
        sleep(1);
        printf("It is still OTHER's turn.\n");
      }else{
        printf("The circular machinery rotates the buck of the gun facing OTHER, signalling that it is their turn.\n");
      }
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
        if (ACTION == 1){
          whoShot = 1;
          printf("OTHER aims the barrel towards you...\n");
          sleep(1);
          printf("You take a deep breath.\n");
          sleep(1);
        }
        if (ACTION == 2){
          whoShot = 0;
          printf("OTHER holds the gun up to their chin...\n");
          sleep(1);
          printf("You watch solemnly.\n");
          sleep(1);
          printf("...\n");
        }
        if (ACTION == 3){
          printf("Click...\n");
          sleep(1);
          if (whoShot == 0){
            printf("OTHER takes a deep breath of relief.\n");
            sleep(1);
            printf("OTHER racks the shotgun.\n");
            sleep(1);
            printf("A blank bullet pops out.\n");
            sleep(1);
            printf("OTHER gets another turn.\n");
            sleep(1);
            playRound(plrNum, ri, 1);
          }else{
            printf("Your take a deep breath of relief.\n");
            sleep(1);
            printf("How unfortunate for OTHER.\n");
            sleep(1);
            printf("OTHER racks the shotgun.\n");
            sleep(1);
            printf("A blank bullet pops out.\n");
            sleep(1);
            playRound(plrNum, ri, 0);
          }
        }
        if (ACTION == 4){
          printf("BAM!\n");
          sleep(1);
          if (whoShot == 0){
            printf("OTHER falls off the table.\n");
            sleep(1);
            printf("A used live bullet falls out the shotgun.\n");
            sleep(1);
            printf("...\n");
            sleep(1);
            printf("OTHER gets up.\n");
            sleep(1);
            printf("The monitor beeps.\n");
            sleep(1);
            printf("OTHER has lost a charge.\n");
            sleep(1);
            playRound(plrNum, ri, 0);
          }else{
            printf("Silence.\n");
            sleep(1);
            printf("You feel the possible sweet release of death coming towards you...\n");
            sleep(1);
            printf("But as you were about to embrace it...\n");
            sleep(1);
            printf("BZZT!\n");
            sleep(1);
            printf("You are brought back to life as you feel a shock within your whole body.\n");
            sleep(1);
            printf("In front of you, lies a used live shell.\n");
            sleep(1);
            printf("The monitor beeps.\n");
            sleep(1);
            printf("You have lost a charge.\n");
            sleep(1);
            playRound(plrNum, ri, 0);
          }
        }
      }
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
