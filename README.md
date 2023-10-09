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
| **handle_line**     | - Handles user input and history                  |
|                     | - Handles Ctrl + D                                |
| **lexer**           | - Checks if a character is part of a token group  |
|                     | - Handles bunny ears for token creation           |
|                     | - Finds token start and end positions             |
|                     | - Separates tokens in a line of code              |
| **parser**          | - Locates the next command in tokens              |
|                     | - Removes bunny ears from tokens                  |
|                     | - Extracts commands based on index pair           |
|                     | - Separates token list by pipe symbol             |
|                     | - Removes space tokens                            |
| **signals**         | - Resets terminal attributes                      |
|                     | - Handles printing a new line after a signal      |
| **vars**            | - Handles special index searching for variables   |
|                     | - Resets temporary storage                        |
|                     | - Breaks down the line based on indexes           |
|                     | - Recombines strings found in temp storage        |
|                     | - Handles variable replacement in commands        |
| **redirection**     | - Handles input redirection                       |
|                     | - Handles here document input redirection         |
|                     | - Handles output redirection                      |
|                     | - Handles append output redirection               |
|                     | - Checks if a string is a redirection symbol      |
|                     | - Checks if a command contains a redirection      |
|                     | - Gets the type of redirection                    |
| **cmds**            | X Runs a single command                           |
|                     | - Gets the number of commands                     |
|                     | - Gets the exit code of the last command          |
|                     | - Handles execution of multiple commands          |
|                     | X Executes commands                               |
|                     | - Gets possible command paths                     |
|                     | - Checks if a string is in executable format      |
|                     | X Gets all possible paths of a command            |
|                     | X Trims the "PATH=" string from a path            |
|                     | X Appends the command name to the path            |
| **builtins**        | - Handles the cd command                          |
|                     | - Handles the echo command                        |
|                     | - Handles the exit command                        |
|                     | - Handles the export command                      |
|                     | - Handles the pwd command                         |
|                     | - Handles the env command                         |
|                     | - Handles the unset command                       |
|                     | - Handles all builtin commands                    |


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