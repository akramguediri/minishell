/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:55:27 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/23 12:10:10 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command2(char *cmd, t_data *data, t_cmd_hist *h, int z)
{
	char	*token;
	char	*args[64];
	int		arg_count;
	char	*cmd2;
	int		r;

	r = z;
	arg_count = 0;
	if (cmd)
		cmd2 = ft_strdup(cmd);
	token = strtok(cmd, " ");
	while (token != NULL)
	{
		args[arg_count++] = token;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;
	if (handle_command(cmd2, data, h, z) == 0)
	{
		z = ft_execvp(args[0], args);
		return (r);
	}
	return (EXIT_SUCCESS);
}

char	*process_command(char *command, char *input_file, char *output_file)
{
	int		len;
	char	*processed;
	int		i[2];

	len = ft_strlen(command);
	processed = (char *)ft_calloc(len + 1, 1);
	i[0] = 0;
	i[1] = 0;
	while (i[0] < len)
	{
		if (command[i[0]] == '<')
		{
			i[0]++;
			input_file = extract_input_file(command, &i[0], len);
		}
		else if (command[i[0]] == '>')
		{
			i[0]++;
			output_file = extract_output_file(command, &i[0], len);
		}
		else
			processed[i[1]++] = command[i[0]++];
	}
	return (processed[i[1]] = '\0', free(input_file), free(output_file),
		processed);
}

void	initialize_s_cmd_data(struct s_cmd_data *cmddata, char *input2)
{
	cmddata->r = 0;
	cmddata->here = 0;
	if (input2)
		cmddata->input = ft_split(input2, ' ');
	else
		cmddata->input = NULL;
	cmddata->input_file = NULL;
	cmddata->output_file = NULL;
	cmddata->commandlist = NULL;
	cmddata->heredoc = NULL;
	cmddata->commandlist = ft_calloc(1, 1);
	cmddata->commandlist[0] = '\0';
}

void	process_s_cmd_data(struct s_cmd_data *cmddata, char *input2)
{
	int	i;

	i = 0;
	initialize_s_cmd_data(cmddata, input2);
	while (cmddata->input[i] && cmddata->input)
	{
		if (ft_strnstr(cmddata->input[i], "<<", 2) != 0)
			cmddata->here = 2;
		else if (ft_strncmp(cmddata->input[i], "<", 1) == 0)
			cmddata->input_file = process_input_file(cmddata->input, &i);
		else if (ft_strncmp(cmddata->input[i], ">", 1) == 0
			|| ft_strncmp(cmddata->input[i], ">>", 2) == 0)
			cmddata->output_file = process_output_file(cmddata->input, &i,
					&(cmddata->r));
		else
			cmddata->processed = process_regular_command(cmddata,
					cmddata->input[i]);
		if (cmddata->here == 2 && (!ft_strlen(ft_strtrim(cmddata->input[i],
						"<"))))
			process_heredoc(cmddata, cmddata->input[i + 1]);
		else if (cmddata->here == 2)
			process_heredoc(cmddata, ft_strtrim(cmddata->input[i], "<"));
		i++;
	}
}

char	*process_regular_command(struct s_cmd_data *cmddata, char *command)
{
	char	*input_file;
	char	*output_file;

	input_file = NULL;
	output_file = NULL;
	cmddata->processed = process_command(command, input_file, output_file);
	if (cmddata->processed)
	{
		cmddata->commandlist = ft_strjoin(cmddata->commandlist,
				cmddata->processed);
		cmddata->commandlist = ft_strjoin(cmddata->commandlist, " ");
		free(cmddata->processed);
	}
	return ((cmddata->commandlist));
}
