#include "game.h"

//group id 16

//main
int main(){
  connect();
  return 0;
}

/*=========================
  connect
  args: none

  either connects to an existing WKP if it was made (second client/player), or creates the WKP and asleeps connection (first client/player)
  after connection, both clients run nameSetup(int plrNum)

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
    printf("A player has joined!\n");
    close(fd); //close pipe
    sleep(1);
    nameSetup(0);
  }else{ //player 2
    int fd = open("wkp", O_WRONLY); //connect to existing player 1
    if (fd == -1){ //if during connetion error happens
      printf("connect: open error: %d: %s\n", errno, strerror(errno));
    }
    printf("Welcome. You have joined another player.\n");
    close(fd); //close pipe
    nameSetup(1);
  }
  return;
}

/*=========================
  nameSetup
  args: plrNum, ri.

  gets names and sends it to each other, then sends info to structs. plrNum differentiates the players
  creates struct ri

  returns n/a
  =========================*/
void nameSetup(int plrNum){
  //setup struct for game
  struct roundInfo ri;
  ri.firstTurn = 0;
  ri.lives = 0;
  ri.blanks = 0;
  ri.plr1hp = 0;
  ri.plr2hp = 0;
  ri.roundNum = 0;
  ri.turn = 0;
  if (plrNum == 0){ //client 1
    //setup name var
    char input[7];
    //dialouge
    sleep(1);
    printf("You walk into the club, late at night.\n");
    sleep(1);
    printf("You get situated in a room, with another person stood at the other side of the table.\n");
    sleep(1);
    printf("You shake hands, and prepare to begin.\n");
    sleep(1);
    printf("The dealer approaches you both. He gives you each a contract.\n");
    sleep(1);
    printf("The paper looks dry, but you can make out a line at the bottom, requesting your signature.\n");
    sleep(1);
    printf("The dealer looks at you both calmly, then asks...\n");
    sleep(1);
    printf("\"Please sign the contract.\"\n");
    sleep(1);
    printf("You pick up the piece of paper, and a machine opens up to the side, which looks like a hole-puncher but with a mechanical keyboard?\n");
    sleep(1);
    printf("You start to type...\n");
    sleep(1);
    printf("Please sign the contract. (Type in your name, 6 characters maximum.)\n");
    fgets(input, 7, stdin); //wait for player input
    sleep(1);
    printf("You sign... %s", input);
    sleep(1);
    //fix the \n
    input[strlen(input) - 1] = '\0';
    ri.plr1 = input;
    //send pipe (info abt plr1)
    int fd = open("wkp", O_WRONLY);
    if (fd == -1){ //if during connection error happens
      printf("nameSetup: open error: %d: %s\n", errno, strerror(errno));
    }
    write(fd, input, 7);
    close(fd);
    //recieve pipe (info abt plr2)
    char input2[7];
    printf("The dealer now gives the other person a contract.\n");
    sleep(1);
    printf("Waiting for the other person to finish signing...\n");
    int fd2 = open("wkp", O_RDONLY); //read from other plr
    if (fd2 == -1){ //if during connetion error happens
      printf("nameSetup: open error: %d: %s\n", errno, strerror(errno));
    }
    read(fd2, input2, 7);
    close(fd2); //close pipe
    ri.plr2 = input2;
    printf("They have signed their contract.\n");
    startRound(plrNum, ri);
  }else{ //client 2
    //setup name var
    char input[7];
    //dialouge
    sleep(1);
    printf("You walk into the club, late at night.\n");
    sleep(1);
    printf("You get situated in a room, with another person stood at the other side of the table.\n");
    sleep(1);
    printf("You shake hands, and prepare to begin.\n");
    sleep(1);
    printf("The dealer approaches you both. He gives the other person a contract.\n");
    sleep(1);
    //recieve pipe (info abt plr1)
    char input2[7];
    printf("Waiting for the other person to finish signing...\n");
    int fd = open("wkp", O_RDONLY);
    if (fd == -1){ //if during connetion error happens
      printf("nameSetup: open error: %d: %s\n", errno, strerror(errno));
    }
    read(fd, input2, 7);
    close(fd); //close pipe
    ri.plr1 = input2;
    //send pipe (info abt plr2)
    int fd2 = open("wkp", O_WRONLY);
    printf("The dealer now gives you a contract.\n");
    sleep(1);
    printf("The paper looks dry, but you can make out a line at the bottom, requesting your signature.\n");
    sleep(1);
    printf("The dealer looks at you both calmly, then asks...\n");
    sleep(1);
    printf("\"Please sign the contract.\"\n");
    sleep(1);
    printf("You pick up the piece of paper, and a machine opens up to the side, which looks like a hole-puncher but with a mechanical keyboard?\n");
    sleep(1);
    printf("You start to type...\n");
    sleep(1);
    printf("Please sign the contract. (Type in your name, 6 characters maximum.)\n");
    fgets(input, 7, stdin); //wait for player input
    input[strlen(input) - 1] = '\0';
    ri.plr2 = input;
    sleep(1);
    printf("You sign... %s\n", input);
    if (fd2 == -1){ //if during connetion error happens
      printf("nameSetup: open error: %d: %s\n", errno, strerror(errno));
    }
    write(fd2, input, 7);
    close(fd2);
    startRound(plrNum, ri);
  }
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
      printf("[ %s: %d | %s: %d ]\n", ri.plr1, hp, ri.plr2, hp);
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
    if (plrNum == 0){
      printf("You watch %s load the shells into the shotgun...\n", ri.plr2);
    }else{
      printf("You watch %s load the shells into the shotgun...\n", ri.plr1);
    }
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
      printf("[ %s: %d | %s: %d ]\n", ri.plr1, ri.plr1hp, ri.plr2, ri.plr2hp);
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
  sleep(3);
  kill(getpid(), SIGTERM);
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
      if (plrNum == 0){
        printf("It plays a jingle, and text is displayed: \"%s WON!\"\n", ri.plr1);
      }else{
        printf("It plays a jingle, and text is displayed: \"%s WON!\"\n", ri.plr2);
      }
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
      if (plrNum == 0){
        printf("It plays a jingle, and text is displayed: \"%s WON!\"\n", ri.plr2);
      }else{
        printf("It plays a jingle, and text is displayed: \"%s WON!\"\n", ri.plr1);
      }
      sleep(1);
      if (plrNum == 0){
        printf("A mechanism pulls down a briefcase from the top of the room, and shoves it towards %s...\n", ri.plr2);
      }else{
        printf("A mechanism pulls down a briefcase from the top of the room, and shoves it towards %s...\n", ri.plr1);
      }
      sleep(1);
      if (plrNum == 0){
        printf("You grudgingly watch %s enjoy their rewards.\n", ri.plr2);
      }else{
        printf("You grudgingly watch %s enjoy their rewards.\n", ri.plr1);
      }
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
        if (plrNum == 0){
          printf("[ YOU: %d | %s: %d ]\n[ SHOOT ]\n", ri.plr1hp, ri.plr2, ri.plr2hp); //display stats
        }else{
          printf("[ YOU: %d | %s: %d ]\n[ SHOOT ]\n", ri.plr2hp, ri.plr1, ri.plr1hp); //display stats
        }
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
            if (plrNum == 0){
              printf("Who will you shoot?\n[%s] [SELF]\n(Shooting yourself with a blank skips %s's turn.)\n", ri.plr2, ri.plr2);
            }else{
              printf("Who will you shoot?\n[%s] [SELF]\n(Shooting yourself with a blank skips %s's turn.)\n", ri.plr1, ri.plr1);
            }
            char pickInput[20];
            fgets(pickInput, 20, stdin); //wait for player input
            //parse pickInput
            pickInput[strlen(pickInput) - 1] = '\0';
            if ((strcmp(pickInput, ri.plr1) == 0 && plrNum == 1) || (strcmp(pickInput, ri.plr2) == 0 && plrNum == 0)){ //player shot at OTHER
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
              if (plrNum == 0){
                printf("You aim the shotgun at %s...\n", ri.plr2);
              }else{
                printf("You aim the shotgun at %s...\n", ri.plr1);
              }
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
                if (plrNum == 0){
                  printf("%s falls to the floor.\n", ri.plr2);
                }else{
                  printf("%s falls to the floor.\n", ri.plr1);
                }
                sleep(1);
                if (plrNum == 0){
                  printf("The blood of %s spills onto the table.\n", ri.plr2);
                }else{
                  printf("The blood of %s spills onto the table.\n", ri.plr1);
                }
                sleep(1);
                if (plrNum == 0){
                  printf("Suddenly, the defilibrators activate and restart %s's heartbeat.\n", ri.plr2);
                }else{
                  printf("Suddenly, the defilibrators activate and restart %s's heartbeat.\n", ri.plr1);
                }
                sleep(1);
                if (plrNum == 0){
                  printf("%s gets up.\n", ri.plr2);
                }else{
                  printf("%s gets up.\n", ri.plr1);
                }
                sleep(1);
                printf("You rack the shotgun.\n");
                sleep(1);
                printf("A used live shell pops out.\n");
                sleep(1);
                printf("The monitor beeps.\n");
                sleep(1);
                if (plrNum == 0){
                  printf("%s has lost a charge.\n", ri.plr2);
                }else{
                  printf("%s has lost a charge.\n", ri.plr1);
                }
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
            }else if (strcmp(pickInput, "SELF") == 0){ //player shoots self
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
        if (plrNum == 0){
          printf("It is still %s's turn.\n", ri.plr2);
        }else{
          printf("It is still %s's turn.\n", ri.plr1);
        }
      }else{
        if (plrNum == 0){
          printf("The circular machinery rotates the buck of the gun facing %s, signalling that it is their turn.\n", ri.plr2);
        }else{
          printf("The circular machinery rotates the buck of the gun facing %s, signalling that it is their turn.\n", ri.plr1);
        }
      }
      sleep(1);
      while (ri.turn != plrNum){
        if (plrNum == 0){
          printf("%s decides your fate...\n", ri.plr2);
        }else{
          printf("%s decides your fate...\n", ri.plr1);
        }
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
          if (plrNum == 0){
            printf("%s picks up the shotgun.\n", ri.plr2);
          }else{
            printf("%s picks up the shotgun.\n", ri.plr1);
          }
          sleep(1);
        }
        if (ACTION == 1){
          whoShot = 1;
          if (plrNum == 0){
            printf("%s aims the barrel towards you...\n", ri.plr2);
          }else{
            printf("%s aims the barrel towards you...\n", ri.plr1);
          }
          sleep(1);
          printf("You take a deep breath.\n");
          sleep(1);
        }
        if (ACTION == 2){
          whoShot = 0;
          if (plrNum == 0){
            printf("%s holds the gun up to their chin...\n", ri.plr2);
          }else{
            printf("%s holds the gun up to their chin...\n", ri.plr1);
          }
          sleep(1);
          printf("You watch solemnly.\n");
          sleep(1);
          printf("...\n");
        }
        if (ACTION == 3){
          printf("Click...\n");
          sleep(1);
          if (whoShot == 0){
            if (plrNum == 0){
              printf("%s takes a deep breath of relief.\n", ri.plr2);
            }else{
              printf("%s takes a deep breath of relief.\n", ri.plr1);
            }
            sleep(1);
            if (plrNum == 0){
              printf("%s racks the shotgun.\n", ri.plr2);
            }else{
              printf("%s racks the shotgun.\n", ri.plr1);
            }
            sleep(1);
            printf("A blank bullet pops out.\n");
            sleep(1);
            if (plrNum == 0){
              printf("%s gets another turn.\n", ri.plr2);
            }else{
              printf("%s gets another turn.\n", ri.plr1);
            }
            sleep(1);
            playRound(plrNum, ri, 1);
          }else{
            printf("Your take a deep breath of relief.\n");
            sleep(1);
            if (plrNum == 0){
              printf("How unfortunate for %s.\n", ri.plr2);
            }else{
              printf("How unfortunate for %s.\n", ri.plr1);
            }
            sleep(1);
            if (plrNum == 0){
              printf("%s racks the shotgun.\n", ri.plr2);
            }else{
              printf("%s racks the shotgun.\n", ri.plr1);
            }
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
            if (plrNum == 0){
              printf("%s falls off the table.\n", ri.plr2);
            }else{
              printf("%s falls off the table.\n", ri.plr1);
            }
            sleep(1);
            printf("A used live bullet falls out the shotgun.\n");
            sleep(1);
            printf("...\n");
            sleep(1);
            if (plrNum == 0){
              printf("%s gets up.\n", ri.plr2);
            }else{
              printf("%s gets up.\n", ri.plr1);
            }
            sleep(1);
            printf("The monitor beeps.\n");
            sleep(1);
            if (plrNum == 0){
              printf("%s has lost a charge.\n", ri.plr2);
            }else{
              printf("%s has lost a charge.\n", ri.plr1);
            }
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
