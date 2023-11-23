/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:28:54 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:08:42 by otuyishi         ###   ########.fr       */
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
