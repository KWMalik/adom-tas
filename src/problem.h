/**
Handles errors.

@author Sampsa "Tuplanolla" Kiiskinen
**/
#ifndef PROBLEM_H
#define PROBLEM_H

/**
Lists the error codes.
**/
enum problem_e {
	NO_PROBLEM,
	NULL_PROBLEM,
	MALLOC_PROBLEM,

	//main.c
	TEMPORARY_UNLINK_PROBLEM,
	PROCESS_UNLINK_PROBLEM,
	VERSION_OPEN_PROBLEM,
	VERSION_READ_PROBLEM,
	VERSION_CLOSE_PROBLEM,
	VERSION_MISMATCH_PROBLEM,
	COUNT_OPEN_PROBLEM,
	COUNT_WRITE_PROBLEM,
	COUNT_CLOSE_PROBLEM,
	LD_PRELOAD_SETENV_PROBLEM,
	EXEC_PROBLEM,

	//config.c
	CONFIG_OPEN_PROBLEM,
	CONFIG_WRITE_PROBLEM,
	CONFIG_CLOSE_PROBLEM,
	CONFIG_STAT_PROBLEM,
	CONFIG_PARSE_PROBLEM,
	HOME_CONFIG_PROBLEM,
	HOME_GETENV_PROBLEM,
	HOME_GETPWUID_PROBLEM,
	HOME_STAT_PROBLEM,
	EXECUTABLE_DATA_CONFIG_PROBLEM,
	EXECUTABLE_DATA_HOME_PROBLEM,
	EXECUTABLE_DATA_STAT_PROBLEM,
	EXECUTABLE_TEMPORARY_STAT_PROBLEM ,

	LD_PRELOAD_CONFIG_PROBLEM,
	LD_PRELOAD_GETENV_PROBLEM,
	LD_PRELOAD_STAT_PROBLEM,
	EXECUTABLE_CONFIG_PROBLEM,
	EXECUTABLE_STAT_PROBLEM,
	EXECUTABLE_TYPE_PROBLEM,
	EXECUTABLE_PERMISSION_PROBLEM,
	EXECUTABLE_SIZE_PROBLEM,
	EXECUTABLE_OPEN_PROBLEM,
	EXECUTABLE_READ_PROBLEM,
	EXECUTABLE_HASH_PROBLEM,
	EXECUTABLE_CLOSE_PROBLEM,
	EXECUTABLE_CONFIG_STAT_PROBLEM,
	EXECUTABLE_PROCESS_STAT_PROBLEM,
	EXECUTABLE_KEYBIND_STAT_PROBLEM,
	EXECUTABLE_VERSION_STAT_PROBLEM,
	EXECUTABLE_COUNT_STAT_PROBLEM,

	LIBC_CONFIG_PROBLEM,
	LIBC_STAT_PROBLEM,
	LIBNCURSES_CONFIG_PROBLEM,
	LIBNCURSES_STAT_PROBLEM,
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
	AUTOPLAY_CONFIG_PROBLEM,
	COLOR_CONFIG_PROBLEM,
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

	//loader.c
	LIBC_DLOPEN_PROBLEM,
	LIBNCURSES_DLOPEN_PROBLEM,
	LD_PRELOAD_UNSETENV_PROBLEM,

	//shm.c
	SHM_MALLOC_PROBLEM,
	SHM_KEY_PROBLEM,
	SHM_GET_PROBLEM,
	SHM_ATTACH_PROBLEM,
	SHM_DETACH_PROBLEM,
	SHM_REMOVE_PROBLEM,

	//put.c
	INPUT_OPEN_PROBLEM,
	INPUT_READ_PROBLEM,
	INPUT_CLOSE_PROBLEM,
	OUTPUT_OPEN_PROBLEM,
	OUTPUT_WRITE_PROBLEM,
	OUTPUT_CLOSE_PROBLEM,

	//fork.c
	FORK_PROBLEM,

	//asm.c
	ASM_MEMCMP_PROBLEM,
	ASM_MPROTECT_PROBLEM
};
typedef enum problem_e problem_t;

const char * problem_message(problem_t code);

/**
Propagates an error code.

Returns the error code of an error if one occurs:
<pre>
problem_t method {
	PROPAGATE(another_method());
	yet_another_method();
	return NO_PROBLEM;
}
</pre>

@param code The error code.
**/
#undef PROPAGATE
#define PROPAGATE(code) do {\
		const problem_t PROPAGATE_code = code;\
		if (PROPAGATE_code != NO_PROBLEM) return PROPAGATE_code;\
	} while (FALSE)

#endif
