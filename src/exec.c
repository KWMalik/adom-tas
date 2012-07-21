/**
Provides information about
	the files,
	the memory and
	the behavior
		of the executable and
emulates
	the random number generator and
	the key number interpreter
		of the executable.

@author Sampsa "Tuplanolla" Kiiskinen
**/
#ifndef EXEC_C
#define EXEC_C

#include <stdlib.h>//*random, size_t
#include <stdio.h>//*print*
#include <string.h>//str*, mem*
#include <limits.h>//CHAR_BIT

#include <curses.h>//KEY_*

#include "util.h"//intern, SWAP

#include "exec.h"

/**
The amount of color pairs initialized (including erroneously initialized pairs).

The value is set by <code>init_pair</code>.
**/
intern short pairs = 31;

/**
The actual turn count without negative turns.

The value is set by <code>wgetch</code>.
**/
intern int turns = 0;

/**
The size of the executable.
**/
intern const size_t executable_size = 2452608;

/**
The hash code of the executable.
**/
intern const int executable_hash = 893530599;

/**
The version of the executable.
**/
intern const unsigned char executable_version[4] = {1, 1, 1, 0};

/**
The minimum height of the terminal.
**/
intern const int executable_rows_min = 25;

/**
The maximum height of the terminal.
**/
intern const int executable_cols_min = 77;

/**
The minimum width of the terminal.
**/
intern const int executable_rows_max = 127;
/**
The maximum width of the terminal.
**/
intern const int executable_cols_max = 127;

/**
The name of the data directory.
**/
intern const char * const executable_data_directory = ".adom.data";

/**
The name of the temporary file directory.
**/
intern const char * const executable_temporary_directory = "tmpdat";

/**
The name prefix of temporary files.
**/
intern const char * const executable_temporary_file = "adom_tdl";

/**
The amount of temporary file groups.
**/
intern const unsigned int executable_temporary_levels = 51;

/**
The amount of temporary files in groups.
**/
intern const unsigned int executable_temporary_parts = 4;

/**
The name of the configuration file.
**/
intern const char * const executable_config_file = ".adom.cfg";

/**
The name of the process lock file.
**/
intern const char * const executable_process_file = ".adom.prc";

/**
The name of the keybinding file.
**/
intern const char * const executable_keybind_file = ".adom.kbd";

/**
The name of the version file.
**/
intern const char * const executable_version_file = ".adom.ver";

/**
The name of the error file.
**/
intern const char * const executable_error_file = ".adom.err";

/**
The name of the count file.
**/
intern const char * const executable_count_file = ".adom.cnt";

/**
The default configuration.
**/
intern const char * const executable_config =
		"Allow_Default_Names    = false\n"
		"Auto_Lock_Doors        = false\n"
		"Auto_Lock_Nearest      = true\n"
		"Auto_Open_Doors        = true\n"
		"Auto_Pickup            = ['~(}/]{=\\!?\"%$*\n"
		"Auto_Select_Doors      = true\n"
		"Base_Delay_Factor      = 1\n"
		"Check_Item_In_Hand     = true\n"
		"Colored_Menus          = true\n"
		"Colored_PC             = true\n"
		"Fast_Missile_Readying  = true\n"
		"Fast_More              = true\n"
		"Item_Status_Color      = cyan\n"
		"Low_Hitpoint_Warning   = true\n"
		"Message_Buffer_Size    = 10000\n"
		"Metric_Measuring       = true\n"
		"Nice_Stuff             = true\n"
		"No_Book_Spellcasting   = false\n"
		"Persistence            = 10000\n"
		"Questioned_Attributes  = true\n"
		"Reload_Missiles        = false\n"
		"Reverse_Message_Order  = true\n"
		"Show_Experience        = true\n"
		"Sorted_Skills          = true\n"
		"Starvation_Warning     = true\n"
		"Uncursed_String        = mundane\n"
		"Verbose_Level_Messages = false\n"
		"Walk_Carefully         = true\n"
		"Zap_Wands_In_Hand      = true\n"
	;

/**
The default keybindings.
**/
intern const char * const executable_keybind =
		"#Activate trap\n"
		"ACT:\\Ct\n\n"
		"#Apply skill\n"
		"ASK:a\n\n"
		"#Ascend stairway/Leave location\n"
		"ASC:<\n\n"
		"#Cast spell\n"
		"CST:Z\n\n"
		"#Chat with monsters\n"
		"TLK:C\n\n"
		"#Change highlight mode\n"
		"HIL:H\n\n"
		"#Change tactic\n"
		"TAC:T\n\n"
		"#Check literacy\n"
		"LIT:L\n\n"
		"#Clean ears\n"
		"CLE:E\n\n"
		"#Close door\n"
		"CLO:c\n\n"
		"#Continuous search\n"
		"CSE:ws\n\n"
		"#Create short character logfile\n"
		"CSL:(\n\n"
		"#Create verbose character logfile\n"
		"CVL:)\n\n"
		"#Create screenshot\n"
		"CSS:[\n\n"
		"#Descend stairway/Enter location\n"
		"DSC:>\n\n"
		"#Describe weather\n"
		"DEW::W\n\n"
		"#Dip (something) into (potion)\n"
		"DIP:!\n\n"
		"#Display available talents\n"
		"DAT::T\n\n"
		"#Display and quick-mark skills\n"
		"DSK:A\n\n"
		"#Display background\n"
		"BKG:B\n\n"
		"#Display bill\n"
		"BIL:P\n\n"
		"#Display burden levels\n"
		"DBL::b\n\n"
		"#Display configuration variables\n"
		"CFG:=\n\n"
		"#Display chaos powers\n"
		"CHP:\\\\\n\n"
		"#Display character information\n"
		"DCI:\\Mq\n\n"
		"#Display companions\n"
		"DCO::c\n\n"
		"#Display current wealth\n"
		"MNY:$\n\n"
		"#Display name of your deity\n"
		"DID::g\n\n"
		"#Display elapsed game time\n"
		"DET:\\Ce\n\n"
		"#Display identified items\n"
		"IDI:/\n\n"
		"#Display kick statistics\n"
		"KST:K\n\n"
		"#Display killed monsters\n"
		"DKM::k\n\n"
		"#Display level map\n"
		"DLM:\\Ml\n\n"
		"#Display message buffer\n"
		"MSG::m\n\n"
		"#Display missile statistics\n"
		"DMS:M\n\n"
		"#Display monster wound status\n"
		"MWS::w\n\n"
		"#Display quest status\n"
		"QST:q\n\n"
		"#Display recipes\n"
		"RCP:R\n\n"
		"#Display required experience\n"
		"EXP:x\n\n"
		"#Display talents\n"
		"DTL:\\Mt\n\n"
		"#Display version\n"
		"VER:V\n\n"
		"#Display weapon skills\n"
		"DWS:\\Cw\n\n"
		"#Display weapon statistics\n"
		"WST:W\n\n"
		"#Drink\n"
		"DRK:D\n\n"
		"#Drop item(s)\n"
		"DRO:d\n\n"
		"#Drop items in a comfortable way\n"
		"DRC:\\Cd\n\n"
		"#Eat\n"
		"EAT:e\n\n"
		"#Examine environment\n"
		"EXE:l\n\n"
		"#Extended drop\n"
		"EDR::d\n\n"
		"#Extended pay\n"
		"EPA::p\n\n"
		"#Extended use\n"
		"EUS::u\n\n"
		"#Give item to monster\n"
		"GIV:g\n\n"
		"#Handle something\n"
		"HDL:h\n\n"
		"#Inventory\n"
		"INV:i\n\n"
		"#Invoke mindcraft\n"
		"INM:\\Ci\n\n"
		"#Issue order to companion\n"
		"ISO:\\Co\n\n"
		"#Kick\n"
		"KCK:k\n\n"
		"#Look\n"
		"LOK:\\Cl\n\n"
		"#Mark spells\n"
		"MSP::Z\n\n"
		"#Miscellaneous equipment\n"
		"STF:I\n\n"
		"#Move to the southwest\n"
		"MSW:1\n\n"
		"#Move to the south\n"
		"MOS:2 \\D\n\n"
		"#Move to the southeast\n"
		"MSE:3\n\n"
		"#Move to the west\n"
		"MOW:4 \\L\n\n"
		"#Move to the east\n"
		"MOE:6 \\R\n\n"
		"#Move to the northwest\n"
		"MNW:7\n\n"
		"#Move to the north\n"
		"MON:8 \\U\n\n"
		"#Move to the northeast\n"
		"MNE:9\n\n"
		"#Name monster/yourself\n"
		"BAP:n\n\n"
		"#Online help\n"
		"HLP:?\n\n"
		"#Open door\n"
		"OPN:o\n\n"
		"#Pay\n"
		"PAY:p\n\n"
		"#Pick up items primitively (fast)\n"
		"PPK:;\n\n"
		"#Pick up items\n"
		"PCK:,\n\n"
		"#Pick up items comfortably\n"
		"CPC:\\Cp\n\n"
		"#Pray\n"
		"PRA:_\n\n"
		"#Quit game\n"
		"QIT:Q\n\n"
		"#Read\n"
		"REA:r\n\n"
		"#Recall monster memory\n"
		"RMM:&\n\n"
		"#Redraw screen\n"
		"RED:\\Cr\n\n"
		"#Sacrifice\n"
		"SAC:O\n\n"
		"#Save and quit game\n"
		"SAV:S\n\n"
		"#Search\n"
		"SEA:s\n\n"
		"#Set tactics to 'berserker'\n"
		"ST0:\\1\n\n"
		"#Set tactics to 'very aggressive'\n"
		"ST1:\\2\n\n"
		"#Set tactics to 'aggressive'\n"
		"ST2:\\3\n\n"
		"#Set tactics to 'normal'\n"
		"ST3:\\4\n\n"
		"#Set tactics to 'defensive'\n"
		"ST4:\\5\n\n"
		"#Set tactics to 'very defensive'\n"
		"ST5:\\6\n\n"
		"#Set tactics to 'coward'\n"
		"ST6:\\7\n\n"
		"#Set variable\n"
		"SEV::=\n\n"
		"#Shoot/Throw missile\n"
		"SHT:t\n\n"
		"#Swap position with monster\n"
		"SWA::s\n\n"
		"#Switch auto-pickup on/off\n"
		"CAP:\\Ca\n\n"
		"#Switch the dynamic display\n"
		"STS::t\n\n"
		"#Switch the required (more) key\n"
		"CMK:\\Ck\n\n"
		"#Unlock door\n"
		"ULD:\\Cu\n\n"
		"#Use item\n"
		"USE:U\n\n"
		"#Use class power\n"
		"UMP:\\Cx\n\n"
		"#Use special ability\n"
		"SPA:m\n\n"
		"#Use tool\n"
		"UTO:u\n\n"
		"#Wait\n"
		"WAT:. 5\n\n"
		"#Walk to the southwest\n"
		"WM1:w1\n\n"
		"#Walk to the south\n"
		"WM2:w2\n\n"
		"#Walk to the southeast\n"
		"WM3:w3\n\n"
		"#Walk to the west\n"
		"WM4:w4\n\n"
		"#Walk on the spot\n"
		"WM5:w5\n\n"
		"#Walk to the east\n"
		"WM6:w6\n\n"
		"#Walk to the northwest\n"
		"WM7:w7\n\n"
		"#Walk to the north\n"
		"WM8:w8\n\n"
		"#Walk to the northeast\n"
		"WM9:w9\n\n"
		"#Wipe face\n"
		"WIF:F\n\n"
		"#Zap wand\n"
		"ZAP:z\n"
	;

/**
The unique parts of the questions.
**/
intern const char * const executable_question_strings[51] = {
	"Your father w",
	"In y",
	"You an",
	"In c",
	"Your g",
	"When e",
	"You are o",
	"Your f",
	"While c",
	"You have b",
	"At",
	"While a",
	"While p",
	"S",
	"After s",
	"H",
	"The ",
	"After e",
	"E",
	"N",
	"As y",
	"During y",
	"While d",
	"During a",
	"While o",
	"If",
	"Your b",
	"You are car",
	"You have s",
	"Your k",
	"You are cal",
	"You are a",
	"As a",
	"While s",
	"One",
	"When y",
	"What w",
	"What a",
	"Wi",
	"You are i",
	"Ther",
	"Your m",
	"On ",
	"You a",
	"You are w",
	"You are ch",
	"You r",
	"Your father h",
	"You f",
	"While l",
	"After m"
};

/**
The lengths of the questions.
**/
intern const size_t executable_question_lens[51] = {
	13,
	4,
	6,
	4,
	6,
	6,
	9,
	6,
	7,
	10,
	2,
	7,
	7,
	1,
	7,
	1,
	4,
	7,
	1,
	1,
	4,
	8,
	7,
	8,
	7,
	2,
	6,
	11,
	10,
	6,
	11,
	9,
	4,
	7,
	3,
	6,
	6,
	6,
	2,
	9,
	4,
	6,
	3,
	5,
	9,
	10,
	5,
	13,
	5,
	7,
	7
};

/**
The length of the longest question.
**/
intern const size_t executable_question_max = 13;

/**
The effects of the questions.
**/
intern const int executable_question_effects[51][4][9] = {
	{
		{+1, -1,  0,  0, +1,  0,  0,  0, -1},
		{+1,  0, -1,  0,  0, -1,  0,  0, +1},
		{ 0, +1,  0,  0,  0, -1,  0,  0,  0},
		{-1, +1, +1,  0,  0, -1,  0,  0,  0}
	},
	{
		{+1, -1, +1,  0, +1, -1, -1,  0,  0},
		{ 0, +1,  0,  0,  0, -1,  0,  0,  0},
		{-1, +1, +1,  0, -1,  0,  0,  0,  0},
		{+1,  0,  0,  0, +1,  0,  0, -2,  0}
	},
	{
		{+1, -1,  0, +1, +1, -1,  0, -1,  0},
		{ 0, +1, -1, +1,  0, -1,  0,  0,  0},
		{-1,  0,  0, -1,  0,  0,  0, +2,  0},
		{-1,  0,  0,  0, -1, -1,  0, +1, +2}
	},
	{
		{+1,  0,  0,  0,  0,  0,  0,  0, -1},
		{ 0,  0, +1,  0,  0,  0,  0, -1,  0},
		{-1, +1,  0,  0,  0,  0,  0,  0,  0},
		{+1, -1,  0,  0, +1, -1,  0,  0,  0}
	},
	{
		{ 0, +2,  0,  0, -1, -1,  0,  0,  0},
		{ 0, +1,  0,  0, -1, -1,  0, +1,  0},
		{ 0, +1, -1, +1,  0, -1, -1,  0, +1},
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0}
	},
	{
		{ 0, +1, +1,  0, -1, -1,  0,  0,  0},
		{ 0, -1,  0,  0, +1,  0,  0,  0,  0},
		{ 0, +1,  0, -1,  0, +1,  0,  0, -1},
		{ 0, -1,  0, +1,  0,  0,  0,  0,  0}
	},
	{
		{ 0, +2,  0, -1, -1,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0, +1, -1},
		{ 0, -1,  0, +1,  0,  0,  0,  0,  0},
		{ 0,  0, +1, +1, -1,  0,  0, -1,  0}
	},
	{
		{ 0, -1, +2,  0,  0, -1,  0,  0,  0},
		{ 0,  0, -1,  0,  0,  0,  0,  0, +1},
		{ 0, -1, -1, +1,  0,  0,  0,  0, +1},
		{ 0, +1,  0,  0,  0, -1,  0,  0,  0}
	},
	{
		{ 0, -1,  0,  0, +1,  0,  0,  0,  0},
		{ 0,  0, +2, -1, -1,  0,  0,  0,  0},
		{ 0, -1, -1,  0,  0,  0,  0, +2,  0},
		{ 0,  0,  0, +1, -1,  0,  0,  0,  0}
	},
	{
		{ 0, +1,  0, -1,  0,  0,  0,  0,  0},
		{ 0, -2, +2,  0, +1,  0, -1,  0,  0},
		{ 0, +1,  0, +1,  0, -2,  0,  0,  0},
		{ 0, +1, -2,  0,  0,  0,  0,  0, +1}
	},
	{
		{ 0, -1, +2,  0,  0, -1,  0,  0,  0},
		{ 0,  0, -1,  0,  0, -1,  0, +1, +1},
		{ 0,  0, -1, +1,  0, -1,  0, +1,  0},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0}
	},
	{
		{ 0, -1,  0, +1,  0,  0,  0, -1, +1},
		{ 0, -1,  0, +2,  0, -1,  0,  0,  0},
		{+1,  0,  0, -1,  0, -1,  0,  0, +1},
		{ 0,  0, +1,  0,  0,  0,  0, -1,  0}
	},
	{
		{ 0,  0,  0,  0,  0,  0, +1, -1,  0},
		{ 0, +1,  0,  0, -1,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0, +1,  0, -1,  0},
		{ 0, -1,  0,  0,  0, -1,  0, +1, +1}
	},
	{
		{ 0, +1, -1,  0, +1, -1,  0,  0,  0},
		{ 0,  0, -2,  0,  0,  0,  0, +2,  0},
		{ 0, -1, +1,  0, -1, +1,  0,  0,  0},
		{ 0, -2,  0, +1,  0,  0,  0, +1,  0}
	},
	{
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{ 0, +1, -1,  0,  0,  0,  0, -1, +1},
		{ 0,  0, -1, +1,  0, -1,  0, +1,  0},
		{ 0, +1, +1,  0,  0,  0,  0, -2,  0}
	},
	{
		{ 0, +1, -1,  0,  0, -1,  0, +1,  0},
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{+1, -1, +1,  0, +1,  0,  0, -1, -1},
		{ 0, -1, +1,  0,  0,  0,  0, +1, -1}
	},
	{
		{ 0, -1,  0,  0, +1,  0,  0,  0,  0},
		{ 0,  0,  0, +1, -1,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0, +1,  0, -1,  0},
		{ 0, +1,  0, -1,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0,  0, +1, +1, -1,  0, -1,  0},
		{+1, -1,  0,  0, +1,  0,  0, -1,  0},
		{-1,  0,  0, +1, -1,  0,  0, +1,  0},
		{+1,  0,  0, -1,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0,  0, +1,  0, -1,  0,  0,  0},
		{ 0, -1,  0,  0,  0,  0,  0, +1,  0},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0},
		{ 0, -1, -1,  0,  0,  0,  0, +2,  0}
	},
	{
		{-1,  0,  0, +1, -1,  0,  0,  0, +1},
		{-1, +1, -2, +1, +1, -1,  0,  0, +1},
		{ 0, +1,  0, +1,  0, -2,  0, -1, +1},
		{-1,  0, +1,  0,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0, +1,  0,  0, +1,  0, -2,  0},
		{ 0,  0,  0,  0,  0, -1,  0, +1,  0},
		{ 0,  0, -1, +1,  0,  0,  0,  0,  0},
		{-1, +1, -1, +1,  0, -1,  0,  0, +1}
	},
	{
		{ 0,  0,  0, -1,  0, +2,  0, -1,  0},
		{ 0, +1, -1,  0,  0, -1,  0, +1,  0},
		{ 0,  0, -1,  0, +1,  0,  0,  0,  0},
		{ 0,  0, +1,  0,  0,  0,  0,  0, -1}
	},
	{
		{-1, +1,  0,  0,  0, +1,  0, -1,  0},
		{-1,  0,  0,  0, -1,  0,  0, +2,  0},
		{-1, -1,  0, +1,  0,  0,  0, +1,  0},
		{ 0,  0,  0,  0,  0,  0,  0, -1, +1}
	},
	{
		{-1,  0,  0,  0, -1, +1,  0, +1,  0},
		{+1, -1,  0,  0, +1, -1,  0,  0,  0},
		{ 0,  0,  0,  0, +1, +1,  0, -1, -1},
		{ 0,  0, +1,  0, -1,  0,  0,  0,  0}
	},
	{
		{ 0, +1,  0,  0, -2, +1,  0,  0,  0},
		{ 0, -1, +1, +1, -1,  0,  0,  0,  0},
		{-1,  0, -1, +1,  0,  0,  0,  0, +1},
		{ 0,  0, -1,  0, +1,  0,  0, -1, +1}
	},
	{
		{+1,  0,  0,  0, +1, -1,  0, -1,  0},
		{ 0,  0,  0, +1,  0, -1,  0,  0,  0},
		{ 0,  0, +1,  0,  0, -1,  0,  0,  0},
		{ 0,  0, +1,  0,  0,  0, -2, +1,  0}
	},
	{
		{+1,  0, +1, -1,  0, -1,  0,  0,  0},
		{ 0, +1, -1,  0,  0, +1,  0,  0, -1},
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{-1,  0,  0, +1,  0, +1,  0,  0, -1}
	},
	{
		{ 0, -1,  0,  0,  0, +1,  0, +1, -1},
		{ 0,  0,  0,  0,  0, +1,  0,  0, -1},
		{+1,  0,  0, +1,  0, -1,  0, -1,  0},
		{+1,  0,  0,  0, +1,  0,  0, -1, -1}
	},
	{
		{ 0,  0, -1, +1, -1,  0,  0,  0, +1},
		{+1, -2,  0,  0, +1,  0,  0,  0,  0},
		{ 0, -1,  0,  0,  0, +1,  0, +1, -1},
		{ 0, -1,  0,  0,  0,  0,  0, +1,  0}
	},
	{
		{ 0,  0, -1,  0,  0, +1,  0,  0,  0},
		{ 0,  0, +2,  0,  0, -1,  0, -1,  0},
		{+1, -1,  0,  0, +1, -1,  0,  0,  0},
		{ 0, -1,  0, +1,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0, -1,  0,  0, +1,  0, -1, +1},
		{ 0,  0, +2,  0,  0, -1,  0,  0, -1},
		{ 0, +1,  0,  0,  0, -1,  0, +1, -1},
		{ 0,  0, +1,  0,  0,  0,  0,  0, -1}
	},
	{
		{ 0, +1,  0,  0,  0, -1,  0,  0,  0},
		{ 0,  0, -1,  0,  0, -1,  0, +2,  0},
		{ 0,  0, +1,  0,  0,  0,  0, -1,  0},
		{ 0,  0, +1,  0,  0, -1,  0,  0,  0}
	},
	{
		{ 0,  0, -1,  0,  0,  0,  0, +1,  0},
		{ 0, +1,  0,  0,  0,  0,  0,  0, -1},
		{ 0,  0,  0,  0, -1, +1,  0,  0,  0},
		{ 0,  0,  0, +1,  0, -1,  0,  0,  0}
	},
	{
		{ 0, +1,  0,  0,  0,  0,  0,  0, -1},
		{+1, -1,  0, +1,  0,  0,  0, -1,  0},
		{ 0, -1,  0, -1,  0,  0,  0, +2,  0},
		{ 0,  0,  0,  0,  0, -1,  0, +1,  0}
	},
	{
		{+1,  0,  0, -1, +1, -1,  0,  0,  0},
		{ 0, +1,  0, +1,  0,  0,  0, -1, -1},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0},
		{ 0,  0, +1,  0,  0, -1,  0,  0,  0}
	},
	{
		{+1, -1, -1,  0, +1,  0,  0,  0,  0},
		{+1, +1,  0,  0, -1,  0,  0, -1,  0},
		{+1,  0,  0,  0, +1, -2,  0,  0,  0},
		{ 0,  0, +2,  0,  0, -2,  0,  0,  0}
	},
	{
		{+1, -1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0, -1,  0, +1,  0},
		{-1,  0,  0, +1,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0, +1,  0, -1,  0}
	},
	{
		{ 0,  0, +1,  0,  0, -1,  0,  0,  0},
		{ 0,  0,  0,  0,  0, -2,  0, +1, +1},
		{ 0,  0,  0, +1,  0, -1,  0,  0,  0},
		{ 0,  0,  0, -1, +1,  0,  0,  0,  0}
	},
	{
		{+1, -1, +1, -1,  0,  0,  0,  0,  0},
		{-1, +1,  0,  0,  0,  0,  0,  0,  0},
		{ 0, -1,  0,  0,  0, +1,  0,  0,  0},
		{-1,  0,  0,  0, -1, +1,  0, +1,  0}
	},
	{
		{ 0,  0, -1, +1,  0,  0,  0,  0,  0},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0},
		{+1, -1,  0,  0,  0,  0,  0,  0,  0},
		{ 0, -1,  0, +1,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0,  0,  0,  0,  0,  0, -1, +1},
		{ 0, +1, -1,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0, -1,  0, +1,  0},
		{ 0,  0,  0,  0, +1,  0,  0,  0, -1}
	},
	{
		{+1,  0,  0, -1,  0,  0,  0,  0,  0},
		{-1, +1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0, -1, +1,  0,  0,  0,  0},
		{ 0,  0,  0, +1, -1,  0,  0,  0,  0}
	},
	{
		{ 0,  0,  0, -1, +1,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0, -1, +1},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0},
		{+1, -1,  0,  0,  0,  0,  0,  0,  0}
	},
	{
		{ 0, -1,  0,  0,  0,  0,  0, +1,  0},
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{+1, -1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0, +1, -1,  0,  0,  0, -1, +1}
	},
	{
		{+1, -1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0, -1, +1,  0,  0,  0,  0,  0},
		{+1,  0,  0,  0, +1, -1,  0, -1,  0},
		{-1, +1,  0,  0,  0,  0,  0,  0,  0}
	},
	{
		{ 0,  0,  0,  0,  0, +1,  0, -1,  0},
		{ 0,  0,  0, +1,  0,  0,  0, -1,  0},
		{ 0,  0, +1,  0,  0,  0,  0, -1,  0},
		{ 0,  0,  0,  0,  0, -1,  0, +1,  0}
	},
	{
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{ 0,  0, -1,  0,  0,  0,  0, +1,  0},
		{+1, -1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0, -1, +1}
	},
	{
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{ 0,  0, +1,  0, +1, -1,  0, -1,  0},
		{ 0,  0, +1,  0,  0, -1,  0,  0,  0},
		{ 0,  0, -1, +1,  0, -1,  0, +1,  0}
	},
	{
		{ 0, -1,  0,  0,  0, +1,  0, +1, -1},
		{ 0, +1,  0, -1,  0,  0,  0,  0,  0},
		{ 0, -1,  0, +1,  0, -1,  0, +1,  0},
		{ 0,  0, -1,  0,  0,  0,  0, +1,  0}
	},
	{
		{ 0, -1, +1,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0, +1, -1},
		{ 0, +1, -1,  0,  0, -1,  0, +1,  0},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0}
	},
	{
		{ 0,  0, +1,  0,  0,  0,  0,  0, -1},
		{ 0,  0, +1,  0,  0,  0,  0, -1,  0},
		{+1,  0,  0,  0, +1,  0,  0, -2,  0},
		{ 0, +1,  0,  0,  0,  0,  0, -1,  0}
	}
};

/**
A pointer to the item characters.
**/
intern const char const executable_terrain_chars[62] = {
	'?',
	'#',
	'#',
	'.',
	'+',
	'#',
	'#',
	'/',
	'#',
	'+',
	'.',
	'+',
	'.',
	'/',
	'=',
	'"',
	'~',
	'^',
	'&',
	'"',
	'.',
	'^',
	'^',
	'!',
	'~',
	'"',
	'.',
	'o',
	'*',
	'.',
	'=',
	'+',
	'*',
	'=',
	'*',
	'*',
	'*',
	'*',
	'*',
	'o',
	'o',
	'o',
	'*',
	'=',
	'*',
	'*',
	'o',
	'*',
	'*',
	'*',
	'o',
	'&',
	'*',
	'*',
	'*',
	'o',
	'*',
	'*',
	'*',
	'*',
	'#',
	'*'
};

intern const char const executable_object_chars[38] = {
	' ',
	'>',
	'<',
	'>',
	'<',
	'_',
	'*',
	'*',
	'^',
	'0',
	'^',
	'^',
	'|',
	'0',
	'&',
	'+',
	'8',
	'"',
	'"',
	'"',
	'"',
	'"',
	'"',
	'$',
	'$',
	'_',
	'_',
	'_',
	'_',
	'.',
	'.',
	'.',
	'.',
	'_',
	'.',
	'&',
	'!',
	'T'
};

/**
A pointer to the item characters.
**/
intern const char * const executable_item_chars = (void * )0x8286c0c;

/**
A pointer to the material colors.
**/
intern const char const executable_material_colors[14] = {7, 7, 6, 6, 10, 15, 15, 6, 6, 14, 8, 7, 7, 8};

/**
A pointer to the item data.
**/
intern const executable_item_data_t * const executable_item_data = (void * )0x08273b00;

/**
A pointer to the monster data.
**/
intern const executable_monster_data_t * const executable_monster_data = (void * )0x08264fc0;

/**
A pointer to the turn count.
**/
intern int * const executable_turns = (void * )0x082b16e0;

/**
A pointer to the save count.
**/
intern int * const executable_saves = (void * )0x082b6140;

/**
A pointer to the terrain map.
**/
intern unsigned char ** const executable_terrain = (void * )0x0829ea60;

/**
A pointer to the object map.
**/
intern unsigned char ** const executable_objects = (void * )0x0829ea78;

/**
A pointer to the item map.
**/
intern executable_map_item_t *** const executable_items = (void * )0x082a4e7c;

/**
A pointer to the monster map.
**/
intern executable_map_monster_t ** const executable_monsters = (void * )0x082b6a08;

/**
Untested variables.
**/
intern unsigned int * const executable_blindness = (void * )0x082b17dc;
intern unsigned int * const executable_stunnedness = (void * )0x082b17e0;
intern unsigned int * const executable_confusion = (void * )0x082b17e4;
intern unsigned int * const executable_slowness = (void * )0x082b17e8;
intern unsigned int * const executable_paralyzation = (void * )0x082b17ec;
intern unsigned int * const executable_sickness = (void * )0x082b17f0;
intern unsigned int * const executable_deafness = (void * )0x082b17f4;
intern unsigned int * const executable_muteness = (void * )0x082b17f8;
intern unsigned int * const executable_sleepiness = (void * )0x082b17fc;
intern unsigned int * const executable_blessedness = (void * )0x082b1800;
intern unsigned int * const executable_invisibility = (void * )0x082b1804;
intern unsigned int * const executable_drunkenness = (void * )0x082b5f50;
intern unsigned int * const executable_tactics = (void * )0x082b5f54;
intern unsigned int * const executable_satiety = (void * )0x082b5f60;
intern unsigned int * const executable_intrinsics = (void * )0x082b5f64;
intern int * const executable_alignment = (void * )0x082b5fac;
intern int * const executable_chaotic_piety = (void * )0x082b5fb0;
intern int * const executable_neutral_piety = (void * )0x082b5fb4;
intern int * const executable_lawful_piety = (void * )0x082b5fb8;
intern unsigned int * const executable_corruption = (void * )0x082b615c;
intern unsigned int * const executable_corruptions = (void * )0x082b6160;
intern unsigned int * const executable_strength_of_atlas = (void * )0x082b61e8;
intern unsigned int * const executable_farsight = (void * )0x082b61ec;
intern unsigned int * const executable_time = (void * )0x082b61f4;//?
intern unsigned int * const executable_speed = (void * )0x082b6230;
intern int * const executable_speed_modifier = (void * )0x082b6238;

intern const char * const executable_blindness_string = "Blind";
intern const char * const executable_stunnedness_string = "Stunned";
intern const char * const executable_confusion_string = "Confused";
intern const char * const executable_slowness_string = "Slowed";
intern const char * const executable_paralyzation_string = "Paralyzed";
intern const char * const executable_sickness_string = "Sick";
intern const char * const executable_deafness_string = "Deaf";
intern const char * const executable_muteness_string = "Mute";
intern const char * const executable_sleepiness_string = "Sleeping";
intern const char * const executable_blessedness_string = "Blessed";
intern const char * const executable_invisibility_string = "Invisible";
intern const char * const executable_drunkenness_string = "Drunk";
intern const char * const executable_severe_berserking_string = "Extremely Berserk";//naked
intern const char * const executable_berserking_string = "Berserk";//= 0
intern const char * const executable_severe_aggressiveness_string = "Very Aggressive";
intern const char * const executable_aggressiveness_string = "Aggressive";
intern const char * const executable_densiveness_string = "Defensive";
intern const char * const executable_severe_densiveness_string = "Very Defensive";
intern const char * const executable_cowardice_string = "Cowardly";//= 6
intern const char * const executable_severe_cowardice_string = "Extremely Cowardly";//low hp
intern const char * const executable_fatal_starvation_string = "Starved";//< 0
intern const char * const executable_starvation_string = "Starving";//< 50
intern const char * const executable_severe_hunger_string = "Very Hungry";//< 100
intern const char * const executable_hunger_string = "Hungry";//< 200
intern const char * const executable_satiety_string = "Satiated";//> 1500 && <= 2500
intern const char * const executable_bloatedness_string = "Bloated";//> 2500
intern const char * const executable_fatal_bloatedness_string = "Overfed";
intern const char * const executable_intrinsic_fire_resistance_string = "Fire Resistant";
intern const char * const executable_intrinsic_poison_resistance_string = "Poison Resistant";
intern const char * const executable_intrinsic_cold_resistance_string = "Cold Resistant";
intern const char * const executable_intrinsic_acid_resistance_string = "Acid Resistant";
intern const char * const executable_intrinsic_luck_string = "Lucky";
intern const char * const executable_intrinsic_fate_smile_string = "Fate Smilant";
intern const char * const executable_intrinsic_curse_string = "Cursed";
intern const char * const executable_intrinsic_sleep_resistance_string = "Sleep Resistant";
intern const char * const executable_intrinsic_petrification_resistance_string = "Petrification Resistant";
intern const char * const executable_intrinsic_doom_string = "Doomed";
intern const char * const executable_intrinsic_teleportitis_string = "Teleporting";
intern const char * const executable_intrinsic_invisibility_string = "Invisible";
intern const char * const executable_intrinsic_teleport_control_string = "Teleport Controlling";
intern const char * const executable_intrinsic_stun_resistance_string = "Stun Resistant";
intern const char * const executable_intrinsic_death_ray_resistance_string = "Death Ray Resistant";
intern const char * const executable_intrinsic_paralyzation_resistance_string = "Paralyzation Resistant";
intern const char * const executable_intrinsic_shock_resistance_string = "Shock Resistant";
intern const char * const executable_intrinsic_invisible_sight_string = "Invisible Seeing";
intern const char * const executable_intrinsic_fire_immunity_string = "Fire Immune";
intern const char * const executable_intrinsic_acid_immunity_string = "Acid Immune";
intern const char * const executable_intrinsic_cold_immunity_string = "Cold Immune";
intern const char * const executable_intrinsic_shock_immunity_string = "Shock Immune";
intern const char * const executable_intrinsic_water_breathing_string = "Water Breathing";
intern const char * const executable_intrinsic_holiness_string = "Holy";
intern const char * const executable_intrinsic_confusion_resistance_string = "Confusion Resistant";
intern const char * const executable_severe_chaoticness_string = "Very Chaotic";//<= -5000
intern const char * const executable_chaoticness_string = "Chaotic";//< -1250
intern const char * const executable_lawfulness_neutral_string = "Chaotic Neutral";//< -125
intern const char * const executable_neutrality_string = "Neutral";//>= 125 && <= -125
intern const char * const executable_lawfulness_neutral_string = "Lawful Neutral";//> 125
intern const char * const executable_lawfulness_string = "Lawful";//> 1250
intern const char * const executable_severe_lawfulness_string = "Very Lawful";//>= 5000
intern const char * const executable_not_corrupted_string = "Uncorrupted";//< 1000
intern const char * const executable_corrupted_string = "Corrupted";//< 17000
intern const char * const executable_corrupted_string = "Very Corrupted";//>= 17000
intern const char * const executable_strength_of_atlas_string = "Strong as Atlas";
intern const char * const executable_farsight_string = "Farsighty";
intern const char * const executable_severe_fastness_string = "Very Fast";//>= 150 plus starsign
intern const char * const executable_fastness_string = "Fast";//>= 105
intern const char * const executable_slowness_string = "Slow";//<= 95
intern const char * const executable_severe_slowness_string = "Very Slow";//>= 65

intern const char * const executable_poisonedness_string = "Poisoned";//?
intern const char * const executable_severe_bleeding_string = "Severely Bleeding";
intern const char * const executable_bleeding_string = "Bleeding";//?
intern const char * const executable_fatal_overburdenedness_string = "Crushed";
intern const char * const executable_overburdenedness_string = "Overburdened";
intern const char * const executable_severe_strainedness_string = "Very Strained";
intern const char * const executable_strainedness_string = "Strained";
intern const char * const executable_burdenedness_string = "Burdened";//?

/**
The random number generator's counter c.
**/
intern unsigned char * const executable_arc4_c = (void * )0x08264a60;

/**
The random number generator's state S.
**/
intern unsigned char * const executable_arc4_s = (void * )0x082ada40;

/**
The random number generator's first iterator i.
**/
intern unsigned char * const executable_arc4_i = (void * )0x082adb40;

/**
The random number generator's second iterator j.
**/
intern unsigned char * const executable_arc4_j = (void * )0x082adb41;

/**
The amount of random number generator calls measured
	from seeding the random number generator
	to its first cyclic point.
**/
intern const unsigned int executable_arc4_calls = 1165;

/**
The amount of random number generator calls
	from the splash screen or the main menu
	to loading a game automatically.
**/
intern const unsigned int executable_arc4_calls_automatic_load = 205;

/**
The amount of random number generator calls measured
	from the splash screen or the main menu
	to loading a game manually.
**/
intern const unsigned int executable_arc4_calls_manual_load = 409;

/**
The emulated random number generator's state S.
**/
unsigned char arc4_s[0x100];

/**
The emulated random number generator's first iterator i.
**/
unsigned char arc4_i = 0x00;

/**
The emulated random number generator's second iterator j.
**/
unsigned char arc4_j = 0x00;

/**
The emulated random number generator's counter c.
**/
unsigned int arc4_c = 0;

/**
Seeds the current state S.

@param seed The seed k.
**/
void sarc4(const int seed) {
	unsigned char i = 0x00;
	unsigned char j = 0x00;
	do {
		arc4_s[i] = i;
		i++;
	} while(i != 0x00);
	do {
		j = (unsigned char )(j + (arc4_s[i] + ((const unsigned char * )&seed)[i % sizeof seed]));
		SWAP(arc4_s[i], arc4_s[j]);
		i++;
	} while (i != 0x00);
}

/**
Generates a byte r (and changes the current state).

The order of operations is wrong to replicate the behavior of the executable.

@return The byte r.
**/
unsigned char arc4(void) {
	//point A
	arc4_j += arc4_s[arc4_i];
	SWAP(arc4_s[arc4_i], arc4_s[arc4_j]);
	const unsigned char r = arc4_s[(unsigned char )(arc4_s[arc4_i] + arc4_s[arc4_j])];
	arc4_i++;//should be at point A
	return r;
}

/**
Generates an integer R (and changes the current state).

@return The integer R.
**/
unsigned int arc4l(void) {
	arc4_c++;
	unsigned int R = 0;
	for (size_t bit = 0; bit < sizeof R; bit++) {
		R |= arc4() << bit * CHAR_BIT;
	}
	return R;
}

/**
Counts and stuff.
**/
unsigned int arc4s(const unsigned int sup) {
	const int x = arc4l();//((int (*)(int) )0x08126130)(sup);
	return x % sup;
}

/**
Generates and injects
	the initial state S and
	the iterators i and j.

@param seed The seed k to use.
**/
void iarc4(const unsigned int seed, const unsigned int calls) {//inelegant, but works
	arc4_i = 0;
	arc4_j = 0;
	srandom(seed);
	sarc4(random());
	for (unsigned int call = 0; call < executable_arc4_calls; call++) {
		arc4l();
	}
	const unsigned int slurp = 20;
	const unsigned int add = 10;
	const unsigned int sup = 18;
	int first = arc4s(slurp);
	for (unsigned int iterator = 0; iterator < first + add; iterator++) {
		const unsigned int second = arc4s(sup);
		while (arc4s(sup) == second);
	}
	for (unsigned int call = 0; call < calls; call++) {
		arc4l();
	}
	memcpy(executable_arc4_c, &arc4_c, sizeof arc4_c);
	memcpy(executable_arc4_s, arc4_s, sizeof arc4_s);
	memcpy(executable_arc4_i, &arc4_i, sizeof arc4_i);
	memcpy(executable_arc4_j, &arc4_j, sizeof arc4_j);
}

/*
Returns the key code of a key number.

The code is generated automatically:
<pre>
gcc src/meta.c -O3 -o obj/meta
mkdir -p src/meta
obj/meta key_code > src/meta/key_code.c
rm -f obj/meta
</pre>

@param code The key number.
@return The key code.
*/
const char * key_code(int key);
#include "meta/key_code.c"

#endif
