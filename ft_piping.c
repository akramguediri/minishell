/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/10/30 16:23:46 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	execute_pipes_with_io_redirection(char *input_file, char *output_file,
// 		char *cmds[], int num_cmds, int r)
// {
// 	int		pipe_fd[num_cmds - 1][2];
// 	pid_t	children[num_cmds];
// 	int		input_fd;
// 	int		output_fd;
// 	int		i;
// 	int		j;
// 	char	*token;
// 			char *args[64];
// 	int		arg_count;

// 	i = 0;
// 	while (i < num_cmds - 1)
// 	{
// 		if (pipe(pipe_fd[i]) == -1)
// 		{
// 			perror("Pipe creation failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < num_cmds)
// 	{
// 		children[i] = fork();
// 		if (children[i] < 0)
// 		{
// 			perror("Fork failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		if (children[i] == 0)
// 		{
// 			if (i > 0)
// 			{
// 				dup2(pipe_fd[i - 1][0], STDIN_FILENO);
// 				close(pipe_fd[i - 1][0]);
// 			}
// 			else if (input_file)
// 			{
// 				input_fd = open(input_file, O_RDONLY);
// 				if (input_fd < 0)
// 				{
// 					perror("Failed to open input file");
// 					exit(EXIT_FAILURE);
// 				}
// 				dup2(input_fd, STDIN_FILENO);
// 				close(input_fd);
// 			}
// 			if (i < num_cmds - 1)
// 			{
// 				dup2(pipe_fd[i][1], STDOUT_FILENO);
// 				close(pipe_fd[i][1]);
// 			}
// 			else if (output_file)
// 			{
// 				if (r == 1)
// 					output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND,
// 					0644);
// 				else
// 					output_fd = open(output_file,O_CREAT | O_TRUNC| O_WRONLY,
//						0644);
// 				if (output_fd < 0)
// 				{
// 					perror("Failed to open output file");
// 					exit(EXIT_FAILURE);
// 				}
// 				dup2(output_fd, STDOUT_FILENO);
// 				close(output_fd);
// 			}
// 			j = 0;
// 			while (j < num_cmds - 1)
// 			{
// 				close(pipe_fd[j][0]);
// 				close(pipe_fd[j][1]);
// 				j++;
// 			}
// 			token = strtok(cmds[i], " ");
// 			arg_count = 0;
// 			while (token != NULL)
// 			{
// 				args[arg_count++] = token;
// 				token = strtok(NULL, " ");
// 			}
// 			args[arg_count] = NULL;
// 			execvp(args[0], args);
// 			perror("Execvp failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < num_cmds - 1)
// 	{
// 		close(pipe_fd[i][0]);
// 		close(pipe_fd[i][1]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < num_cmds)
// 	{
// 		waitpid(children[i], NULL, 0);
// 		i++;
// 	}
// }
void	create_pipes(int num_pipes, int pipe_fd[][2])
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		if (pipe(pipe_fd[i]) == -1)
		{
			perror("Pipe creation failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	redirect_input(int i, char *input_file, int pipe_fd[][2])
{
	int	input_fd;

	if (i > 0)
	{
		// Redirect input from the previous pipe
		dup2(pipe_fd[i - 1][0], STDIN_FILENO);
		close(pipe_fd[i - 1][0]);
	}
	else if (input_file)
	{
		// Redirect input from a file
		input_fd = open(input_file, O_RDONLY);
		if (input_fd < 0)
		{
			perror("Failed to open input file");
			exit(EXIT_FAILURE);
		}
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
}

void	redirect_output(int i, int num_cmds, char *output_file, int r,
		int pipe_fd[][2])
{
	int	output_fd;

	if (i < num_cmds - 1)
	{
		// Redirect output to the next pipe
		dup2(pipe_fd[i][1], STDOUT_FILENO);
		close(pipe_fd[i][1]);
	}
	else if (output_file)
	{
		// Redirect output to a file
		if (r == 1)
			output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			output_fd = open(output_file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (output_fd < 0)
		{
			perror("Failed to open output file");
			exit(EXIT_FAILURE);
		}
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

void	execute_command2(char *cmd, t_data *data, t_cmd_hist *h)
{
	char	*token;
	char	*args[64];
	int		arg_count;

	arg_count = 0;
	// if (strcmp(cmd, "clear") == 0)
	// 	custom_clear();
	// else if (ft_strnstr(cmd, "cd", 2) != 0)
	// 	cd(cmd);
	// else if (ft_strnstr(cmd, "echo ", 5) != 0)
	// 	echo(cmd);
	handle_command(cmd, data, h);
	token = strtok(cmd, " ");
	while (token != NULL)
	{
		args[arg_count++] = token;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;
	execvp(args[0], args);
	perror("Execvp failed");
	exit(EXIT_FAILURE);
}

void	wait_for_children(int num_cmds, pid_t children[])
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(children[i], NULL, 0);
		i++;
	}
}

void	execute_pipes_with_io_redirection(struct CommandData	cmddata, t_data *data, t_cmd_hist *h)
{
	int		pipe_fd[cmddata.num_cmds - 1][2];
	pid_t	children[cmddata.num_cmds];
	int		i;
	int		j;
	int		k;

	create_pipes(cmddata.num_cmds - 1, pipe_fd);
	i = 0;
	while (i < cmddata.num_cmds)
	{
		children[i] = fork();
		if (children[i] < 0)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (children[i] == 0)
		{
			redirect_input(i, cmddata.input_file, pipe_fd);
			redirect_output(i, cmddata.num_cmds, cmddata.output_file, cmddata.r, pipe_fd);
			j = 0;
			while (j < cmddata.num_cmds - 1)
			{
				close(pipe_fd[j][0]);
				close(pipe_fd[j][1]);
				j++;
			}
			execute_command2(cmddata.t[i], data, h);
		}
		i++;
	}
	k = 0;
	while (k < cmddata.num_cmds - 1)
	{
		close(pipe_fd[k][0]);
		close(pipe_fd[k][1]);
		k++;
	}
	wait_for_children(cmddata.num_cmds, children);
}

// void execute_pipes_with_io_redirection(char *input_file, char *output_file,
//	char *cmds[], int num_cmds, int r) {
//     int pipe_fd[num_cmds - 1][2];
//     pid_t children[num_cmds];

//     create_pipes(num_cmds - 1, pipe_fd);

//     int i = 0;
//     while (i < num_cmds) {
//         children[i] = fork();
//         if (children[i] < 0) {
//             perror("Fork failed");
//             exit(EXIT_FAILURE);
//         }
//         if (children[i] == 0) {
//             redirect_input(i, num_cmds, input_file, pipe_fd);
//             redirect_output(i, num_cmds, output_file, r, pipe_fd);
//             int j = 0;
//             while (j < num_cmds - 1) {
//                 close(pipe_fd[j][0]);
//                 close(pipe_fd[j][1]);
//                 j++;
//             }
//             execute_command(cmds[i]);
//         }
//         i++;
//     }

//     int k = 0;
//     while (k < num_cmds - 1) {
//         close(pipe_fd[k][0]);
//         close(pipe_fd[k][1]);
//         k++;
//     }

//     wait_for_children(num_cmds, children);
// }

char	*process_command(char *command)
{
	int		len;
	char	*input_file;
	char	*output_file;
	int		i;
	int		j;
	char	*processed;

	if (command == NULL)
	{
		return (NULL);
	}
	len = strlen(command);
	processed = (char *)malloc(len + 1);
	input_file = NULL;
	output_file = NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		if (command[i] == '<')
		{
			// Handle input redirection, e.g., "cat < input.txt"
			i++; // Move past the '<' character
			while (i < len && command[i] == ' ')
			{
				i++; // Skip spaces
			}
			if (i < len)
			{
				// Extract the input file name
				while (i < len && command[i] != ' ' && command[i] != '>')
				{
					input_file = (char *)realloc(input_file, j + 2);
					input_file[j++] = command[i];
					i++;
				}
				input_file[j] = '\0';
			}
		}
		else if (command[i] == '>')
		{
			i++; // Move past the '>' character
			while (i < len && command[i] == ' ')
				i++; // Skip spaces
			if (i < len)
			{
				while (i < len && command[i] != ' ' && command[i] != '<')
				{
					output_file = (char *)realloc(output_file, j + 2);
					output_file[j++] = command[i];
					i++;
				}
				output_file[j] = '\0';
			}
		}
		else
		{
			processed[j++] = command[i];
			i++;
		}
	}
	processed[j] = '\0';
	free(input_file);
	free(output_file);
	return (processed);
}
int	count_characters(const char *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == c)
		{
			count++;
		}
		i++;
	}
	return (count);
}

// int	commandd(char *input_command2)
// {
// 	char	**input_command;
// 	int		i;
// 	char	*input_file;
// 	char	*output_file;
// 	char	*commandlist;
// 	char	**t;
// 	char	*processed;
// 	int		num_cmds;
// 	int		r;

// 	r = 0;
// 	input_command = ft_split(input_command2, ' ');
// 	i = 0;
// 	input_file = NULL;
// 	output_file = NULL;
// 	commandlist = NULL;
// 	commandlist = malloc(1);
// 	commandlist[0] = '\0';
// 	while (input_command[i])
// 	{
// 		if (ft_strncmp(input_command[i], "<", 1) == 0)
// 		{
// 			if (input_command[i + 1])
// 			{
// 				input_file = strdup(input_command[i + 1]);
// 				i++;
// 			}
// 		}
// 		else if (ft_strncmp(input_command[i], ">", 1) == 0
// 			|| ft_strncmp(input_command[i], ">>", 2) == 0)
// 		{
// 			if (ft_strncmp(input_command[i], ">>", 2) == 0)
// 				r = 1;
// 			if (input_command[i + 1])
// 			{
// 				output_file = strdup(input_command[i + 1]);
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			processed = process_command(input_command[i]);
// 			if (processed)
// 			{
// 				commandlist = ft_strjoin(commandlist, processed);
// 				commandlist = ft_strjoin(commandlist, " ");
// 				free(processed);
// 			}
// 		}
// 		i++;
// 	}
// 	// cmds = (char ***)malloc(sizeof(char **) * 2);
// 	t = ft_split(commandlist, '|');
// 	num_cmds = count_characters(commandlist, '|') + 1;
// 	execute_pipes_with_io_redirection(input_file, output_file, t, num_cmds, r);
// 	// Free allocated memory.
// 	free(commandlist);
// 	free(input_file);
// 	free(output_file);
// 	// Free memory allocated for 'input_command' and 'cmds'.
// 	return (0);
// }

// Function to process the command.
void	process_command_data(struct CommandData *cmddata, char *input_command2)
{
	int	i;

	cmddata->r = 0;
	cmddata->input_command = ft_split(input_command2, ' ');
	i = 0;
	cmddata->input_file = NULL;
	cmddata->output_file = NULL;
	cmddata->commandlist = NULL;
	cmddata->commandlist = malloc(1);
	cmddata->commandlist[0] = '\0';
	while (cmddata->input_command[i])
	{
		if (ft_strncmp(cmddata->input_command[i], "<", 1) == 0)
		{
			if (cmddata->input_command[i + 1])
			{
				cmddata->input_file = strdup(cmddata->input_command[i + 1]);
				i++;
			}
		}
		else if (ft_strncmp(cmddata->input_command[i], ">", 1) == 0
			|| ft_strncmp(cmddata->input_command[i], ">>", 2) == 0)
		{
			if (ft_strncmp(cmddata->input_command[i], ">>", 2) == 0)
				cmddata->r = 1;
			if (cmddata->input_command[i + 1])
			{
				cmddata->output_file = strdup(cmddata->input_command[i + 1]);
				i++;
			}
		}
		else
		{
			cmddata->processed = process_command(cmddata->input_command[i]);
			if (cmddata->processed)
			{
				cmddata->commandlist = ft_strjoin(cmddata->commandlist,
					cmddata->processed);
				cmddata->commandlist = ft_strjoin(cmddata->commandlist, " ");
				free(cmddata->processed);
			}
		}
		i++;
	}
}

int	commandd(char *input_command2, t_data *data, t_cmd_hist *h)
{
	struct CommandData	cmddata;

	process_command_data(&cmddata, input_command2);
	cmddata.t = ft_split(cmddata.commandlist, '|');
	cmddata.num_cmds = count_characters(cmddata.commandlist, '|') + 1;
	execute_pipes_with_io_redirection(cmddata, data, h);
	free(cmddata.commandlist);
	free(cmddata.input_file);
	free(cmddata.output_file);
	return (0);
}
