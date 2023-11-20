# Minishell

Minishell is a challenging project undertaken as part of the 42 curriculum. This project aims to replicate the functionality of the bash shell. At 42, Minishell is crafted entirely from the ground up and stands as one of the most demanding projects in the curriculum.

## Project Overview

Minishell serves as a platform for students to deepen their understanding of shell scripting and system-level programming. By recreating the core functionalities of a Unix-like shell, participants gain valuable insights into process management, parsing, and command execution. This project is an opportunity to explore the inner workings of a fundamental tool used in the Unix ecosystem.

## Walkthrough

| Function            | Description                                       |
|---------------------|---------------------------------------------------|
| **main**            | X Initializes program data structure              |
|                     | X Converts environment variables to a linked list |
|                     | X Stores the current working directory            |
|                     | X Initializes termios structures for later use    |
| **handle_line**     | X Handles user input and history                  |
|                     | X Handles Ctrl + D                                |
| **lexer**           | X Checks if a character is part of a token group  |
|                     | X Handles bunny ears for token creation           |
|                     | X Finds token start and end positions             |
|                     | X Separates tokens in a line of code              |
| **parser**          | X Locates the next command in tokens              |
|                     | X Removes bunny ears from tokens                  |
|                     | X Extracts commands based on index pair           |
|                     | X Separates token list by pipe symbol             |
|                     | X Removes space tokens                            |
| **signals**         | X Resets terminal attributes                      |
|                     | X Handles printing a new line after a signal      |
| **vars**            | X Handles special index searching for variables   |
|                     | X Resets temporary storage                        |
|                     | X Breaks down the line based on indexes           |
|                     | X Recombines strings found in temp storage        |
|                     | X Handles variable replacement in commands        |
| **redirection**     | X Handles input redirection                       |
|                     | X Handles here document input redirection         |
|                     | X Handles output redirection                      |
|                     | X Handles append output redirection               |
|                     | X Checks if a string is a redirection symbol      |
|                     | X Checks if a command contains a redirection      |
|                     | X Gets the type of redirection                    |
| **cmds**            | X Runs a single command                           |
|                     | X Gets the number of commands                     |
|                     | X Gets the exit code of the last command          |
|                     | X Handles execution of multiple commands          |
|                     | X Executes commands                               |
|                     | X Gets possible command paths                     |
|                     | X Checks if a string is in executable format      |
|                     | X Gets all possible paths of a command            |
|                     | X Trims the "PATH=" string from a path            |
|                     | X Appends the command name to the path            |
| **builtins**        | X Handles the cd command                          |
|                     | X Handles the echo command                        |
|                     | X Handles the exit command                        |
|                     | X Handles the export command                      |
|                     | X Handles the pwd command                         |
|                     | X Handles the env command                         |
|                     | X Handles the unset command                       |
|                     | X Handles all builtin commands                    |


## Key Features

- **Bash Replication**: Minishell faithfully emulates the behavior of the bash shell, providing users with a familiar interface and feature set.

- **From Scratch**: The Minishell project is implemented entirely from scratch, allowing students to develop a deep understanding of the underlying mechanisms of a shell.

- **Curriculum Challenge**: As one of the flagship projects in the 42 curriculum, Minishell pushes students to apply their knowledge and problem-solving skills to create a functional and efficient shell.

## How to Use

To use Minishell, follow the installation and usage instructions provided in the project's documentation. You can find detailed information on building, running, and testing the shell in the project's repository.

## Contributions

Contributions to the Minishell project are welcome. If you have ideas for improvements or bug fixes, please feel free to submit pull requests. We encourage collaboration and the sharing of knowledge among the 42 community.

## Credits

Minishell is brought to you by Oreste & Akram 42 Heilbronn students.

## License

To be added...

----------------------------------------------------------------------------------------------------------------------------------------------------------------

**Note**: This README provides a brief overview of the Minishell project. For more in-depth information, consult the project's documentation and codebase. Happy shell scripting! 🚀 