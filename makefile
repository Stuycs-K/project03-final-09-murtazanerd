compile: game.c
	@gcc -o game game.c -Wall
run:
	@./game
clean:
	@rm -f game wkp
