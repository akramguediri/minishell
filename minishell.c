/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:52:06 by aguediri          #+#    #+#             */
/*   Updated: 2023/10/10 18:55:53 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_up(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
void	printenvList(t_env *envlist)
{
	t_env	*current;

	current = envlist;
	while (current != NULL)
	{
		printf("%s\n", current->l);
		current = current->next;
	}
}
// void	printhstList(t_cmd_hist *envlist)
// {
// 	while (envlist != NULL)
// 	{
// 		printf("%s\n", envlist->history);
// 		envlist = envlist->next;
// 	}
// }

void	printhstList(t_cmd_hist *envlist)
{
   if (envlist == NULL)
    {
        return; // Base case: reached the end of the list
    }
    
    // Recursively call the function on the next node first
    printhstList(envlist->next);

    // Print the current node's data
    if (envlist->history != NULL)
    {
        printf("%d %s\n",envlist->history_index, envlist->history);
    }
}

void	ft_lstaddback(t_env **lst, t_env *new)
{
	t_env	*t;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	t = *lst;
	while (t->next != NULL)
		t = t->next;
	t->next = new;
}

void	ft_init(char **env, t_data **data)
{
	t_env	*temp;
	int		i;
	t_env	*new_env;

	i = 0;
	*data = (t_data *)malloc(sizeof(t_data)); // Allocate memory for t_data
	(*data)->env = NULL;                      // Initialize the env field
	while (env[i])
	{
		new_env = (t_env *)malloc(sizeof(t_env));
		new_env->l = ft_strdup(env[i]);
		new_env->next = NULL; // Initialize the 'next' pointer
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

void	ft_getactivepath(t_data *data)
{
	size_t	i;
	char	*s;

	i = pathconf(".", _PC_PATH_MAX);
	s = (char *)malloc((size_t)i);
	data = (t_data *)malloc(sizeof(t_data));
	if (!s)
		return ;
	s = getcwd(s, i);
	if (s)
		data->path = ft_strdup(s);
	printf("%s %%", s);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	if (argc < 1)
		printf("fbkk");
	if (argv[0])
		;
	data = NULL;
	ft_init(env, &data);
	termios(data);
}
