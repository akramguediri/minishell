/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:33:06 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:09:44 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_directory(void)
{
	size_t	path_max;
	char	*current_directory;

	path_max = pathconf(".", _PC_PATH_MAX);
	current_directory = (char *)malloc(path_max);
	if (!current_directory)
	{
		return (NULL);
	}
	return (getcwd(current_directory, path_max));
}

char	*get_parent_path(const char *path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		i++;
	}
	while (i > 0 && path[i - 1] != '/')
	{
		i--;
	}
	return (ft_substr(path, 0, i));
}

void	change_directory(const char *new_path)
{
	chdir(new_path);
}

void	handle_dotdot(const char *input, char *current_directory)
{
	char	*trimmed_path;
	char	*new_path;

	if (strnstr(input, "..", 2) != 0)
	{
		current_directory = get_parent_path(current_directory);
	}
	trimmed_path = ft_strtrim(input, "..");
	if (trimmed_path)
	{
		new_path = ft_strjoin(current_directory, "/");
		new_path = ft_strjoin(new_path, trimmed_path);
		change_directory(new_path);
		free(new_path);
		free(trimmed_path);
	}
}
