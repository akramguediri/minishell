/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:30:09 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:08:29 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd_command(char *current_directory)
{
	char	**path_parts;
	char	*new_path;

	path_parts = ft_split(current_directory, '/');
	if (path_parts[0] && path_parts[1])
	{
		new_path = ft_strjoin("/", path_parts[0]);
		new_path = ft_strjoin(new_path, "/");
		new_path = ft_strjoin(new_path, path_parts[1]);
		new_path = ft_strjoin(new_path, "/");
		change_directory(new_path);
		free(new_path);
	}
}

void	cd(char *input)
{
	char	**tokens;
	char	*current_directory;

	tokens = ft_split(input, ' ');
	if (!tokens)
	{
		return ;
	}
	current_directory = get_current_directory();
	if (tokens[1])
	{
		handle_dotdot(tokens[1], current_directory);
	}
	else if (!ft_strncmp(input, "cd", 2))
	{
		handle_cd_command(current_directory);
	}
	free(current_directory);
}

int	handle_clear(void)
{
	custom_clear();
	return (1);
}

int	handle_pwd(t_data *data)
{
	ft_getactivepath(data, 1);
	printf("\n");
	return (1);
}
