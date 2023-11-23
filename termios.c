/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:47:52 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:19:13 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_termios(struct termios *saved_attributes)
{
	struct termios	new_attributes;

	if (tcgetattr(STDIN_FILENO, saved_attributes) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	new_attributes = *saved_attributes;
	new_attributes.c_lflag &= ~(ICANON);
	new_attributes.c_cc[VMIN] = 1;
	new_attributes.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attributes) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	restore_termios(struct termios *saved_attributes)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, saved_attributes) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	printvar(char *s, char *argument)
{
	int		i;
	char	**t;

	i = 0;
	while (environ[i])
	{
		if (ft_strnstr(environ[i], s, ft_strlen(s)))
		{
			t = ft_split(environ[i], '=');
			if (argument[1] == '\'' && argument[0] == '\"')
				printf("\'%s\' ", t[1]);
			else
				printf("%s", t[1]);
			break ;
		}
		i++;
	}
}

void	exec(char *s, t_data *data, t_cmd_hist *h, int z)
{
	char	**t;
	int		i;

	t = ft_split(s, '&');
	i = 0;
	while (t[i])
	{
		run_command(t[i], data, h, z);
		i++;
	}
}

void	termios(t_data *data)
{
	struct termios	saved_attributes;
	t_cmd_hist		*command;
	int				i;
	t_cmd_hist		*h;
	int				z;

	h = NULL;
	i = 0;
	z = 0;
	command = (t_cmd_hist *)ft_calloc(sizeof(t_cmd_hist), 1);
	setup_signals();
	init_termios(&saved_attributes);
	while (1)
	{
		command->history = read_command(data);
		if (!command->history)
			break ;
		process_history(command, &i, &h);
		process_command3(command, data, h, z);
		command = (t_cmd_hist *)ft_calloc(sizeof(t_cmd_hist), 1);
	}
	restore_termios(&saved_attributes);
	free_history_nodes(h);
	free_history_nodes(command);
}
