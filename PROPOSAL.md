# Final Project Proposal

## Group Members:

Naf (Nafiyu Murtaza)
       
# Intentions:

The indie turn-based game [Buckshot Roulette](https://store.steampowered.com/app/2835570/Buckshot_Roulette/) by Mike Klubnika, but extremely simplified for the purposes of this project.
    
# Intended usage:

Text will feature the current state of the round, and show statistics such as both players' HP and what items they have (if applicable).  

Users will type from a list of commands the game shows (i.e SHOOT | ITEM | INFO). Some commands feature multiple arguments (ITEM COLA).  

Actions are shown in the form of text appearing on both clients (i.e Player1 holds the gun, and points it at you... | You hold the gun, pointing it towards Player2...). 
  
# Technical Details:

`Allocating Memory` will be used for holding the shotgun shell order (in the form of an array), as well as structs that define current round behavior (checks such as is the other player cuffed? is the shotgun sawed off? etc.) if applicable.  

`Semaphores` will be used for creating a loading environment for the first player that runs the program, waiting for a second player to run. This may not be applicable should I attempt a two-pipe connection as you suggested.  

`Pipes` will be used to handle transferring information between clients.  

`Shared Memory` may be used for keeping information in a place that is not a server.  

`Signals` will be used to disconnect and show applicable player(s) messages should the other client disconnect (i.e Player2 quit...).  

I am responsible for 100% of the project. I will begin with setting the connections and being able to share information and print statements on both clients and then begin making the actual game.  

`Structs` will be used to hold round information. Annd that's about it for data structures, other than basic structures like ints, etc.
    
# Intended pacing:

`January 9-12` - Get connections and transferring information done (The 'Systems' part, arguably in my opinion the hardest part).  

`January 15` - Get the skeleton game running (The 'bare minimum' to submit, should be easy).  

`January 21` - Add as much extra stuff as possible by this point (Items, Names, dialouge lines, wait times between dialouge lines... the lengthiest part).  

Note certain dates may be pushed, especially the Systems part as it is the most important aspect of the project.
