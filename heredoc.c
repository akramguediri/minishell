/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:46:16 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/22 18:16:21 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// char	*heredoc(const char *delimiter)
// {
// 	char	*buffer;
// 	char	*line;
// 	char	*temp;
// 	size_t	required_size;
// 	size_t	buffer_size;

// 	buffer = NULL;
// 	line = NULL;
// 	buffer_size = 256;
// 	buffer = (char *)ft_calloc(buffer_size, 1);
// 	buffer[0] = '\0';
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 		{
// 			perror("Error reading input");
// 			free(buffer);
// 			return (NULL);
// 		}
// 		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		required_size = ft_strlen(buffer) + ft_strlen(line)
// 			+ ft_strlen(delimiter) + 1;
// 		if (required_size > buffer_size)
// 		{
// 			buffer = ft_realloc(buffer, buffer_size, buffer_size * 2);
// 			if (buffer == NULL)
// 			{
// 				perror("Memory allocation error");
// 				free(line);
// 				return (NULL);
// 			}
// 		}
// 		temp = ft_strjoin(buffer, line);
// 		temp = ft_strjoin(temp, "\n");
// 		if (temp == NULL)
// 		{
// 			free(line);
// 			return (NULL);
// 		}
// 		free(buffer);
// 		buffer = temp;
// 		free(line);
// 	}
// 	if (temp == NULL)
// 		return (NULL);
// 	return (temp);
// }
