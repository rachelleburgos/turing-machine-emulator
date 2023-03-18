/**
 * @file TuringMachine.h
 * @author Rachelle Burgos (rachelle.burgos27@myhunter.cuny.edu)
 * @brief Contains the class definitions for the TuringMachine class and its nested classes.
 * @date 2023-03-15
 */

#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include "kbhit.h"

// KEY: current state, current tape symbol
// VALUE: <new state, new tape symbol>, direction
#define TRANSITION_MAP std::map<std::pair<char, char>, std::pair<std::pair<char, char>, char>>

class TuringMachine {
private:
    /**
     * @brief This function reads in a Turing Machine description from a file and creates a map of five-tuples
     *        representing the Turing Machine description (the transition function).
     *
     * @param in_file The input file stream object that contains the Turing Machine description.
     * @post Creates a map with the transition function of the Turing Machine.
     * The key of the map is a pair of the current state and the current tape symbol.
     * The value of the map is a pair of a pair of the new state and the new tape symbol and the direction.
     */
    void createTransitionFunction(std::ifstream &in_file) {
        std::string line;

        // Read in the file line by line.
        while (std::getline(in_file, line)) {
            // Check if the line contains a comment.
            if (line.substr(0, 2) == "//" || line.empty()) {
                continue;
            } else if (line.find("//") != std::string::npos) {
                // Remove the comment from the line.
                line = line.substr(0, line.find("//"));
            }

            // Create a five-tuple from the line and add it to the transition function map.
            std::stringstream ss(line);
            char input_current_state, current_tape_symbol, new_state, new_tape_symbol, direction;
            ss >> input_current_state >> current_tape_symbol >> new_state >> new_tape_symbol >> direction;

            transition_function[{input_current_state, current_tape_symbol}] = {{new_state, new_tape_symbol}, direction};
        }

        in_file.close();
    };

    /**
     * @brief Initializes the tape of the Turing Machine.
     *
     * @param input_string The input string to be processed by the Turing Machine.
     */
    void initializeTape(const std::string &input_string) {
        // Clear the tape.
        tape.clear();

        tape.push_back('B');
        // Add the input string to the tape.
        for (const auto& symbol: input_string) {
            tape.push_back(symbol);
        }

        // Add the blank symbol to the tape.
        tape.push_back('B');
    }

    /**
     * @brief Resets the read/write head of the Turing Machine to the start of the tape.
     */
    void resetHead() {
        // Set the read/write head left to the first non-blank symbol on the tape.
        read_write_head_index = 0;
    }

    /**
     * @brief This function prints the current ID of the Turing Machine. 
     * 
     * @param state The current state of the Turing Machine.
     */
    void printID(const char &state) { 
        std::string tape_str;
        for (const auto& symbol: tape) {
            tape_str += symbol;
        }

        size_t first_non_blank_symbol = tape_str.find_first_not_of('B');
        size_t last_non_blank_symbol = tape_str.find_last_not_of('B');
        if (last_non_blank_symbol != std::string::npos && tape_str[last_non_blank_symbol] == 'B') {
            last_non_blank_symbol = tape_str.find_last_not_of('B', last_non_blank_symbol-1);
        }

        std::cout << (first_non_blank_symbol != std::string::npos ? tape_str.substr(first_non_blank_symbol, read_write_head_index) : ""); // Print the tape up to the read/write head.
        std::cout << " [ q" << state << " ] "; // Print the current state of the Turing Machine.
        std::cout << (last_non_blank_symbol != std::string::npos && read_write_head_index+1 < tape_str.size() ? tape_str.substr(read_write_head_index+1, last_non_blank_symbol-read_write_head_index) : "") << std::endl; // Print the tape after the read/write head.
    }

    /**
     * @briief Moves the read/write head of the Turing Machine.
     *
     * @param direction The direction to move the read/write head.
     */
    void moveHead(const char &direction) {
        if (direction == 'R') {
            if (read_write_head_index == tape.size()-1) {
                tape.push_back('B');
                read_write_head_index++;
            } else if(read_write_head_index < tape.size()-1)
                read_write_head_index++;
        } else if (direction == 'L') {
            if (read_write_head_index == 0) {
                tape.insert(tape.begin(), 'B');
                read_write_head_index = 0;
            } else if (read_write_head_index > 0) {
                read_write_head_index--;
            }
        }
    }

    // **** Data Members ****

    TRANSITION_MAP transition_function; // The transition function of the Turing Machine.
    std::vector<char> tape; // The tape of the Turing Machine.
    char current_state; // The current state of the Turing Machine.
    size_t read_write_head_index; // The read/write head of the Turing Machine.

public:
    /**
     * @brief Construct a new Turing Machine object 
     */
    TuringMachine() : tape{}, current_state{'0'}, read_write_head_index{0} {}

    /**
     * @brief Construct a new Turing Machine object
     * 
     * @param in_file The input file containing information about the transition function.
     * @param input_string The input string to be processed by the Turing Machine.
     */
     TuringMachine(std::ifstream &in_file, const std::string &input_string)
            : tape{}, current_state{'0'}, read_write_head_index{0} {
        createTransitionFunction(in_file);
        initializeTape(input_string);
        resetHead();
    }

    /**
     * @brief Destroy the Turing Machine object
     */
    ~TuringMachine() = default;

    /**
     * @brief Simulates the Turing Machine on the input string. 
     */
    void run() {
        bool halt = false;
        const char final_state = 'f';

        while (!halt) {
            printID(current_state); // Print the ID of the Turing Machine.

            // Check if the user wants to halt the Turing Machine.
            // If so, then halt the Turing Machine, prompt for a new input string, and run the Turing Machine on the new input string.
            if (_kbhit()) {
                char halt_char = std::getchar();

                std::string new_input_string;
                bool valid_input = false;

                if (halt_char == 'h') {
                    std::cout << std::endl << "Turing Machine halted.\nEnter a new input string to be processed." << std::endl;

                    // Keep prompting the user for a valid input string until the user enters a valid input string.
                    while (!valid_input) {
                        std::getline(std::cin, new_input_string, '\n');

                        // Allow the user to enter an empty string.
                        // If the user inputs a string consisting of only whitespace characters, then the input string is considered to be empty.
                        if (new_input_string.empty()) {
                            break;
                        } else if (std::all_of(new_input_string.begin(), new_input_string.end(), [](unsigned char c) { return std::isspace(c); })) {
                            new_input_string.clear();
                            break;
                        }

                        // Otherwise, if the string contains whitespaces remove the whitespaces.
                        new_input_string.erase(std::remove_if(new_input_string.begin(), new_input_string.end(), [](unsigned char c) { return std::isspace(c); }), new_input_string.end());

                        // Check if the input string is valid.
                        // The input string is valid if it contains only the symbols '0' and '1'.
                        for (char symbol: new_input_string) {
                            if (symbol != '0' && symbol != '1') {
                                std::cerr
                                        << "Error: Invalid input string. Please enter a string that contains only the symbols '0' and '1'."
                                        << std::endl;
                                valid_input = false;
                                break;
                            } else {
                                valid_input = true;
                            }
                        }
                    }

                    // Clear the input buffer.
                    std::cin.clear();

                    // Initialize the Turing Machine's tape with the new input string.
                    initializeTape(new_input_string);

                    // Reset the Turing Machine's current state to the start state.
                    current_state = '0';

                    // Reset the Turing Machine's read/write head to the start of the tape.
                    resetHead();
                }
            }

            // Check if the read/write head is at the end of the tape.
            if(read_write_head_index+1 == tape.size()) {
                tape.push_back('B');
            }

            auto transition = transition_function.find({current_state, tape[read_write_head_index+1]});

            // Check if the Turing Machine is in a final state.
            if (current_state == final_state) {
                std::cout << "String was accepted by the Turing Machine.";
                halt = true;
            } else if (transition != transition_function.end()) {
                // Perform the transition.
                current_state = transition->second.first.first; // Update the current state of the Turing Machine.

                tape[read_write_head_index+1] = (transition->second.first.second); // Update the symbol on the tape that the read/write head is currently on.
                moveHead(transition->second.second); // Move the read/write head to the left or right.
            } else {
                std::cout << "There is no transition out of this state.\nString was rejected by the Turing Machine."
                          << std::endl;
                halt = true;
            }
        }
    }
};

#endif // TURING_MACHINE_H