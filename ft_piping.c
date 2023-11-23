/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:09:32 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
