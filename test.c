// #include <ncurses.h>

// int	main(void)
// {
// 	int	ch;

// 	initscr();
// 	raw();
// 	keypad(stdscr, TRUE);
// 	//printw("Press Arrow Up, Arrow Down, or Delete. Press q to quit.\n");
// 	refresh();
// 	while ((ch = getch()) != 'q')
// 	{
// 		if (ch == KEY_UP)
// 		{
// 			printw("Up Arrow Pressed\n");
// 		}
// 		else if (ch == KEY_DOWN)
// 		{
// 			printw("Down Arrow Pressed\n");
// 		}
// 		else if (ch == 127)
// 		{
// 			printw("Delete Key Pressed\n");
// 		}
// 		refresh();
// 	}
// 	endwin();
// 	return (0);
// }

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>
#include <unistd.h>
#include "minishell.h"

// // Define a simple node structure for the linked list
// struct Node {
//     int data;
//     struct Node* next;
// };

// // Function to add a new node to the linked list
// void insertNode(struct Node** head, int data) {
//     struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
//     newNode->data = data;
//     newNode->next = *head;
//     *head = newNode;
// }

// // Function to print the linked list
// void printList(struct Node* head) {
//     while (head != NULL) {
//         printf("%d ", head->data);
//         head = head->next;
//     }
//     printf("\n");
// }

// // Function to read a line of input with history support
// char* readlineWithHistory() {
//     char* input = readline("Enter command: ");
//     if (input && *input) {
//         add_history(input);
//     }
//     return input;
// }

// // Function to set terminal attributes for non-canonical mode
// // void setNonCanonicalMode() {
// //     struct termios term;
// //     tcgetattr(STDIN_FILENO, &term);
// //     term.c_lflag &= ~(ICANON | ECHO);
// //     tcsetattr(STDIN_FILENO, TCSANOW, &term);
// // }

// // Function to restore terminal attributes to canonical mode
// void restoreTerminalAttributes() {
//     struct termios term;
//     tcgetattr(STDIN_FILENO, &term);
//     term.c_lflag |= ICANON | ECHO;
//     tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

// // Function to detect the up arrow key
// int isUpArrowPressed(char* input) {
//     return (input[0] == 27 && input[1] == 91 && input[2] == 65);
// }

// int main() {
//     struct Node* head = NULL;

//     // Insert some sample data into the linked list
//     for (int i = 0; i < 10; ++i) {
//         insertNode(&head, i);
//     }

// //    setNonCanonicalMode();

//     while (1) {
//         char* input = readlineWithHistory();

//         if (input) {
//             if (isUpArrowPressed(input)) {
//                 // Handle up arrow key press
//                 // Implement your scrolling logic here
//                 printf("Up arrow pressed - Implement scrolling logic here\n");
//             } else {
//                 // Handle other input
//                 // Implement your other logic here
//                 printf("Input: %s\n", input);
//             }

//             free(input);
//         }
//     }

//     restoreTerminalAttributes();

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

// Define a simple node structure for the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to add a new node to the linked list
void insertNode(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

// Function to print the linked list
void printList(struct Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Function to read a line of input with history support
char* readlineWithHistory(t_cmd_hist** history) {
    char* input = readline("Enter command: ");

    if (input && *input) {
        add_history(input);

        // Update command history
        t_cmd_hist* newHistory = (t_cmd_hist*)malloc(sizeof(t_cmd_hist));
        newHistory->history = input;
        newHistory->history_index = 0;
        newHistory->history_size = sizeof(input);
        newHistory->next = *history;
        *history = newHistory;
    }

    return input;
}

// Function to restore terminal attributes to canonical mode
void restoreTerminalAttributes() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to detect the up arrow key
int isUpArrowPressed(char* input) {
    return (input[0] == 27 && input[1] == 91 && input[2] == 65);
}

// int main() {
//     struct Node* head = NULL;
//     t_cmd_hist* history = NULL;

//     for (int i = 0; i < 10; ++i) {
//         insertNode(&head, i);
//     }

//     while (1) {
//         char* input = readlineWithHistory(&history);

//         if (input) {
//             if (isUpArrowPressed(input)) {
//                 // Handle up arrow key press
//                 // Implement your scrolling logic here
//                 printf("Up arrow pressed - Implement scrolling logic here\n");
//             } else {
//                 // Handle other input
//                 // Implement your other logic here
//                 printf("Input: %s\n", input);
//             }

//             free(input);
//         }
//     }

//     // Free command history
//     while (history != NULL) {
//         t_cmd_hist* next = history->next;
//         free(history->history);
//         free(history);
//         history = next;
//     }

//     restoreTerminalAttributes();

//     return 0;
// }
