/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:27:39 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 16:49:36 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_command(t_data *data)
{
	char	*input;

	input = NULL;
	input = readline(ft_strjoin(ft_getactivepath(data, 0), " %"));
	return (input);
}

void	process_command3(t_cmd_hist *command, t_data *data, t_cmd_hist *h,
		int z)
{
	if (ft_strlen(command->history) == 1)
	{
		ft_putendl_fd("Command not found", 2);
		return ;
	}
	if (ft_strnstr(command->history, "exit", 5) != 0)
		exit(0);
	else if (strnstr(command->history, "export ", 7) != 0)
		my_export(command->history + 7);
	else if (strncmp(command->history, "unset ", 6) == 0)
		my_unset(command->history + 6);
	else
		exec(command->history, data, h, z);
}

int	handle_command(char *cmd, t_data *data, t_cmd_hist *h, int z)
{
	char	*trimmed_cmd;
	int		result;

	trimmed_cmd = ft_strtrim(cmd, " \t");
	result = 0;
	if (!trimmed_cmd)
		result = 1;
	else if (ft_strncmp(trimmed_cmd, "clear", 5) == 0)
		result = handle_clear();
	else if (ft_strncmp(trimmed_cmd, "env", 3) == 0 || ft_strncmp(trimmed_cmd,
			"export", 6) == 0)
		result = handle_env();
	else if (ft_strncmp(trimmed_cmd, "history", 7) == 0)
		result = handle_history(h);
	else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
		result = handle_pwd(data);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		result = handle_echo(cmd, z);
	free(trimmed_cmd);
	return (result);
}

void	run_command(char *cmd, t_data *data, t_cmd_hist *h, int z)
{
	if (ft_strnstr(cmd, "cd", 2) != 0)
	{
		cd(cmd);
	}
	else
		commandd(cmd, data, h, z);
}

int	commandd(char *input2, t_data *data, t_cmd_hist *h, int z)
{
	struct s_cmd_data	cmddata;

	cmddata.z = z;
	process_s_cmd_data(&cmddata, ft_strtrim(input2, "|"));
	cmddata.t = ft_splitonsteroids(cmddata.commandlist, '|');
	cmddata.num_cmds = count_characters(cmddata.commandlist, '|')
		- countpipes(cmddata.t, '|') + 1;
	pipes_io_redir(cmddata, data, h);
	free(cmddata.commandlist);
	free(cmddata.input_file);
	free(cmddata.output_file);
	return (0);
}
