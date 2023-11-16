/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:28:27 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/16 16:10:11 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execvp(const char *file, char *const argv[])
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	*full_path;
	size_t	len;

	if (ft_strchr(file, '/') != NULL)
	{
		return (execve(file, argv, environ));
	}
	path = getenv("PATH");
	if (path == NULL)
	{
		perror("getenv");
		return (-1);
	}
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		perror("strdup");
		return (-1);
	}
	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + strlen(file) + 2;
		full_path = (char *)malloc(len);
		if (full_path == NULL)
		{
			perror("malloc");
			free(path_copy);
			return (-1);
		}
		snprintf(full_path, len, "%s/%s", dir, file);
		if (execve(full_path, argv, environ) != -1)
		{
			free(path_copy);
			free(full_path);
			return (-1);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (-1);
}
