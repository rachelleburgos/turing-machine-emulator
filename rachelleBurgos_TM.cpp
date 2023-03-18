/**
 * @file TuringMachine.cpp
 * @author Rachelle Burgos (rachelle.burgos27@myhunter.cuny.edu)
 * @brief This program simulates a one-tape, one-track, two-way infinite tape deterministic Turing machine.
 *        The program reads in a Turing machine description from a file and then reads in a string to be processed.
 *        The program then simulates the Turing Machine on the input string and prints out the ID for each configuration. 
 * @date 2023-03-15
 */

using namespace std;

#include "TuringMachine.h"

int main(int argc, char **argv) {
    // Check if the user provided a file name.
    if (argc < 2) {
        cerr << "Error: Please provide a valid name." << endl;
        return 1;
    }

    ifstream in_file(argv[1]);

    // Read in the input string.
    string input_string;
    bool valid_input = false;
    cout
            << "Enter a string to be processed by the Turing Machine. Note: The string must contain only the symbols '0' and '1'."
            << endl;

    // Keep prompting the user for a valid input string until the user enters a valid input string.
    while (!valid_input) {
        getline(std::cin, input_string, '\n');

        // Allow the user to enter an empty string.
        // If the user inputs a string consisting of only whitespace characters, then the input string is considered to be empty.
        if (input_string.empty()) {
            break;
        } else if (std::all_of(input_string.begin(), input_string.end(), [](unsigned char c) { return std::isspace(c); })) {
            input_string.clear();
            break;
        }

        // Otherwise, if the string contains whitespaces remove the whitespaces.
        input_string.erase(std::remove_if(input_string.begin(), input_string.end(), [](unsigned char c) { return std::isspace(c); }), input_string.end());

        // Check if the input string is valid.
        // The input string is valid if it contains only the symbols '0' and '1'.
        for (char symbol: input_string) {
            if (symbol != '0' && symbol != '1') {
                cerr << "Error: Invalid input string. Please enter a string that contains only the symbols '0' and '1'."
                     << endl;
                valid_input = false;
                break;
            } else {
                valid_input = true;
            }
        }
    }

    // Clear the input buffer.
    cin.clear();

    cout << "The input string is: " << input_string << endl
         << "The Turing Machine is now running..." << endl
         << "To pause the simulation and enter a new input string, press the h key." << endl << endl;

    TuringMachine tm(in_file, input_string);
    tm.run();

    return 0;
}