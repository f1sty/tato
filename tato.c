#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

typedef enum { BLOCK, BREAK, LONG_BREAK } Status;

void print_centre(char *str, Status status) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  int x = (getmaxx(stdscr) - strlen(str)) / 2;
  int y = getmaxy(stdscr) / 2;

  if (status == BLOCK) {
    attron(COLOR_PAIR(1));
  } else {
    attron(COLOR_PAIR(2));
  }
  mvprintw(y, x, "%s", str);
  if (status == BLOCK) {
    attroff(COLOR_PAIR(1));
  } else {
    attroff(COLOR_PAIR(2));
  }
}

int main(void) {
  initscr();
  char *status = "initial status in here";

  raw();
  noecho();
  curs_set(0);
  print_centre(status, BLOCK);
  refresh();
  int ch;
  while ((ch = getch())) {
    if (ch == 'p') {
      clear();
      print_centre("you pressed `p`", BREAK);
    } else {
      break;
    }
    refresh();
  }
  endwin();

  return 0;
}
