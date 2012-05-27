/**
This is a mess, but works, which is enough for the first build.
**/

/*
Half of these are unnecessary.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curses.h>
#include <libgen.h>
#include <getopt.h>
#include <dlfcn.h>

/**
Lists boolean types.
Apparently some other library already does this.
**/
/*
enum bool_e {
	FALSE,
	TRUE,
};
typedef enum bool_e bool;
*/

/**
Lists error codes.
**/
enum error_e {
	NO_ERROR,
	SETENV_ERROR,
	USER_ID_ERROR,
	DLOPEN_ERROR,
	STAT_ERROR,
	WRONG_VERSION_ERROR,
	WRONG_SIZE_ERROR,
};
typedef enum error_e error_t;

/**
Converts an error code to an error message.
@param error The error code.
@return The error message.
**/
const char *error_message(const error_t code) {
	if (code == NO_ERROR) return "Nothing failed.";
	else if (code == SETENV_ERROR) return "Setting the environment variable LD_PRELOAD failed.";
	else if (code == USER_ID_ERROR) return "Linking dynamic libraries failed due to conflicting user identifiers.";
	else if (code == DLOPEN_ERROR) return "Loading dynamically linked libraries failed.";
	else if (code == STAT_ERROR) return "Reading the executable failed.";
	else if (code == WRONG_VERSION_ERROR) return "Identifying the executable by its version failed.";
	else if (code == WRONG_SIZE_ERROR) return "Identifying the executable by its size failed.";
	else return NULL;
}

/**
Prints an error message and forwards its error code.
@param error The error code.
@return The error code.
**/
const error_t propagate(const error_t code) {
	fprintf(stderr, "Error: %s\n", error_message(code));
	return code;
}

/**
Logs a message.
@param message The message.
@return TRUE if the message was logged successfully and FALSE otherwise.
**/
char *log_file = "/home/tuplanolla/adom-tas.log";
const bool printl(const char *message) {
	if (log_file == NULL) fprintf(stderr, "LOG: %s\n", message);//TODO add a file_exists check
	else {//TODO write to a file
		FILE *handle = fopen(log_file, "a");
		fwrite(message, 1, strlen(message), handle);
		fclose(handle);
	}
	return TRUE;
}

/**
Serves as an annotation for overloads.
**/
#define OVERLOAD(x) x

/**
Defines the default locations of dynamically linked libraries.
**/
#define LIBC "/lib/i686/cmov/libc.so.6"
#define LIBNCURSES "/lib/libncurses.so.5"

/**
Declares the original dynamically linked library functions.
**/
typedef int (*INIT_PAIR)(short pair, short f, short b);
typedef int (*WCLEAR)(WINDOW *win);
typedef int (*WREFRESH)(WINDOW *win);
typedef int (*WMOVE)(WINDOW *win, int y, int x);
typedef int (*WADDCH)(WINDOW *win, chtype ch);
typedef int (*WADDNSTR)(WINDOW *win, const char *str, int n);
typedef chtype (*WINCH)(WINDOW *win);
typedef int (*WGETCH)(WINDOW *win);
typedef int (*WGETNSTR)(WINDOW *win, char *str, int n);
typedef int (*SPRINTF)(char *str, const char *format, ...);
typedef int (*VSPRINTF)(char *str, const char *format, va_list ap);
typedef int (*UNLINK)(const char *pathname);
typedef time_t (*TIME)(time_t *timer);
//typedef void (*GETMAXYX)(WINDOW *win, int y, int x);

INIT_PAIR real_init_pair;
WCLEAR real_wclear;
WREFRESH real_wrefresh;
WMOVE real_wmove;
WADDCH real_waddch;
WADDNSTR real_waddnstr;
WINCH real_winch;
WGETCH real_wgetch;
WGETNSTR real_wgetnstr;
SPRINTF real_sprintf;
VSPRINTF real_vsprintf;
UNLINK real_unlink;
TIME real_time;
//GETMAXYX real_getmaxyx;

/**
Loads functions from dynamically linked libraries (libc and libncurses).
**/
void load_dynamic_libraries() {
	char *libc_path;
	char *curses_path;

	/*
	Extracts library paths from environment variables.
	*/
	libc_path = getenv("LIBC_PATH");
	if (libc_path == NULL) libc_path = LIBC;
	curses_path = getenv("CURSES_PATH");
	if (curses_path == NULL) curses_path = LIBNCURSES;

	void *handle;

	/*
	Imports functions from libc.
	*/
	handle = (void *)dlopen(libc_path, RTLD_LAZY);//requires either RTLD_LAZY or RTLD_NOW
	if (handle == NULL) propagate(DLOPEN_ERROR);
	real_sprintf = (SPRINTF )dlsym(handle, "sprintf");
	real_vsprintf = (VSPRINTF )dlsym(handle, "vsprintf");
	real_unlink = (UNLINK )dlsym(handle, "unlink");
	real_time = (TIME )dlsym(handle, "time");

	/*
	Imports functions from libncurses.
	*/
	handle = (void *)dlopen(curses_path, RTLD_LAZY);
	if (handle == NULL) propagate(DLOPEN_ERROR);
	real_init_pair = (INIT_PAIR )dlsym(handle, "init_pair");
	real_wclear = (WCLEAR )dlsym(handle, "wclear");
	real_wrefresh = (WREFRESH )dlsym(handle, "wrefresh");
	real_wmove = (WMOVE )dlsym(handle, "wmove");
	real_waddch = (WADDCH )dlsym(handle, "waddch");
	real_waddnstr = (WADDNSTR )dlsym(handle, "waddnstr");
	real_winch = (WINCH )dlsym(handle, "winch");
	real_wgetch = (WGETCH )dlsym(handle, "wgetch");
	real_wgetnstr = (WGETNSTR )dlsym(handle, "wgetnstr");
	//real_getmaxyx = (GETMAXYX )dlsym(handle, "getmaxyx");

	/*
	Prevents reloading libraries for child processes.
	*/
	unsetenv("LD_PRELOAD");
}

/**
Guarantees a single call to load the dynamically linked libraries.
**/
bool initialized = FALSE;
void initialize() {
	if (initialized) return;
	initialized = TRUE;

	load_dynamic_libraries();
}

/**
Overloads wgetch with a simple log wrapper.
**/
int OVERLOAD(wgetch)(WINDOW *win) {
	printl("Called wgetch.\n");
	return real_wgetch(win);
}

/**
Overloads time with a simple log wrapper.
**/
time_t OVERLOAD(time)(time_t *timer) {
	initialize();
	printl("Called time.\n");
	return (time_t )0;
}

#define HAND_PICKED_WIZARDRY "/home/tuplanolla/adom-tas/adom"
#define HAND_PICKED_SORCERY "/home/tuplanolla/adom-tas/src/adom-tas.so"

/**
Tests the class.
@param argc The amount of command line arguments.
@param argv The command line arguments.
@return 0 if successful and an error number otherwise.
**/
const int main(const int argc, const char **argv) {
	/*
	Manages the environment variable LD_PRELOAD.
	*/
	const char *preload = getenv("LD_PRELOAD");
	if (preload == NULL) {
		if (setenv("LD_PRELOAD", HAND_PICKED_SORCERY, TRUE)) {
			return propagate(SETENV_ERROR);
		}
	}

	/*
	Conjures bacon.
	*/
	int result;
	struct stat file;
	result = stat(HAND_PICKED_WIZARDRY, &file);
	if (result != 0) return propagate(STAT_ERROR);
	if (file.st_mode & (S_ISUID | S_ISGID)) {
		return propagate(USER_ID_ERROR);
	}

	/*
	Uses file system heuristics to identify the executable.
	*/
	if (FALSE) return propagate(WRONG_VERSION_ERROR);//TODO if ver file in ADOM_DATA exists it contains 01 01 01 00
	if (file.st_size != 2452608) return propagate(WRONG_SIZE_ERROR);

	/*
	Launches the executable.
	*/
	const char *adom_argv = argv+1;//TODO explain
	const int adom_argc = argc-1;
	execvp(HAND_PICKED_WIZARDRY, adom_argv);//execute with varargs including PATH

	return 0;
}

