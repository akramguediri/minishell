/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:46:16 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/19 17:28:40 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc(const char *delimiter)
{
	char	*buffer;
	char	*line;
	char	*temp;

	buffer = calloc(1, 1);
	line = NULL;
	printf("Enter Heredoc content. Terminate with '%s':\n", delimiter);
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
		temp = realloc(buffer, strlen(buffer) + strlen(line) + strlen(delimiter)
				+ 1);
		if (temp == NULL)
		{
			perror("Memory allocation error");
			free(buffer);
			free(line);
			return (NULL);
		}
		buffer = temp;
		strcat(buffer, line);
		free(line);
	}
	strcat(buffer, delimiter);
	return (buffer);
}
