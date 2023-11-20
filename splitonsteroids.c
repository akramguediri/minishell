/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitonsteroids.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:58:41 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/20 21:36:36 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>
#include <stdlib.h>

static int	ft_wordcount(char const *s, char c)
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

static int	ft_split_len(const char *s, int l, char c)
{
	int		i;
	bool	inside_single_quotes;
	bool	inside_double_quotes;

	i = 0;
	inside_single_quotes = false;
	inside_double_quotes = false;
	while ((s[l + i] != c && s[l + i]) || inside_single_quotes
		|| inside_double_quotes)
	{
		if (s[l + i] == '\'')
			inside_single_quotes = !inside_single_quotes;
		else if (s[l + i] == '"')
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	return (i);
}

static int	ft_split_start(const char *s, char c, int l)
{
	bool	inside_single_quotes;
	bool	inside_double_quotes;

	inside_single_quotes = false;
	inside_double_quotes = false;
	while ((s[l] == c || s[l] == ' ' || s[l] == '\t') && !(inside_single_quotes
			|| inside_double_quotes))
	{
		if (s[l] == '\'')
			inside_single_quotes = !inside_single_quotes;
		else if (s[l] == '"')
			inside_double_quotes = !inside_double_quotes;
		l++;
	}
	return (l);
}

char	**ft_splitonsteroids(char *s, char c)
{
	t_steroids	st;

	st.j = 0;
	st.l = 0;
	st.s2 = NULL;
	if (count_characters(s, '\'') % 2 != 0)
	{
		st.s2 = heredoc("\'");
		st.s2 = ft_strjoin(st.s2, "\'");
	}
	else if (count_characters(s, '\"') % 2 != 0)
	{
		st.s2 = heredoc("\"");
		st.s2 = ft_strjoin(st.s2, "\"");
	}
	if (st.s2)
		s = ft_strjoin(s, st.s2);
	st.result = (char **)malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!st.result)
		return (NULL);
	while (s[st.l])
	{
		st.l = ft_split_start(s, c, st.l);
		if (ft_split_len(s, st.l, c) != 0)
		{
			st.result[st.j] = (char *)malloc((ft_split_len(s, st.l, c) + 1)
					* sizeof(char));
			if (!st.result[st.j])
				return (free_split(st.result), NULL);
			st.k = 0;
			st.len = ft_split_len(s, st.l, c);
			while (st.k < st.len)
			{
				st.result[st.j][st.k] = s[st.l + st.k];
				st.k++;
			}
			st.result[st.j][st.len] = '\0';
			//st.result[st.j] = ft_strtrim(st.result[st.j], "'\"");
			st.j++;
		}
		st.l += ft_split_len(s, st.l, c);
	}
	st.result[st.j] = NULL;
	return (st.result);
}
