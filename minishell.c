/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:52:06 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/23 12:24:53 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printenvlist(t_env *envlist)
{
	t_env	*current;

	current = envlist;
	while (current != NULL)
	{
		printf("%s\n", current->l);
		current = current->next;
	}
}

void	printhst_list(t_cmd_hist *envlist)
{
	if (envlist == NULL)
		return ;
	printhst_list(envlist->next);
	if (envlist->history != NULL)
		printf("%d %s\n", envlist->history_index, envlist->history);
}

void	ft_init(char **env, t_data **data)
{
	t_env	*temp;
	int		i;
	t_env	*new_env;

	i = 0;
	*data = (t_data *)malloc(sizeof(t_data));
	(*data)->env = NULL;
	while (env[i])
	{
		new_env = (t_env *)malloc(sizeof(t_env));
		new_env->l = ft_strdup(env[i]);
		new_env->next = NULL;
		if ((*data)->env == NULL)
			(*data)->env = new_env;
		else
		{
			temp = (*data)->env;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = new_env;
		}
		i++;
	}
}

const char	*ft_getactivepath(t_data *data, int j)
{
	size_t	i;
	char	*s;

	i = pathconf(".", _PC_PATH_MAX);
	s = (char *)malloc((size_t)i);
	data = (t_data *)malloc(sizeof(t_data));
	if (!s)
		return (NULL);
	s = getcwd(s, i);
	if (s)
		data->path = ft_strdup(s);
	if (j == 1)
		printf("%s\n", s);
	return (s);
}

void	ft_get(t_data *data)
{
	size_t	i;
	char	*s;
	char	**t;

	i = pathconf(".", _PC_PATH_MAX);
	s = (char *)malloc((size_t)i);
	data = (t_data *)malloc(sizeof(t_data));
	if (!s)
		return ;
	s = getcwd(s, i);
	t = ft_split(s, '/');
	i = 0;
	while (t[i])
		i++;
	data->path = ft_strdup(t[i--]);
	printf("%s %% >>", t[i]);
}
