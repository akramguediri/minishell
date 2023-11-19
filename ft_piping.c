/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/19 17:28:26 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_heredoc(char *heredoc_content)
{
	int		heredoc_pipe[2];
	pid_t	cat_pid;

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	cat_pid = fork();
	if (cat_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (cat_pid == 0)
	{
		close(heredoc_pipe[0]);
		if (write(heredoc_pipe[1], heredoc_content, strlen(heredoc_content)) ==
			-1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
		close(heredoc_pipe[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
		close(heredoc_pipe[0]);
		if (waitpid(cat_pid, NULL, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}

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
		dup2(pipe_fd[i][1], STDOUT_FILENO);
		close(pipe_fd[i][1]);
	}
	else if (output_file)
	{
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
			if (cmddata.heredoc)
			{
				char *const argv[] = {"echo", cmddata.heredoc, NULL};
				ft_execvp("echo", argv);
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
	char	*processed;
	char	*input_file;
	char	*output_file;
	int		i;
	int		j;

	if (command == NULL)
		return (NULL);
	len = ft_strlen(command);
	processed = (char *)malloc(len + 1);
	input_file = NULL;
	output_file = NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		if (command[i] == '<')
		{
			i++;
			input_file = extract_input_file(command, &i, len);
		}
		else if (command[i] == '>')
		{
			i++;
			output_file = extract_output_file(command, &i, len);
		}
		else
			processed[j++] = command[i++];
	}
	processed[j] = '\0';
	free(input_file);
	free(output_file);
	return (processed);
}

char	*extract_input_file(char *command, int *i, int len)
{
	int		j;
	char	*input_file;

	while (*i < len && command[*i] == ' ')
	{
		(*i)++;
	}
	if (*i < len)
	{
		j = 0;
		input_file = (char *)malloc(1);
		while (*i < len && command[*i] != ' ' && command[*i] != '>')
		{
			input_file = (char *)realloc(input_file, j + 2);
			input_file[j++] = command[(*i)++];
		}
		input_file[j] = '\0';
		return (input_file);
	}
	return (NULL);
}

char	*extract_output_file(char *command, int *i, int len)
{
	int		j;
	char	*output_file;

	while (*i < len && command[*i] == ' ')
	{
		(*i)++;
	}
	if (*i < len)
	{
		j = 0;
		output_file = (char *)malloc(1);
		while (*i < len && command[*i] != ' ' && command[*i] != '<')
		{
			output_file = (char *)realloc(output_file, j + 2);
			output_file[j++] = command[(*i)++];
		}
		output_file[j] = '\0';
		return (output_file);
	}
	return (NULL);
}

char	*extract_regular_chars(char *command, int *i, int len)
{
	int		j;
	char	*regular_chars;

	j = 0;
	regular_chars = (char *)malloc(len + 1);
	while (*i < len && command[*i] != '<' && command[*i] != '>')
	{
		regular_chars[j++] = command[(*i)++];
	}
	regular_chars[j] = '\0';
	return (regular_chars);
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

void	initialize_command_data(struct CommandData *cmddata,
		char *input_command2)
{
	cmddata->r = 0;
	cmddata->here = 0;
	cmddata->input_command = ft_split(input_command2, ' ');
	cmddata->input_file = NULL;
	cmddata->output_file = NULL;
	cmddata->commandlist = NULL;
	cmddata->heredoc = NULL;
	cmddata->commandlist = malloc(1);
	cmddata->commandlist[0] = '\0';
}

void	process_command_data(struct CommandData *cmddata, char *input_command2)
{
	int	i;

	i = 0;
	initialize_command_data(cmddata, input_command2);
	while (cmddata->input_command[i])
	{
		if (ft_strnstr(cmddata->input_command[i], "<<", 2) != 0)
			cmddata->here = 2;
		else if (ft_strncmp(cmddata->input_command[i], "<", 1) == 0)
			cmddata->input_file = process_input_file(cmddata->input_command,
					&i);
		else if (ft_strncmp(cmddata->input_command[i], ">", 1) == 0
			|| ft_strncmp(cmddata->input_command[i], ">>", 2) == 0)
			cmddata->output_file = process_output_file(cmddata->input_command,
					&i, &(cmddata->r));
		else
			cmddata->processed = process_regular_command(cmddata,
					cmddata->input_command[i]);
		if (cmddata->here == 2)
		{
			process_heredoc(cmddata, cmddata->input_command[i]);
			cmddata->here = 0;
		}
		i++;
	}
}

char	*process_input_file(char **input_command, int *i)
{
	char	*input_file;

	input_file = NULL;
	if (input_command[*i + 1])
	{
		input_file = ft_strdup(input_command[*i + 1]);
		(*i)++;
	}
	return (input_file);
}

char	*process_output_file(char **input_command, int *i, int *r)
{
	char	*output_file;

	output_file = NULL;
	if (ft_strncmp(input_command[*i], ">>", 2) == 0)
	{
		*r = 1;
	}
	if (input_command[*i + 1])
	{
		output_file = ft_strdup(input_command[*i + 1]);
		(*i)++;
	}
	return (output_file);
}

char	*process_regular_command(struct CommandData *cmddata, char *command)
{
	cmddata->processed = process_command(command);
	cmddata->processed = command;
	if (cmddata->processed)
	{
		cmddata->commandlist = ft_strjoin(cmddata->commandlist,
				cmddata->processed);
		cmddata->commandlist = ft_strjoin(cmddata->commandlist, " ");
		free(cmddata->processed);
	}
	return (cmddata->processed);
}

void	process_heredoc(struct CommandData *cmddata, char *input_command)
{
	cmddata->here = 2;
	cmddata->heredoc = heredoc(ft_strtrim(input_command, "<"));
}

int	countpipes(char **t, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (t[i])
	{
		j += count_characters(t[i], c);
		i++;
	}
	return (j);
}

int	commandd(char *input_command2, t_data *data, t_cmd_hist *h)
{
	struct CommandData	cmddata;

	process_command_data(&cmddata, input_command2);
	cmddata.t = ft_splitonsteroids(cmddata.commandlist, '|');
	cmddata.num_cmds = count_characters(cmddata.commandlist, '|')
		- countpipes(cmddata.t, '|') + 1;
	execute_pipes_with_io_redirection(cmddata, data, h);
	free(cmddata.commandlist);
	free(cmddata.input_file);
	free(cmddata.output_file);
	return (0);
}
