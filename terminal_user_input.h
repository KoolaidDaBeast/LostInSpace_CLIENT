#ifndef TERMINAL_USER_INPUT
#define TERMINAL_USER_INPUT

#include "string"

using namespace std;

//Read string user input from terminal
string read_string(string prompt);

//Read double user input from terminal
double read_double(string prompt);

//Read integer user input from terminal
int read_integer(string prompt);

//Read integer user input from terminal
int read_integer_range(string prompt, int min, int max);

//Read double user input from terminal
double read_double_range(string prompt, int min, int max);

//Read boolean user input from terminal
bool read_boolean(string prompt);

#endif

