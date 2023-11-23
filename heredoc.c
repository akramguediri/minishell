/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:46:16 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/23 15:50:57 by otuyishi         ###   ########.fr       */
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

void	process_heredoc(struct s_cmd_data *cmddata, char *input)
{
	cmddata->here = 2;
	cmddata->heredoc = heredoc(input);
	cmddata->here = 0;
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size, char **new_ptr)
{
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
	{
		perror("Memory allocation error");
		return (NULL);
	}
	if (ptr != NULL)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}

char	*heredoc(const char *delimiter)
{
	size_t	ints[2];
	char	*s[3];

	ints[1] = 256;
	s[0] = (char *)ft_calloc(ints[1], 1);
	while (1)
	{
		s[1] = readline("> ");
		if (s[1] == NULL)
			return (perror("Error reading input"), free(s[0]), NULL);
		if (ft_strncmp(s[1], delimiter, ft_strlen(delimiter)) == 0)
			return (free(s[1]), s[2]);
		ints[0] = ft_strlen(s[0]) + ft_strlen(s[1]) + ft_strlen(delimiter) + 1;
		if (ints[0] > ints[1] && !ft_realloc(s[0], ints[1], ints[1] * 2, &s[0]))
			return (perror("allocation error"), free(s[1]), free(s[0]), NULL);
		s[2] = ft_strjoin(ft_strjoin(s[0], s[1]), "\n");
		if (s[2] == NULL)
			return (free(s[1]), NULL);
		free(s[0]);
		s[0] = s[2];
		free(s[1]);
	}
	system("leaks minishell");
	return (s[2]);
}

void	executehere(char *s)
{
	char	**argv;

	argv = (char **)ft_calloc(3, sizeof(char *));
	argv[0] = ft_strdup("echo");
	argv[1] = ft_strdup(s);
	argv[2] = NULL;
	ft_execvp("echo", argv);
}
