/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:17:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/10/17 19:28:03 by otuyishi         ###   ########.fr       */
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
