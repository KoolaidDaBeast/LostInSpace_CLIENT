#include "splashkit.h"

using namespace std;

string read_string(string prompt)
{
    write_line(prompt);
    return read_line();
}

double read_double(string prompt)
{
    string line = "";
    double result;

    line = read_string(prompt);

    while (!is_number(line))
    {
        write_line("Please enter a valid number.");
        line = read_string(prompt);
    }

    result = convert_to_double(line);

    return result;
}

int read_integer(string prompt)
{
    string line = "";
    int result;

    line = read_string(prompt);

    while (!is_integer(line))
    {
        write_line("Please enter a whole number.");
        line = read_string(prompt);
    }

    result = convert_to_integer(line);

    return result;
}

int read_integer_range(string prompt, int min, int max)
{
    int result = read_integer(prompt);

    while (result < min or result > max)
    {
        write_line("Please enter a value between " + to_string(min) + " and " + to_string(max) + ".");

        result = read_integer(prompt);
    }

    return result;
}

double read_double_range(string prompt, int min, int max)
{
    double result = read_double(prompt);

    while (result < min or result > max)
    {
        write_line("Please enter a value between " + to_string(min) + " and " + to_string(max) + ".");

        result = read_double(prompt);
    }

    return result;
}

bool read_boolean(string prompt)
{
    string line = "";
    bool result;

    line = to_lowercase(trim(read_string(prompt)));

    while (line != "yes" && line != "y" && line != "no" && line != "n")
    {
        write_line("ERROR: You must say either 'yes|y' or 'no|n'!");
        line = to_lowercase(trim(read_string(prompt)));
    }

    if (line == "yes" || line == "y")
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

