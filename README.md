Craig McLaughlin
Homework Assignment 1
I adhere to the honor pledge, all work provided = my own
Two days late- my apologies.

Testing:
There are three test files provided: testRead.cli, testsInvalid.cli, and testsValid.cli. All commands in testsValid
should work without providing an error message, all commands in testsInvalid should provide error messages (but not
crash the program) and testRead tests whether the read command works properly.

Using: To use this CLI, input a command followed by the desired parameters, as specified. When inputting file names
use the complete path.

Information: All methods used for this assigment are declared in CLI.h and implemented in CLI.cpp. CLI_ProcessLine()
processes each line, calling the required module methods (such as read() and draw()) as necessary. CLI_ProcessLine()
and CLI_Read() can call each other, infinite recursion is avoided by passing the file stack back and forth and not
permitting more than 5 files to be processed at once. The main_loop() method simply calls the CLI_ProcessLine()
method.

Documentation: Method documentation is provided in the CLI.h file.

