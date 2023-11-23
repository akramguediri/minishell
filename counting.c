/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:28:54 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 16:02:08 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>
#include <stdlib.h>

int	ft_wordcount(char const *s, char c)
{
	int		count;
	int		i;
	bool	inside_single_quotes;
	bool	inside_double_quotes;

	count = 0;
	i = 0;
	inside_single_quotes = false;
	inside_double_quotes = false;
	while (s[i])
	{
		if (s[i] == '\'')
			inside_single_quotes = !inside_single_quotes;
		else if (s[i] == '"')
			inside_double_quotes = !inside_double_quotes;
		if (((s[i] != c && s[i] != ' ' && s[i] != '\t') || inside_single_quotes
				|| inside_double_quotes) && (i == 0 || (s[i - 1] == c || s[i
						- 1] == ' ' || s[i - 1] == '\t')))
		{
			count++;
		}
		i++;
	}
	return (count);
}

int	count_characters(const char *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == c)
		{
			count++;
		}
		i++;
	}
	return (count);
}

int	countpipes(char **t, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (t[i])
	{
		j += count_characters(t[i], c);
		i++;
	}
	return (j);
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
