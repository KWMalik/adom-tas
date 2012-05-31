/**
Lists the prototypes of adom-tas.c.
**/
#ifndef __H
#define __H

/**
Lists the error codes.
**/
enum error_e {
	NO_ERROR,
	EXECUTION_ERROR,
	SETENV_HOME_ERROR,
	SETENV_LD_PRELOAD_ERROR,
	USER_ID_ERROR,
	DLOPEN_LIBC_ERROR,
	DLOPEN_LIBNCURSES_ERROR,
	STAT_ERROR,
	WRONG_VERSION_ERROR,
	WRONG_SIZE_ERROR,
	LOG_WARNING
};
typedef enum error_e error_t;

/**
Converts an error code to an error message.
@param error The error code.
@return The error message.
**/
inline char *error_message(const error_t code) {
	if (code == NO_ERROR) return "Nothing failed.";
	else if (code == SETENV_LD_PRELOAD_ERROR) return "Setting the environment variable LD_PRELOAD failed.";
	else if (code == USER_ID_ERROR) return "Linking dynamic libraries failed due to conflicting user identifiers.";
	else if (code == DLOPEN_LIBC_ERROR) return "Loading dynamically linked libraries failed.";
	else if (code == STAT_ERROR) return "Reading the executable failed.";
	else if (code == WRONG_VERSION_ERROR) return "Identifying the executable by its version failed.";
	else if (code == WRONG_SIZE_ERROR) return "Identifying the executable by its size failed.";
	else if (code == LOG_WARNING) return "Failed to open the log file. Logging to stderr (standard error stream) instead.";
	else return NULL;
}

/**
Declares the original dynamically linked library functions.
**/
typedef int (*INIT_PAIR)(short pair, short f, short b);
typedef int (*WCLEAR)(WINDOW *win);//I really want to overload all these?
typedef int (*WREFRESH)(WINDOW *win);
typedef int (*WMOVE)(WINDOW *win, int y, int x);
typedef int (*WADDCH)(WINDOW *win, chtype ch);
typedef int (*WADDNSTR)(WINDOW *win, const char *str, int n);
typedef chtype (*WINCH)(WINDOW *win);
typedef int (*WGETCH)(WINDOW *win);
typedef int (*WGETNSTR)(WINDOW *win, char *str, int n);
typedef int (*UNLINK)(const char *pathname);
typedef time_t (*TIME)(time_t *timer);
typedef int (*IOCTL)(int d, unsigned long request, ...);

#endif