/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:11:42 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:09:03 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	addtoenv(char *s)
{
	int	i;

	i = 0;
	while (environ[i] != NULL)
	{
		i++;
	}
	environ[i] = malloc(strlen(s) + 1);
	if (environ[i] == NULL)
	{
		perror("export");
	}
	else
	{
		strcpy(environ[i], s);
	}
}

int	handle_env(void)
{
	printenv();
	return (1);
}

void	my_export(char *arg)
{
	char	*key;
	char	*value;

	if (!arg)
		printenv();
	else
	{
		key = strtok(arg, "=");
		value = strtok(NULL, "=");
		if (key != NULL && value != NULL)
		{
			if (setenv(key, ft_strtrim(value, "\'\" "), 1) == 0)
				printf("Exported: %s=%s\n", key, value);
			else
				perror("Export failed");
		}
	}
}

void	my_unset(char *arg)
{
	if (unsetenv(arg) == 0)
		printf("Unset: %s\n", arg);
	else
		perror("Unset failed");
}

void	printenv(void)
{
	int	i;

	i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
