/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:37:57 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:11:11 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_exit(char *s)
{
	ft_putstr_fd(s, 2);
	return (EXIT_FAILURE);
}

void	wait_for_children(int num_cmds, pid_t children[], int z)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(children[i], &z, 0);
		i++;
	}
}

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

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	if (argc < 1)
		error_exit("error");
	if (argv[0])
		;
	data = NULL;
	environ = env;
	ft_init(env, &data);
	termios(data);
}
