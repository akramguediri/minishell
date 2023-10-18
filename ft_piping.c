/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/10/18 15:26:52 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipes_with_io_redirection(char *input_file, char *output_file,
		char **cmds[], int num_cmds)
{
	int		pipe_fd[num_cmds - 1][2];
	pid_t	children[num_cmds];
	int		input_fd;
	int		output_fd;
	int		i;
	int		j;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipe_fd[i]) == -1)
		{
			perror("Pipe creation failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		children[i] = fork();
		if (children[i] < 0)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (children[i] == 0)
		{
			if (i > 0)
			{
				dup2(pipe_fd[i - 1][0], STDIN_FILENO);
				close(pipe_fd[i - 1][0]);
			}
			else if (input_file)
			{
				input_fd = open(input_file, O_RDONLY);
				if (input_fd < 0)
				{
					perror("Failed to open input file");
					exit(EXIT_FAILURE);
				}
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			if (i < num_cmds - 1)
			{
				dup2(pipe_fd[i][1], STDOUT_FILENO);
				close(pipe_fd[i][1]);
			}
			else if (output_file)
			{
				output_fd = open(output_file, O_WRONLY | O_CREAT, 0644);
				if (output_fd < 0)
				{
					perror("Failed to open output file");
					exit(EXIT_FAILURE);
				}
				dup2(output_fd, STDOUT_FILENO);
				close(output_fd);
			}
			j = 0;
			while (j < num_cmds - 1)
			{
				close(pipe_fd[j][0]);
				close(pipe_fd[j][1]);
				j++;
			}
			execvp(cmds[i][0], cmds[i]);
			perror("Execvp failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipe_fd[i][0]);
		close(pipe_fd[i][1]);
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		waitpid(children[i], NULL, 0);
		i++;
	}
}

char	**addcommand(char *s)
{
	char	**t;
	char	**p;
	int		i;

	p = ft_split(s, ' ');
	i = 0;
	t = (char **)malloc(sizeof(char *));
	while (p[i])
	{
		t[i] = ft_strdup(p[i]);
		i++;
	}
	return (t);
}

char *process_command(char *command) {
    if (command == NULL) {
        return NULL;
    }

    int len = strlen(command);
    char *processed = (char *)malloc(len + 1); // Allocate memory for the processed command.

    // Initialize variables for input and output file names.
    char *input_file = NULL;
    char *output_file = NULL;

    int i = 0;
    int j = 0;

    // Process the command, looking for input and output redirection.
    while (i < len) {
        if (command[i] == '<') {
            // Handle input redirection, e.g., "cat < input.txt"
            i++; // Move past the '<' character
            while (i < len && command[i] == ' ') {
                i++; // Skip spaces
            }
            if (i < len) {
                // Extract the input file name
                while (i < len && command[i] != ' ' && command[i] != '>') {
                    input_file = (char *)realloc(input_file, j + 2);
                    input_file[j++] = command[i];
                    i++;
                }
                input_file[j] = '\0';
            }
        } else if (command[i] == '>') {
            // Handle output redirection, e.g., "command > output.txt"
            i++; // Move past the '>' character
            while (i < len && command[i] == ' ') {
                i++; // Skip spaces
            }
            if (i < len) {
                // Extract the output file name
                while (i < len && command[i] != ' ' && command[i] != '<') {
                    output_file = (char *)realloc(output_file, j + 2);
                    output_file[j++] = command[i];
                    i++;
                }
                output_file[j] = '\0';
            }
        } else {
            // Copy characters to the processed command
            processed[j++] = command[i];
            i++;
        }
    }

    processed[j] = '\0'; // Null-terminate the processed command

    // Now, you can use 'processed', 'input_file', and 'output_file' as needed.

    // Clean up and free dynamically allocated memory.
    free(input_file);
    free(output_file);
    return (processed);
}

int main(void) {
    char *input_command2 = "cat < input.txt | grep 'pattern' > output.txt";
    char **input_command = ft_split(input_command2, ' ');
    int i = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    char *commandlist = NULL;
    char ***cmds;
    char **t;

    // Allocate memory for commandlist.
    commandlist = malloc(1);
    commandlist[0] = '\0';

    while (input_command[i]) {
        if (ft_strncmp(input_command[i], "<", 1) == 0) {
            if (input_command[i + 1]) {
                input_file = strdup(input_command[i + 1]);
                i++;
            }
        } else if (ft_strncmp(input_command[i], ">", 1) == 0) {
            if (input_command[i + 1]) {
                output_file = strdup(input_command[i + 1]);
                i++;
            }
        } else {
            char *processed = process_command(input_command[i]);
            if (processed) {
                commandlist = ft_strjoin(commandlist, processed);
                commandlist = ft_strjoin(commandlist, " ");
                free(processed);
            }
        }
        i++;
    }

    printf("%s\n", commandlist);


    t = ft_split(commandlist, '|');

    // Now you can process each command in 't' and create 'cmds' correctly.
	cmds = (char ***)malloc(sizeof(char **));
	while (*t[i])
	{
		cmds[i] = (char **)malloc(sizeof(char *));
		cmds[i] = addcommand(t[i]);
		i++;
	}
	// int num_cmds = 2;
	// execute_pipes_with_io_redirection(input_file, output_file, cmds,
	// num_cmds);



    // Free allocated memory.
    free(commandlist);
    free(input_file);
    free(output_file);
    // Free memory allocated for 'input_command' and 'cmds'.

    return 0;
}

// int	main(void)
// {
// 	char	*input_command2;
// 	char	**input_command;
// 	char	*input_file;
// 	char	*output_file;
// 	char	*commandlist;
// 	int		i;
// 	char	***cmds;
// 	char	**t;

// 	input_command2 = "cat < input.txt | grep 'pattern' > output.txt";
// 	input_command = ft_split(input_command2, ' ');
// 	i = 0;
// 	commandlist = NULL;
// 	commandlist = malloc(ft_strlen(input_command[i]));
// 	while (input_command[i])
// 	{
// 		if (ft_strncmp(input_command[i], "<", 1) == 0)
// 		{
// 			input_file = (char *)malloc(sizeof(char)
// 				* (ft_strlen(input_command[i + 1])));
// 			// printf("%s", input_command[i + 1]);
// 			ft_memcpy(input_file, input_command[i + 1],
// 				ft_strlen(input_command[i + 1]));
// 			i++;
// 		}
// 		else if (ft_strncmp(input_command[i], ">", 1) == 0)
// 		{
// 			output_file = (char *)malloc(sizeof(char)
// 				* (ft_strlen(input_command[i + 1])));
// 			// printf("%s", input_command[i + 1]);
// 			ft_memcpy(output_file, input_command[i + 1],
// 				ft_strlen(input_command[i + 1]));
// 			i++;
// 		}
// 		else
// 		{
// 			commandlist = ft_strjoin(commandlist, input_command[i]);
// 			commandlist = ft_strjoin(commandlist, " ");
// 		}
// 		i++;
// 	}
// 	printf("%s", commandlist);
// 	t = ft_split(commandlist, '|');
// 	cmds = (char ***)malloc(sizeof(char **));
// 	while (*t[i])
// 	{
// 		cmds[i] = (char **)malloc(sizeof(char *));
// 		cmds[i] = addcommand(t[i]);
// 		i++;
// 	}
// 	// int num_cmds = 2;
// 	// execute_pipes_with_io_redirection(input_file, output_file, cmds,
// 	// num_cmds);
// 	return (0);
// }
