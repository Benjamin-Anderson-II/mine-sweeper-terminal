#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define board_height 6
#define board_width 12
#define board_size  72

enum cell_states {EMPTY, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, BOMB};
enum top_layer_states{HIDDEN, SHOWN, FLAGGED};

const int num_of_bombs = 10;

int curr_index = 0;
int flags_used = 0;

//randomly puts a number of bombs on the board
void putMinesOnBoard(int* board, int numOfMines){
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < numOfMines; i++){
		while(1){
			int bomb_index = rand() % board_size;
			if(board[bomb_index]!=BOMB){
				board[bomb_index] = BOMB;
				break;
			}		
		}
	}
}

//this is to be called after putMinesOnBoard()
//it puts the numbers around the bombs, so that you knoow whether a bomb is nearby or not
void putNumsOnBoard(int* board){
	int r, c;//row an column of the current index on the board
	for(int i = 0; i < board_size; i++){

		//so that I don't go out of bounds
		//naturally initializes to false
		bool unavailable_spaces[8] = {false, false, false, false, false, false, false, false};

		c=i%board_width;
		r=(i-c)/board_width;

		//when comes across a bomb, increment everything around it by one
		if(board[i]==BOMB){
			if(r==0){//top border exists
				unavailable_spaces[0]=true;
				unavailable_spaces[1]=true;
				unavailable_spaces[2]=true;
			}
			if(r==board_height-1){//bottom border exists
				unavailable_spaces[5]=true;
				unavailable_spaces[6]=true;
				unavailable_spaces[7]=true;
			}
			if(c==0){//left border exists
				unavailable_spaces[0]=true;
				unavailable_spaces[3]=true;
				unavailable_spaces[5]=true;
			}
			if(c==board_width-1){//right border exists
				unavailable_spaces[2]=true;
				unavailable_spaces[4]=true;
				unavailable_spaces[7]=true;
			}

			for(int i = 0; i < sizeof(unavailable_spaces); i++){
				if(unavailable_spaces[i]){
					//printf("skip%d\n",i);
					continue; //makes sure the space can be indexed properly
				}
				switch(i){
					case 0: //top-left of current pos
					if(board[(r-1)*board_width+(c-1)]!=BOMB)
						board[(r-1)*board_width+(c-1)]++;
					break;
					case 1: //top-center of current pos
					if(board[(r-1)*board_width+c]!=BOMB)
						board[(r-1)*board_width+c]++;
					break;
					case 2: //top-right of cuurent pos
					if(board[(r-1)*board_width+(c+1)]!=BOMB)
						board[(r-1)*board_width+(c+1)]++;
					break;
					case 3: //left of current pos
					if(board[r*board_width+(c-1)]!=BOMB)
						board[r*board_width+(c-1)]++;
					break;
					case 4: //right of current pos
					if(board[r*board_width+(c+1)]!=BOMB)
						board[r*board_width+(c+1)]++;
					break;
					case 5: //bottom-left of current pos
					if(board[(r+1)*board_width+(c-1)]!=BOMB)
						board[(r+1)*board_width+(c-1)]++;
					break;
					case 6: //bottom of current pos
					if(board[(r+1)*board_width+c]!=BOMB)
						board[(r+1)*board_width+c]++;
					break;
					case 7: //bottom-right of current pos
					if(board[(r+1)*board_width+(c+1)]!=BOMB)
						board[(r+1)*board_width+(c+1)]++;
					break;
				}
			}
		}
	}
}

//prints the board
void printBoard(int* board){
	for(int i = 0; i < board_height; i++){
		for(int j = 0; j < board_width; j++){
			printf("%d", board[i*board_width+j]);
			if(i*board_width+j==curr_index) printf(".");
			else printf(" ");
		}
		printf("\n");
	}
}

void printShownBoard(int* screen, int* board){
	for(int i = 0; i < board_height; i++){
		for(int j = 0; j < board_width; j++){
			switch(screen[i*board_width+j]){
				case SHOWN:
				printf("%d", board[i*board_width+j]);
				break;
				case FLAGGED:
				printf("F");
				break;
				case HIDDEN:
				printf("@");

			}
			if(i*board_width+j==curr_index) printf("."); //for cursor
			else printf(" ");
		}
		printf("\n");
	}
}

bool performAction(int *screen, char dir){
	switch(dir){
		case 'w':
			curr_index -= board_width;
			return true;
		case 's':
			curr_index += board_width;
			return true;
		case 'a':
			curr_index--;
			return true;
		case 'd':
			curr_index++;
			return true;
		case 'c':
			screen[curr_index] = SHOWN;
			return true;
		case 'f':
			if(screen[curr_index]!=SHOWN && flags_used < num_of_bombs){
				if(screen[curr_index]!=FLAGGED){
					screen[curr_index]=FLAGGED;
					flags_used++;
				}else{
					screen[curr_index]=HIDDEN;
					flags_used--;
				}
			}
			return true;
		default: return false;
	}
}

bool checkIfEnd(int *board, int *screen){
	if(screen[curr_index]==SHOWN&&board[curr_index]==BOMB) return true;
	int flagged_bombs = 0;
	for(int i = 0; i < board_size; i++){
		if(screen[i]==FLAGGED&&board[i]==BOMB) flagged_bombs++;
	}
	if(flagged_bombs==num_of_bombs)return true;
	return false;
}

int main(int argc, char const *argv[]){
	//number of mines = 10

	//This is the board that has the bombs / number
	static int board[board_size];
	//This is the board that tells the computer what the player sees 
	static int curr_show[board_size];

	putMinesOnBoard(board, num_of_bombs);
	putNumsOnBoard(board);
	char dir;
	while(true){
		printShownBoard(curr_show, board);
		while(true){
			scanf("%c", &dir);
			if(performAction(curr_show, dir))break;
		}
		system("cls");
		if(checkIfEnd(board, curr_show))break;
	}

	return 0;
}

/*TO-DO
	make board with random spaces for mines and number of bombs around a given spot
	make array that shows what's on the board
	funciton for showing a space
	function for flagging a bomb
	win condition
	implement faux movement for terminal with fscan() {wasd}
	if bomb is checked fail

	THINGS THAT HAVE TO BE CHANGED IN ARDUINO
	visualize current movement index
	moving the "camera" (do it based on chunks, instead of dynamically moving the camera)
*/