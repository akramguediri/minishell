/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitonsteroids.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:58:41 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/04 18:43:15 by aguediri         ###   ########.fr       */
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
	char	**t;
	char	*s2;
	int		j;
	int		l;
	int		k;
	int		len;

	j = 0;
	l = 0;
	s2 = NULL;
	if (count_characters(s, '\'') % 2 != 0)
	{
		s2 = heredoc("\'");
	}
	else if (count_characters(s, '\"') % 2 != 0)
	{
		s2 = heredoc("\"");
	}
	if (s2)
		s = ft_strjoin(s, s2);
	printf("%s", s);
	t = (char **)malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!t)
		return (NULL);
	while (s[l])
	{
		l = ft_split_start(s, c, l);
		if (ft_split_len(s, l, c) != 0)
		{
			t[j] = (char *)malloc((ft_split_len(s, l, c) + 1) * sizeof(char));
			if (!t[j])
				return (free_split(t), NULL);
			k = 0;
			len = ft_split_len(s, l, c);
			while (k < len)
			{
				t[j][k] = s[l + k];
				k++;
			}
			t[j][len] = '\0';
			j++;
		}
		l += ft_split_len(s, l, c);
	}
	t[j] = NULL;
	return (t);
}
// #include <stdio.h>

// int	main(void)
// {
// 	char input[] = "\"This is a test 'string' with quotes and, spaces \"'between them.";
// 	char c = ' ';

// 	char **result = ft_splitonsteroids(input, c);

// 	if (result)
// 	{
// 		for (int i = 0; result[i] != NULL; i++)
// 		{
// 			printf("Element %d: \"%s\"\n", i, result[i]);
// 			free(result[i]);
// 		}
// 		free(result);
// 	}
// 	else
// 	{
// 		printf("Splitting failed.\n");
// 	}

// 	return (0);
// }