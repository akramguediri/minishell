/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:23:05 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:08:51 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_echo(char *cmd, int z)
{
	echo(cmd, z);
	return (1);
}

void	printecho(char *s)
{
	int	i;

	i = ft_strlen(s);
	while (s[i] != ' ')
		i--;
	while (s[i])
	{
		printf("%c", s[i]);
		i++;
	}
}

void	print_echo_argument(char *argument, int z)
{
	char	*trimmed_argument;

	if (ft_strlen(argument) == 1)
	{
		printf("%s", argument);
		return ;
	}
	trimmed_argument = ft_strdup(ft_strtrim(argument, "\"'"));
	if (!trimmed_argument)
		return ;
	else if (ft_strnstr(trimmed_argument, "$?", 2))
		printf("%d", z);
	else if (ft_strchr(trimmed_argument, '$') && (argument[0] != '\''
			|| argument[1] == '\''))
		printvar(ft_strtrim(trimmed_argument, "$"), argument);
	else
	{
		if (argument[1] == '\"' && argument[0] == '\'')
			printf("\"%s\" ", trimmed_argument);
		else
			printf("%s ", trimmed_argument);
	}
	free(trimmed_argument);
}

void	handle_echo_options(char **tokens, int *start_index)
{
	if (tokens[*start_index] && ft_strnstr(tokens[*start_index], "-n", 3))
		(*start_index)++;
}

void	echo(char *s, int z)
{
	char	**tokens;
	int		line;
	int		start;
	int		i;

	tokens = ft_splitonsteroids(s, ' ');
	if (!tokens)
	{
		printf("Splitting failed.\n");
		return ;
	}
	line = 1;
	start = 1;
	handle_echo_options(tokens, &start);
	i = start;
	while (tokens[i] != NULL)
	{
		print_echo_argument(tokens[i], z);
		i++;
	}
	if (line)
		printf("\n");
}
