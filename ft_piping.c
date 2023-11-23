/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 15:58:59 by otuyishi         ###   ########.fr       */
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

void	run_child_process(struct s_cmd_data cmddata, t_data *data,
		t_cmd_hist *h, int **pipe_fd)
{
	int	k1;

	redirect_input(cmddata.k, cmddata.input_file, pipe_fd);
	redirect_output(cmddata.k, cmddata, pipe_fd);
	k1 = 0;
	while (k1 < cmddata.num_cmds - 1)
	{
		close(pipe_fd[k1][0]);
		close(pipe_fd[k1][1]);
		k1++;
	}
	if (cmddata.heredoc)
		executehere(cmddata.heredoc);
	cmddata.z = execute_command2(cmddata.t[cmddata.k], data, h, cmddata.z);
	exit(EXIT_SUCCESS);
}

void	close_pipes(int num_cmds, int **pipe_fd)
{
	int	k;

	k = 0;
	while (k < num_cmds - 1)
	{
		close(pipe_fd[k][0]);
		close(pipe_fd[k][1]);
		k++;
	}
}

void	pipes_io_redir(struct s_cmd_data cmddata, t_data *data, t_cmd_hist *h)
{
	int		**pipe_fd;
	pid_t	*children;

	cmddata.k = 0;
	children = (pid_t *)ft_calloc(cmddata.num_cmds, sizeof(pid_t));
	pipe_fd = (int **)ft_calloc((cmddata.num_cmds - 1), sizeof(int *));
	while (cmddata.k < cmddata.num_cmds - 1)
	{
		pipe_fd[cmddata.k] = ft_calloc(2, sizeof(int));
		cmddata.k++;
	}
	create_pipes(cmddata.num_cmds - 1, pipe_fd);
	cmddata.k = 0;
	while (cmddata.k < cmddata.num_cmds)
	{
		children[cmddata.k] = fork();
		if (children[cmddata.k] < 0)
			error_exit("Fork failed");
		if (children[cmddata.k] == 0)
			run_child_process(cmddata, data, h, pipe_fd);
		cmddata.k++;
	}
	close_pipes(cmddata.num_cmds, pipe_fd);
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
