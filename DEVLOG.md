# Dev Log:

This document must be updated daily by EACH group member.

## Naf

### 2025-01-06 - PROPOSAL.md, README.md, DEVLOG.md  
`2:10 - 2:50 (Class)`  
The repository was setup and cloned. `(10 min)`  
`PROPOSAL.md` was completely written and added to the repository. `(5 min)`  
`README.md` was completely written (Exception: The install guide) and added to the repository. `(9 min)`  
`DEVLOG.md` has been setup properly and is now being written on, and will be added to the repository. `(3 min)`  
Accurate time taken has been updated onto `DEVLOG.md`. A few formatting edits were published as well. `(2 min)`

### 2025-01-07 - game.c, game.h, commit author fixes
`8:00 - 8:50 (Home, Absent from School)`  
Files `game.c` and `game.h` were created, and contain functions, headers, and comments. `(30 min)`  
Issues with my device commiting under my name were resolved. `(10 min)`
`8:50 - 9:20 (Home, Absent from School)`
wkp connection established between clients `(30 min)`

### 2025-01-08 - branching setup, function flow set, progress on startRound
`12:00 - 12:50 (Pd 5 - 6, yesterday's 'homework' time, except doing it today)`  
The connection flow between the functions `startRound`, `receiveRound`, and `connect`. `(20 min)`  
Print checks were added to test flow. `(3 min)`  
Branch `naf` was created and is now being used for changes. `(2 min)`  
New function: `receiveRound`. Appropriate details added. `(25 min)`  
`2:10 - 2:50 (Class time)`  
`startRound`: creating round information added. connection between clients sending/receiving information added. dialouge text added. `(40 min)`  
`2:50 - 3:34 (Home time)`  
`startRound`: all dialouge cleaned up, sleep()s added, issues regarding sending information fixed, strsep connections added. `(41 min)`

### 2025-01-09 - rewriting startRound, beginning playRound
`2:10 - 2:50 (Class time)`  
`startRound`: rewrote the entire function to now not only fill structs for the very start of a round, but to also reload the gun when it runs out of bullets. this flows with the struct nicely. `(41 min)`  
`2:50 - 3:35 (Home time that I'm doing now because I have 10th free)`  
`startRound`: minute changes to dialouge. `(15 min)`  
`playRound`: begun work on it, seperating active/inactive players with correct pipe networking. currently not finished with this. `(30 min)`  

### 2025-01-10 - chooseBullet, progress on playRound
`10:20 - 11:01 (Homework time except I do it before class today because I have a free then)`  
Created a new function `chooseBullet` and completed it. `(20 min)`  
`playRound`: made more progress on it, adding live and blank functionality. `(21 min)` 
   
Removed `receiveRound`. `(less than a minute)`  

### 2025-01-14 - full working (sort of) game!!!
`10:20 - 11:01 (Classwork time for yesterday)`  
Finished OTHER dialouge for `playRound`. `(25 min)`  
Got a full working game! There are still some bugs here and there, but after creating the `endGame` function and doing some minor tweaks to dialouge and player tracking in `startRound` it's done! Full game. At least, the skeleton ver. `(16 min)`  
`12:21 - 1:19 (Homework time for yesterday)`  
Began naming system (precisely, obtaining names from the two clients). `(~1 hour)`  
`2:10 - 2:50 (Class)`  
Began replacing all the OTHER's with appropriate names, using if statements. `(36 min)`  
Additionally tweaked a few bugs. `(5 min)`

### 2025-01-15 - Naming done + more tweaks
`1:00 - 3:00 (Home)`  
Finished the rest of naming dialouge. `(40 mins)`  
Adjustments to dialouge time. `(10 min)`  
