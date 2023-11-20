/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:46:16 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/20 17:40:47 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
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
	char	*buffer;
	char	*line;
	char	*temp;
	size_t	required_size;
	size_t	buffer_size;

	buffer = NULL;
	line = NULL;
	buffer_size = 256;
	buffer = (char *)malloc(buffer_size);
	if (buffer == NULL)
	{
		perror("Memory allocation error");
		return (NULL);
	}
	buffer[0] = '\0';
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			perror("Error reading input");
			free(buffer);
			return (NULL);
		}
		if (strncmp(line, delimiter, strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		required_size = strlen(buffer) + strlen(line) + strlen(delimiter) + 1;
		if (required_size > buffer_size)
		{
			buffer = ft_realloc(buffer, buffer_size, buffer_size * 2);
			if (buffer == NULL)
			{
				perror("Memory allocation error");
				free(line);
				return (NULL);
			}
		}
		temp = ft_strjoin(buffer, line);
		temp = ft_strjoin(temp, "\n");
		if (temp == NULL)
		{
			free(line);
			return (NULL);
		}
		free(buffer);
		buffer = temp;
		free(line);
	}
	if (temp == NULL)
		return (NULL);
	return (temp);
}
