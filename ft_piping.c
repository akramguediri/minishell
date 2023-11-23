/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/22 22:01:44 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_heredoc(char *s)
{
	int		heredoc_pipe[2];
	pid_t	cat_pid;

	if (pipe(heredoc_pipe) == -1)
		error_exit("pipe");
	cat_pid = fork();
	if (cat_pid == -1)
		error_exit("fork");
	else if (cat_pid == 0)
	{
		close(heredoc_pipe[0]);
		if (write(heredoc_pipe[1], s, strlen(s)) == -1)
			error_exit("write");
		close(heredoc_pipe[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
		close(heredoc_pipe[0]);
		if (waitpid(cat_pid, NULL, 0) == -1)
			error_exit("write pid");
	}
}

void	create_pipes(int num_pipes, int **pipe_fd)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		if (pipe(pipe_fd[i]) == -1)
			error_exit("Pipe creation failed");
		i++;
	}
}

void	redirect_input(int i, char *input_file, int **pipe_fd)
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
			error_exit("Failed to open input file");
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
}

void	redirect_output(int i, struct s_cmd_data cmddata, int **pipe_fd)
{
	int	output_fd;

	if (i < cmddata.num_cmds - 1)
	{
		dup2(pipe_fd[i][1], STDOUT_FILENO);
		close(pipe_fd[i][1]);
	}
	else if (cmddata.output_file)
	{
		if (cmddata.r == 1)
			output_fd = open(cmddata.output_file, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else
			output_fd = open(cmddata.output_file, O_CREAT | O_TRUNC | O_WRONLY,
					0644);
		if (output_fd < 0)
		{
			perror("Failed to open output file");
			exit(EXIT_FAILURE);
		}
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

int	execute_command2(char *cmd, t_data *data, t_cmd_hist *h, int z)
{
	char	*token;
	char	*args[64];
	int		arg_count;
	char	*cmd2;
	int		r;

	r = z;
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
	if (handle_command(cmd2, data, h, z) == 0)
	{
		z = ft_execvp(args[0], args);
		return (r);
	}
	return (EXIT_SUCCESS);
}

void	wait_for_children(int num_cmds, pid_t children[], int z)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(children[i], &z, 0);
		i++;
	}
}

void	pipes_io_redir(struct s_cmd_data cmddata, t_data *data, t_cmd_hist *h)
{
	int		**pipe_fd;
	pid_t	*children;
	char	**argv;
	int		i;
	int		j;
	int		k;

	argv = (char **)ft_calloc(3, sizeof(char *));
	i = 0;
	children = (pid_t *)ft_calloc(cmddata.num_cmds, sizeof(pid_t));
	pipe_fd = (int **)ft_calloc((cmddata.num_cmds - 1), sizeof(int *));
	while (i < cmddata.num_cmds - 1)
	{
		pipe_fd[i] = ft_calloc(2, sizeof(int));
		i++;
	}
	create_pipes(cmddata.num_cmds - 1, pipe_fd);
	i = 0;
	while (i < cmddata.num_cmds)
	{
		children[i] = fork();
		if (children[i] < 0)
			error_exit("Fork failed");
		if (children[i] == 0)
		{
			redirect_input(i, cmddata.input_file, pipe_fd);
			redirect_output(i, cmddata, pipe_fd);
			j = 0;
			while (j < cmddata.num_cmds - 1)
			{
				close(pipe_fd[j][0]);
				close(pipe_fd[j][1]);
				j++;
			}
			if (cmddata.heredoc)
			{
				argv[0] = ft_strdup("echo");
				argv[1] = ft_strdup(cmddata.heredoc);
				argv[2] = NULL;
				ft_execvp("echo", argv);
			}
			cmddata.z = execute_command2(cmddata.t[i], data, h, cmddata.z);
			exit(EXIT_SUCCESS);
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
	wait_for_children(cmddata.num_cmds, children, cmddata.z);
}

char	*process_command(char *command, char *input_file, char *output_file)
{
	int		len;
	char	*processed;
	int		i[2];

	len = ft_strlen(command);
	processed = (char *)ft_calloc(len + 1, 1);
	i[0] = 0;
	i[1] = 0;
	while (i[0] < len)
	{
		if (command[i[0]] == '<')
		{
			i[0]++;
			input_file = extract_input_file(command, &i[0], len);
		}
		else if (command[i[0]] == '>')
		{
			i[0]++;
			output_file = extract_output_file(command, &i[0], len);
		}
		else
			processed[i[1]++] = command[i[0]++];
	}
	return (processed[i[1]] = '\0', free(input_file), free(output_file),
		processed);
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

void	initialize_s_cmd_data(struct s_cmd_data *cmddata, char *input2)
{
	cmddata->r = 0;
	cmddata->here = 0;
	if (input2)
		cmddata->input = ft_split(input2, ' ');
	else
		cmddata->input = NULL;
	cmddata->input_file = NULL;
	cmddata->output_file = NULL;
	cmddata->commandlist = NULL;
	cmddata->heredoc = NULL;
	cmddata->commandlist = ft_calloc(1, 1);
	cmddata->commandlist[0] = '\0';
}

void	process_s_cmd_data(struct s_cmd_data *cmddata, char *input2)
{
	int	i;

	i = 0;
	initialize_s_cmd_data(cmddata, input2);
	while (cmddata->input[i] && cmddata->input)
	{
		if (ft_strnstr(cmddata->input[i], "<<", 2) != 0)
			cmddata->here = 2;
		else if (ft_strncmp(cmddata->input[i], "<", 1) == 0)
			cmddata->input_file = process_input_file(cmddata->input, &i);
		else if (ft_strncmp(cmddata->input[i], ">", 1) == 0
			|| ft_strncmp(cmddata->input[i], ">>", 2) == 0)
			cmddata->output_file = process_output_file(cmddata->input, &i,
					&(cmddata->r));
		else
			cmddata->processed = process_regular_command(cmddata,
					cmddata->input[i]);
		if (cmddata->here == 2 && (!ft_strlen(ft_strtrim(cmddata->input[i],
						"<"))))
			process_heredoc(cmddata, cmddata->input[i + 1]);
		else if (cmddata->here == 2)
			process_heredoc(cmddata, ft_strtrim(cmddata->input[i], "<"));
		i++;
	}
}

char	*process_input_file(char **input, int *i)
{
	char	*input_file;

	input_file = NULL;
	if (input[*i + 1])
	{
		input_file = ft_strdup(input[*i + 1]);
		(*i)++;
	}
	return (input_file);
}

char	*outputvar(char *s)
{
	int		i;
	char	**t;

	i = 0;
	t = NULL;
	while (environ[i])
	{
		if (ft_strnstr(environ[i], s, ft_strlen(s)))
		{
			t = ft_split(environ[i], '=');
			return (t[1]);
		}
		i++;
	}
	if (!t)
		perror("ambiguous redirect");
	return ("");
}

char	*process_output_file(char **input, int *i, int *r)
{
	char	*output_file;
	char	*var;

	output_file = NULL;
	var = NULL;
	if (ft_strncmp(input[*i], ">>", 2) == 0)
		*r = 1;
	if (input[*i + 1])
	{
		output_file = ft_strdup(input[*i + 1]);
		(*i)++;
	}
	if (ft_strchr(output_file, '$'))
	{
		var = ft_strdup(outputvar(ft_strtrim(output_file, "$")));
		if (!var)
			perror("ambiguous redirect");
	}
	if (var)
		return (var);
	else
		return (output_file);
}

char	*process_regular_command(struct s_cmd_data *cmddata, char *command)
{
	char	*input_file;
	char	*output_file;

	input_file = NULL;
	output_file = NULL;
	cmddata->processed = process_command(command, input_file, output_file);
	if (cmddata->processed)
	{
		cmddata->commandlist = ft_strjoin(cmddata->commandlist,
				cmddata->processed);
		cmddata->commandlist = ft_strjoin(cmddata->commandlist, " ");
		free(cmddata->processed);
	}
	return ((cmddata->commandlist));
}

void	process_heredoc(struct s_cmd_data *cmddata, char *input)
{
	cmddata->here = 2;
	cmddata->heredoc = heredoc(input);
	cmddata->here = 0;
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

int	commandd(char *input2, t_data *data, t_cmd_hist *h, int z)
{
	struct s_cmd_data	cmddata;

	cmddata.z = z;
	process_s_cmd_data(&cmddata, ft_strtrim(input2, "|"));
	cmddata.t = ft_splitonsteroids(cmddata.commandlist, '|');
	cmddata.num_cmds = count_characters(cmddata.commandlist, '|')
		- countpipes(cmddata.t, '|') + 1;
	pipes_io_redir(cmddata, data, h);
	free(cmddata.commandlist);
	free(cmddata.input_file);
	free(cmddata.output_file);
	return (0);
}
