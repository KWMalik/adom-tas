/**
Serves as a template.

@author Sampsa "Tuplanolla" Kiiskinen
**/
#ifndef GUI_C
#define GUI_C

#include <stdlib.h>

#include <curses.h>

#include "util.h"
#include "exec.h"
#include "shm.h"
#include "prob.h"//problem_t, *_PROBLEM
#include "rec.h"
#include "def.h"
#include "lib.h"
#include "fcn.h"
#include "log.h"
#include "cfg.h"

#include "gui.h"

/*
Modes.
*/
intern bool quitting = FALSE;
intern bool inactive = FALSE;
intern bool condensed = FALSE;
intern bool hidden = FALSE;
intern bool playing = FALSE;
intern bool paused = FALSE;
unsigned int previous_count = 0;
char * previous_codes;
frame_t * frame = NULL;
const size_t colors = sizeof interface_colors / sizeof *interface_colors;

/**
The status bar window.
**/
WINDOW * status_win = NULL;
/**
The save state menu window.
**/
WINDOW * menu_win = NULL;
/**
The save state list subwindow.
**/
WINDOW * menu_states_win = NULL;
/**
The save state preview subwindow.
**/
WINDOW * menu_chs_win = NULL;

problem_t init_interface(void) {
	/*
	Initializes the custom color pairs.
	*/
	for (size_t color = 0; color < colors; color++) {
		if (colorful_interface) {
			if (um_init_pair(pairs + color, COLOR_BLACK, interface_colors[color]) == ERR) {
				return error(INIT_PAIR_PROBLEM);
			}
		}
		else {
			if (um_init_pair(pairs + color, COLOR_WHITE, COLOR_BLACK) == ERR) {
				return error(INIT_PAIR_PROBLEM);
			}
		}
	}
	if (um_init_pair(pairs + colors, COLOR_BLACK, COLOR_WHITE) == ERR) {
		return error(INIT_PAIR_PROBLEM);
	}
	if (um_init_pair(pairs + colors + 1, COLOR_WHITE, COLOR_WHITE) == ERR) {
		return error(INIT_PAIR_PROBLEM);
	}

	/*
	Creates the windows.
	*/
	status_win = newwin(1, cols, rows - 1, 0);
	if (status_win == NULL) {
		return error(NEWWIN_PROBLEM);
	}
	const int nlines = rows / 2;
	const int ncols = cols / 2;
	const int begin_y = rows / 4 - 1;
	const int begin_x = cols / 4;
	menu_win = newwin(1 + nlines + 1, 1 + ncols + 1, begin_y - 1, begin_x - 1);
	if (menu_win == NULL) {
		return error(NEWWIN_PROBLEM);
	}
	menu_states_win = newwin(3, 1 + ncols + 1, begin_y + nlines, begin_x - 1);
	if (menu_states_win == NULL) {
		return error(NEWWIN_PROBLEM);
	}
	menu_chs_win = newwin(nlines, ncols, begin_y, begin_x);
	if (menu_chs_win == NULL) {
		return error(NEWWIN_PROBLEM);
	}

	return NO_PROBLEM;
}

problem_t uninit_interface(void) {
	delwin(status_win);
	delwin(menu_win);
	delwin(menu_chs_win);
	delwin(menu_states_win);

	return NO_PROBLEM;
}

/**
Draws the status bar in the bottom of a window from right to left.

@param win The window.
**/
problem_t draw_status(void) {
	wclear(status_win);
	const size_t size = cols + 1;
	char * const str = malloc(size);
	if (str == NULL) {
		return error(MALLOC_PROBLEM);
	}
	int left_pos = 0;
	short int pair = 0;
	#define draw_status_ADDSTR(format, __GVA_ARGS__...) do {\
			snprintf(str, size, format, ##__GVA_ARGS__);\
			wattrset(status_win, COLOR_PAIR(pairs + pair));\
			pair = (pair + 1) % colors;\
			mvwaddstr(status_win, 0, left_pos, str);\
			left_pos += strlen(str) + 1;\
			if (!colorful_interface) {\
				left_pos++;\
			}\
		} while (0)
	draw_status_ADDSTR("Status");
	//draw_status_ADDSTR("P: %u", (unsigned int )getpid());
	draw_status_ADDSTR("I: %s", previous_inputs);
	draw_status_ADDSTR("F: %u/%u", record.count - previous_count, record.count);
	draw_status_ADDSTR("T: 0/%u", (unsigned int )(*executable_turns + surplus_turns));
	const unsigned int durr = (unsigned int )dur + 1;
	if (durr < frame_rate) {
		draw_status_ADDSTR("D: 1/%u", frame_rate / durr);
	}
	else {
		draw_status_ADDSTR("D: %u", durr / frame_rate);
	}
	draw_status_ADDSTR("E: %u/%u", (unsigned int )(timestamp - record.timestamp), (unsigned int )timestamp);
	draw_status_ADDSTR("R: 0x%08x", (unsigned int )hash(executable_arc4_s, 0x100));
	draw_status_ADDSTR("S: %u/%u", current_state, states - 1);
	free(str);

	return NO_PROBLEM;
}

/**
Draws the save state menu in the middle of a window.

@param win The window.
**/
problem_t draw_menu(void) {
	const chtype attr = COLOR_PAIR(pairs + colors);
	const chtype eattr = COLOR_PAIR(pairs + colors + 1);//TODO improve
	const chtype ch = attr | ' ';
	const chtype lrs = eattr | '|';
	const chtype tbs = eattr | '-';
	const chtype tblr = eattr | '+';

	/*
	Draws the menu window.
	*/
	wclear(menu_win);
	wbkgd(menu_win, ch);
	wborder(menu_win, lrs, lrs, tbs, tbs, tblr, tblr, tblr, tblr);

	int y, x;

	/*
	Draws the list subwindow.
	*/
	wclear(menu_states_win);
	wbkgd(menu_states_win, ch);
	wborder(menu_states_win, lrs, lrs, tbs, tbs, tblr, tblr, tblr, tblr);

	getmaxyx(menu_states_win, y, x);
	int row = 1;
	int left_edge = 0;
	int right_edge = x - 1;
	left_edge += strlen(interface_left_more) + 1;
	right_edge -= strlen(interface_right_more) + 1;
	int left_pos;
	int right_pos;
	bool left_end = FALSE;
	bool right_end = FALSE;
	bool left_more = FALSE;
	bool right_more = FALSE;
	int diff = 0;
	int state = current_state;
	do {
		const char * interface_left = "";
		const char * interface_right = "";
		if (state > 0 && state < states) {
			if (shm.pids[state] == 0) {
				interface_left = interface_left_unused;
				interface_right = interface_right_unused;
			}
			else {
				interface_left = interface_left_used;
				interface_right = interface_right_used;
			}
		}
		const size_t len = strlen(interface_left)
				+ intlen(state)
				+ strlen(interface_right);
		int col = -1;
		if (diff == 0) {//move to the center
			diff++;//determines the direction
			left_pos = left_edge + (right_edge - left_edge - len - 1) / 2 + 1;
			col = left_pos;
			right_pos = left_pos + len - 1;
		}
		else if (diff > 0) {//move left
			if (!left_more) {
				left_pos -= len;
				left_pos--;//spacing
				col = left_pos;
				if (left_pos <= left_edge) {
					if (state > 0) {
						left_more = TRUE;
					}
					left_end = TRUE;
					col = -1;
				}
			}
			if (state <= 0) {
				left_end = TRUE;
				col = -1;
			}
		}
		else {//move right
			if (!right_more) {
				right_pos++;//spacing
				col = right_pos + 1;
				right_pos += len;
				if (right_pos >= right_edge) {
					if (state < states) {
						right_more = TRUE;
					}
					right_end = TRUE;
					col = -1;
				}
			}
			if (state >= states) {
				right_end = TRUE;
				col = -1;
			}
		}
		if (col != -1) {
			const size_t size = len + 1;
			char * const buf = malloc(size);
			if (buf == NULL) {
				error(MALLOC_PROBLEM);
			}
			else {
				snprintf(buf, size, "%s%u%s",
						interface_left,
						state,
						interface_right);
				mvwaddstr(menu_states_win, row, col, buf);
				free(buf);
			}
		}
		state += diff;
		diff = - (diff + SGN(diff));
	} while (!(left_end && right_end));
	if (left_more) {
		mvwaddstr(menu_states_win, row, 1, interface_left_more);
	}
	if (right_more) {
		const size_t size = strlen(interface_right_more) + 1;
		mvwaddstr(menu_states_win, row, x - size, interface_right_more);
	}

	/*
	Draws the preview subwindow.
	*/
	wclear(menu_chs_win);
	getmaxyx(menu_chs_win, y, x);
	for (unsigned int row = 0; row < rows; row++) {
		for (unsigned int col = 0; col < cols; col++) {
			mvwaddch(menu_chs_win, row / 2, col / 2, shm.chs[current_state][row][col]);
		}
	}

	return NO_PROBLEM;
}

problem_t draw_interface(void) {
	draw_status();
	um_wrefresh(status_win);
	if (inactive) {
		draw_menu();
		um_wrefresh(menu_win);
		um_wrefresh(menu_chs_win);
		um_wrefresh(menu_states_win);
	}

	return NO_PROBLEM;
}

#endif
