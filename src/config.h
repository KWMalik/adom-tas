/**
@see config.c
@author Sampsa "Tuplanolla" Kiiskinen
**/
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>//FILE
#include <time.h>//time_t

#include "util.h"//bool
#include "problem.h"//problem_t

extern char * home_path;
extern char * executable_path;
extern char * executable_data_path;
extern char * executable_temporary_path;
extern char ** executable_temporary_paths;
extern char * executable_config_path;
extern char * executable_process_path;
extern char * executable_keybind_path;
extern char * executable_version_path;
extern char * executable_error_path;
extern char * executable_count_path;
extern char * loader_path;
extern char * libc_path;
extern char * libncurses_path;
extern int states;
extern int rows;
extern int cols;
extern char * shm_path;
extern unsigned int generations;
extern time_t timestamp;
extern bool sql_emulation;
extern bool automatic_playback;
extern bool colorful_interface;
extern char * iterator;
extern char * input_path;
extern char ** output_paths;
extern FILE * error_stream;
extern FILE * warning_stream;
extern FILE * note_stream;
extern FILE * call_stream;
extern int save_key;
extern int load_key;
extern int state_key;
extern int unstate_key;
extern int duration_key;
extern int unduration_key;
extern int time_key;
extern int untime_key;
extern int menu_key;
extern int condense_key;
extern int hide_key;
extern int play_key;
extern int stop_key;
extern int quit_key;

problem_t init_external_config(void);
problem_t init_internal_config(void);
problem_t uninit_config(void);

#endif
