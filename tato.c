#include "config.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

typedef enum { WORKING_BLOCK, BREAK, LONG_BREAK } Status;
typedef struct {
  int minutes;
  int seconds;
} TimeSpan;

thrd_t thread;

void print_centre(char *str, Status status) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);

  int x = (getmaxx(stdscr) - strlen(str)) / 2;
  int y = getmaxy(stdscr) / 2;

  switch (status) {
  case WORKING_BLOCK:
    attrset(COLOR_PAIR(1));
    break;
  case BREAK:
    attrset(COLOR_PAIR(2));
    break;
  case LONG_BREAK:
    attrset(COLOR_PAIR(3));
    break;
  default:
    exit(EXIT_FAILURE);
  }
  mvprintw(y, x, "%s", str);
}

char *get_status_str(Status status) {
  switch (status) {
  case WORKING_BLOCK:
    return "working";
  case BREAK:
    return "break";
  case LONG_BREAK:
    return "long break";
  default:
    exit(EXIT_FAILURE);
  }
}

// If `duration` is 0.0, return empty TimeSpan, so we can implement skip feature.
TimeSpan get_time_left(time_t start, double duration) {
  if (duration == 0.0) {
    TimeSpan dt = {.minutes = 0, .seconds = 0};
    return dt;
  }
  double elapsed = difftime(time(NULL), start);
  double seconds_left = duration - elapsed;
  int minutes = (int)seconds_left / 60;
  int seconds = (int)seconds_left % 60;
  TimeSpan dt = {.minutes = minutes, .seconds = seconds};

  return dt;
}

int play_alarm(void *path) {
  char cmd[255] = {};
  snprintf(cmd, 255, PLAYER " %s", (char *)path);
  system(cmd);

  return 0;
}

void print_status(Status status, TimeSpan *time_left) {
  char status_str[50] = {};
  snprintf(status_str, 50, "%s: %02dm %02ds left", get_status_str(status),
           time_left->minutes, time_left->seconds);
  print_centre(status_str, status);
}

void init() {
  initscr();
  raw();
  noecho();
  curs_set(0);
  timeout(1000);
}

// TODO: probably switch to integers
int main(void) {
  init();

  int ch;
  double working_block_len = WORKING_BLOCK_LENGTH_MIN * 60.0;
  double break_len = BREAK_LENGTH_MIN * 60.0;
  double long_break_len = LONG_BREAK_LENGTH_MIN * 60.0;
  double duration = working_block_len;
  unsigned working_blocks_in_session = WORKING_BLOCKS_IN_SESSION;
  time_t start = time(NULL);
  Status status = WORKING_BLOCK;
  TimeSpan time_left = get_time_left(start, duration);

  thrd_create(&thread, play_alarm, "~/.config/tato/sounds/oi.mp3");
  print_status(status, &time_left);

  refresh();

  while ((ch = getch())) {
    switch (ch) {
    case 'q':
      endwin();
      return 0;
    case 'p':
      duration += POSTPONE_LEN_MIN * 60.0;
      break;
    case 'r':
      duration = working_block_len;
      working_blocks_in_session = WORKING_BLOCKS_IN_SESSION;
      start = time(NULL);
      status = WORKING_BLOCK;
      break;
    case 's':
      duration = 0.0;
      break;
    }
    time_left = get_time_left(start, duration);
    if (time_left.minutes <= 0 && time_left.seconds <= 0) {
      if (status == WORKING_BLOCK) {
        --working_blocks_in_session;
        if (working_blocks_in_session == 0) {
          status = LONG_BREAK;
          duration = long_break_len;
          working_blocks_in_session = WORKING_BLOCKS_IN_SESSION;
          thrd_create(&thread, play_alarm,
                      "~/.config/tato/sounds/long_ding.mp3");
        } else {
          status = BREAK;
          duration = break_len;
          thrd_create(&thread, play_alarm, "~/.config/tato/sounds/ding.mp3");
        }
      } else {
        status = WORKING_BLOCK;
        duration = working_block_len;
        thrd_create(&thread, play_alarm, "~/.config/tato/sounds/oi.mp3");
      }
      start = time(NULL);
      time_left = get_time_left(start, duration);
    }
    clear();
    print_status(status, &time_left);
    refresh();
  }
}
