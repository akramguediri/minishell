/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:47:13 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:40:28 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
