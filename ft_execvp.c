/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:28:27 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/18 17:56:49 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execvp(const char *file, char *const argv[])
{
	char	*path_copy;
	int		result;

	if (ft_strchr(file, '/') != NULL)
	{
		return (execve(file, argv, environ));
	}
	path_copy = get_path_copy();
	if (path_copy == NULL)
	{
		perror("get_path_copy");
		return (-1);
	}
	result = execute_with_path(file, argv, path_copy);
	free(path_copy);
	return (result);
}

int	execute_with_path(const char *file, char *const argv[], char *path_copy)
{
	char	*dir;
	char	*full_path;

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = build_full_path(dir, file);
		if (full_path == NULL)
		{
			perror("build_full_path");
			free_resources(path_copy, NULL);
			return (-1);
		}
		if (execve(full_path, argv, environ) != -1)
		{
			free_resources(path_copy, full_path);
			return (-1);
		}
		free_resources(NULL, full_path);
		dir = strtok(NULL, ":");
	}
	return (-1);
}

char	*build_full_path(const char *dir, const char *file)
{
	size_t	len;
	char	*full_path;

	len = strlen(dir) + strlen(file) + 2;
	full_path = (char *)malloc(len);
	if (full_path == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	snprintf(full_path, len, "%s/%s", dir, file);
	return (full_path);
}

char	*get_path_copy(void)
{
	char	*path;
	char	*path_copy;

	path = getenv("PATH");
	if (path == NULL)
	{
		perror("getenv");
		return (NULL);
	}
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		perror("strdup");
		return (NULL);
	}
	return (path_copy);
}

void	free_resources(char *path_copy, char *full_path)
{
	if (path_copy != NULL)
	{
		free(path_copy);
	}
	if (full_path != NULL)
	{
		free(full_path);
	}
}
