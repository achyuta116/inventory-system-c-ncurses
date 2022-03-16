#include "header.h"
#include <ncurses.h>
int main(int argc, char* argv[]){
	initscr();
	int rows,columns;
	getmaxyx(stdscr,rows,columns);
	if(argc != 2){
		printw("Usage : %s <file_name>",argv[0]);
		mvprintw(rows-1,0,"Enter a key to exit");
		refresh();
		getch();
		endwin();
		return 1;
	}
	FILE *file = fopen(argv[1],"r");
	if(file == NULL){
		printw("File does not exist");
		mvprintw(rows-1,0,"Enter a key to exit");
		refresh();
		getch();
		endwin();
		return 1;
	}
	fclose(file);
	initialisation(argv[1]);
	endwin();
	return 0;

}