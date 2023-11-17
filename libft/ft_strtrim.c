/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 22:54:26 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/17 07:02:37 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	isset(char const *set, char c)
{
	int	i;

	i = 0;
	while (i < ft_strlen(set))
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*r;

	i = 0;
	k = 0;
	if (s1 == NULL || set == NULL)
		return (NULL);
	j = ft_strlen(s1) - 1;
	if (j <= 0)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(""));
	while (isset(set, s1[i]) && s1[i] != '\0')
		i++;
	while (isset(set, s1[j]) && j > i)
		j--;
	r = (char *)malloc((j - i + 2) * sizeof(char));
	if (r == NULL)
		return (NULL);
	while (i <= j)
		r[k++] = s1[i++];
	r[k] = '\0';
	return (r);
}
