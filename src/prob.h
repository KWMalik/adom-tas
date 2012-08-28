/**
@see prob.c
@author Sampsa "Tuplanolla" Kiiskinen
**/
#ifndef PROB_H
#define PROB_H

/**
Propagates an error code.

@param code The error code.
@deprecated
**/
#define PROPAGATE(code) do {\
const problem_d PROPAGATE_problem = code;\
if (PROPAGATE_problem != NO_PROBLEM) {\
return PROPAGATE_problem;\
}\
} while (0)
#define PROPAGATEC(code, function) do {\
const problem_d PROPAGATEC_problem = code;\
if (PROPAGATEC_problem != NO_PROBLEM) {\
function(PROPAGATEC_problem);\
}\
} while (0)
#define PROPAGATEA(code, variable) do {\
const problem_d PROPAGATEC_problem = code;\
if (PROPAGATEC_problem != NO_PROBLEM) {\
variable = PROPAGATEC_problem;\
}\
} while (0)

/**
Lists the error codes.
**/
enum problem_e {
	/*
	*.c
	*/
	NO_PROBLEM,
	ASSERT_PROBLEM,
	NULL_PROBLEM,
	MALLOC_PROBLEM,

	/*
	main.c
	*/
	TEMPORARY_UNLINK_PROBLEM,
	CONFIG_OPEN_PROBLEM,
	CONFIG_WRITE_PROBLEM,
	CONFIG_CLOSE_PROBLEM,
	PROCESS_UNLINK_PROBLEM,
	KEYBIND_OPEN_PROBLEM,
	KEYBIND_WRITE_PROBLEM,
	KEYBIND_CLOSE_PROBLEM,
	VERSION_OPEN_PROBLEM,
	VERSION_READ_PROBLEM,
	VERSION_CLOSE_PROBLEM,
	VERSION_MISMATCH_PROBLEM,
	ERROR_UNLINK_PROBLEM,
	COUNT_OPEN_PROBLEM,
	COUNT_WRITE_PROBLEM,
	COUNT_CLOSE_PROBLEM,
	LD_PRELOAD_SETENV_PROBLEM,
	EXEC_PROBLEM,

	/*
	cfg.c
	*/
	CONFIGURATION_OPEN_PROBLEM,
	CONFIGURATION_WRITE_PROBLEM,
	CONFIGURATION_CLOSE_PROBLEM,
	CONFIGURATION_STAT_PROBLEM,
	CONFIGURATION_PARSE_PROBLEM,
	HOME_CONFIG_PROBLEM,
	HOME_GETENV_PROBLEM,
	HOME_GETPWUID_PROBLEM,
	HOME_STAT_PROBLEM,
	EXEC_DATA_CONFIG_PROBLEM,
	EXEC_DATA_HOME_PROBLEM,
	EXEC_DATA_STAT_PROBLEM,
	EXEC_TEMPORARY_STAT_PROBLEM,

	LD_PRELOAD_CONFIG_PROBLEM,
	LD_PRELOAD_GETENV_PROBLEM,
	LD_PRELOAD_STAT_PROBLEM,
	EXEC_CONFIG_PROBLEM,
	EXEC_STAT_PROBLEM,
	EXEC_TYPE_PROBLEM,
	EXEC_PERMISSION_PROBLEM,
	EXEC_SIZE_PROBLEM,
	EXEC_OPEN_PROBLEM,
	EXEC_READ_PROBLEM,
	EXEC_HASH_PROBLEM,
	EXEC_CLOSE_PROBLEM,
	EXEC_CONFIG_STAT_PROBLEM,
	EXEC_PROCESS_STAT_PROBLEM,
	EXEC_KEYBIND_STAT_PROBLEM,
	EXEC_VERSION_STAT_PROBLEM,
	EXEC_ERROR_STAT_PROBLEM,
	EXEC_COUNT_STAT_PROBLEM,

	LIBC_CONFIG_PROBLEM,
	LIBC_STAT_PROBLEM,
	LIBNCURSES_CONFIG_PROBLEM,
	LIBNCURSES_STAT_PROBLEM,
	ENFORCE_CONFIG_PROBLEM,
	ROW_CONFIG_PROBLEM,
	ROW_AMOUNT_PROBLEM,
	COL_CONFIG_PROBLEM,
	COL_AMOUNT_PROBLEM,
	STATE_CONFIG_PROBLEM,
	STATE_AMOUNT_PROBLEM,
	SHM_CONFIG_PROBLEM,
	SHM_STAT_PROBLEM,
	GENERATIONS_CONFIG_PROBLEM,
	SQL_CONFIG_PROBLEM,
	PRESERVE_CONFIG_PROBLEM,
	AUTOPLAY_CONFIG_PROBLEM,
	MONOCHROME_CONFIG_PROBLEM,
	ITERATOR_CONFIG_PROBLEM,
	INPUT_CONFIG_PROBLEM,
	INPUT_STAT_PROBLEM,
	OUTPUT_CONFIG_PROBLEM,
	OUTPUT_STAT_PROBLEM,
	ERROR_CONFIG_PROBLEM,
	ERROR_STAT_PROBLEM,
	ERROR_OPEN_PROBLEM,
	ERROR_CLOSE_PROBLEM,
	WARNING_CONFIG_PROBLEM,
	WARNING_STAT_PROBLEM,
	WARNING_OPEN_PROBLEM,
	WARNING_CLOSE_PROBLEM,
	NOTE_CONFIG_PROBLEM,
	NOTE_STAT_PROBLEM,
	NOTE_OPEN_PROBLEM,
	NOTE_CLOSE_PROBLEM,
	CALL_CONFIG_PROBLEM,
	CALL_STAT_PROBLEM,
	CALL_OPEN_PROBLEM,
	CALL_CLOSE_PROBLEM,
	LOG_CHANGE_PROBLEM,

	/*
	lib.c
	*/
	LIBC_DLOPEN_PROBLEM,
	LIBC_DLSYM_PROBLEM,
	LIBNCURSES_DLOPEN_PROBLEM,
	LIBNCURSES_DLSYM_PROBLEM,
	LD_PRELOAD_UNSETENV_PROBLEM,
	LIBC_DLCLOSE_PROBLEM,
	LIBNCURSES_DLCLOSE_PROBLEM,

	/*
	shm.c
	*/
	SHM_MALLOC_PROBLEM,
	SHM_KEY_PROBLEM,
	SHM_EXCL_PROBLEM,
	SHM_GET_PROBLEM,
	SHM_ATTACH_PROBLEM,
	SHM_DETACH_PROBLEM,
	SHM_REMOVE_PROBLEM,

	/*
	put.c
	*/
	INPUT_OPEN_PROBLEM,
	INPUT_READ_PROBLEM,
	INPUT_FORMAT_PROBLEM,
	INPUT_FRAME_PROBLEM,
	INPUT_CLOSE_PROBLEM,
	OUTPUT_OPEN_PROBLEM,
	OUTPUT_WRITE_PROBLEM,
	OUTPUT_CLOSE_PROBLEM,

	/*
	fork.c
	*/
	FORK_PROBLEM,

	/*
	gui.c
	*/
	INIT_PAIR_PROBLEM,
	NEWWIN_PROBLEM,
	DELWIN_PROBLEM,

	/*
	asm.c
	*/
	ASM_MPROTECT_PROBLEM,
	ASM_MEMCMP_PROBLEM
};
typedef enum problem_e problem_d;

extern int probno;

const char * problem_message(problem_d code);

#endif
