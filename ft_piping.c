/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/16 22:52:15 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	execute_command2(char *cmd, t_data *data, t_cmd_hist *h)
{
	char	*token;
	char	*args[64];
	int		arg_count;
	char	*cmd2;
	
	arg_count = 0;
	if (cmd)
		cmd2 = ft_strdup(cmd);
	token = strtok(cmd, " ");
	while (token != NULL)
	{
		args[arg_count++] = token;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;
	if (handle_command(cmd2, data, h) == 0)
	{
		ft_execvp(args[0], args);
		perror("failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
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

void	execute_pipes_with_io_redirection(struct CommandData cmddata,
		t_data *data, t_cmd_hist *h)
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
			redirect_output(i, cmddata.num_cmds, cmddata.output_file, cmddata.r,
				pipe_fd);
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
			i++; // Move past the '<' character
			while (i < len && command[i] == ' ')
			{
				i++; // Skip spaces
			}
			if (i < len)
			{
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
	if (input_file != NULL)
    	free(input_file);
	if (output_file != NULL)
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

void	process_command_data(struct CommandData *cmddata, char *input_command2)
{
	int	i;

	cmddata->r = 0;
	cmddata->input_command = ft_splitonsteroids(input_command2, ' ');
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
				cmddata->input_file = ft_strdup(cmddata->input_command[i + 1]);
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
				cmddata->output_file = ft_strdup(cmddata->input_command[i + 1]);
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
