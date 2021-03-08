#include <stack>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>


#ifndef CS3258_CMAKE_CLI_H
#define CS3258_CMAKE_CLI_H

class CLI {
public:

    /*
     * Function name: isValidNum
     * Description: returns true if the inputted chars can be parsed into a double, else false
     * Arguments: char* num- pointer to array of chars containing parse target
     * Globals: none
     * Returns: void
     */
    static bool isValidNum(char *num);

    /*
     * Function name: CLI_FloatPar
     * Description: parses a char array into a double
     * Arguments: char params[]- array to be parsed
     * Globals: none
     * Returns: parsed double
     */
    static double CLI_FloatPar(char params[]);

    /*
     * Function name: CLI_Move
     * Description: Prints params for now
     * Arguments: char params[]- params to be processed
     * Globals: none
     * Returns: void
     */
    static void CLI_Move(char params[]);

    /*
     * Function name: CLI_Draw
     * Description: Prints params for now
     * Arguments: char params[]- params to be processed
     * Globals: none
     * Returns: void
     */
    static void CLI_Draw(char params[]);

    /*
     * Function name: CLI_Color
     * Description: Prints params for now
     * Arguments: char params[]- params to be processed
     * Globals: none
     * Returns: void
     */
    static void CLI_Color(char params[]);

    /*
     * Function name: CLI_Read
     * Description: Calls CLI_ProcessLine on each line of a file, returns if file stack too large
     * Arguments: char params[]- params to be processed
     *            std::stack<char*> fileStack- stack of file names to be processed
     * Globals: none
     * Returns: void
     */
    static void CLI_Read(char *fileName, std::stack<char *> fileStack);

    /*
     * Function name: CLI_ProcessLine
     * Description: Processes command, calls modules as necessary
     * Arguments: char params[]- command followed by params to pass to modules
     *            std::stack<char*> fileStack- stack of file names to be processed
     * Globals: none
     * Returns: void
     */
    static void CLI_ProcessLine(char* currLine, std::stack<char *> fileStack);
};
#endif //CS3258_CMAKE_CLI_H

