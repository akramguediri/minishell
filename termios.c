/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:47:52 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/22 18:16:57 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_interrupt(int signal)
{
	if (signal == SIGQUIT)
	{
		printf("\nCtrl+D detected. Exiting...\n");
		exit(EXIT_SUCCESS);
	}
}

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

char	*read_command(t_data *data)
{
	char	*input;

	input = NULL;
	input = readline(ft_strjoin(ft_getactivepath(data, 0), " %"));
	return (input);
}

void	eof_handler(int errnum)
{
	printf("Ctrl+D (EOF) pressed. Exiting...\n");
	exit(errnum);
}

void	custom_clear(void)
{
	const char	*clear_sequence;

	if (isatty(STDOUT_FILENO))
	{
		clear_sequence = "\x1b[H\x1b[2J";
		write(STDOUT_FILENO, clear_sequence, ft_strlen(clear_sequence));
	}
}

char	*get_current_directory(void)
{
	size_t	path_max;
	char	*current_directory;

	path_max = pathconf(".", _PC_PATH_MAX);
	current_directory = (char *)malloc(path_max);
	if (!current_directory)
	{
		return (NULL);
	}
	return (getcwd(current_directory, path_max));
}

char	*get_parent_path(const char *path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		i++;
	}
	while (i > 0 && path[i - 1] != '/')
	{
		i--;
	}
	return (ft_substr(path, 0, i));
}

void	change_directory(const char *new_path)
{
	chdir(new_path);
}

void	handle_dotdot(const char *input, char *current_directory)
{
	char	*trimmed_path;
	char	*new_path;

	if (strnstr(input, "..", 2) != 0)
	{
		current_directory = get_parent_path(current_directory);
	}
	trimmed_path = ft_strtrim(input, "..");
	if (trimmed_path)
	{
		new_path = ft_strjoin(current_directory, "/");
		new_path = ft_strjoin(new_path, trimmed_path);
		change_directory(new_path);
		free(new_path);
		free(trimmed_path);
	}
}

void	handle_cd_command(char *current_directory)
{
	char	**path_parts;
	char	*new_path;

	path_parts = ft_split(current_directory, '/');
	if (path_parts[0] && path_parts[1])
	{
		new_path = ft_strjoin("/", path_parts[0]);
		new_path = ft_strjoin(new_path, "/");
		new_path = ft_strjoin(new_path, path_parts[1]);
		new_path = ft_strjoin(new_path, "/");
		change_directory(new_path);
		free(new_path);
	}
}

void	cd(char *input)
{
	char	**tokens;
	char	*current_directory;

	tokens = ft_split(input, ' ');
	if (!tokens)
	{
		return ;
	}
	current_directory = get_current_directory();
	if (tokens[1])
	{
		handle_dotdot(tokens[1], current_directory);
	}
	else if (!ft_strncmp(input, "cd", 2))
	{
		handle_cd_command(current_directory);
	}
	free(current_directory);
}

void	reset_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, reset_handler);
}

void	printecho(char *s)
{
	int	i;

	i = ft_strlen(s);
	while (s[i] != ' ')
		i--;
	while (s[i])
	{
		printf("%c", s[i]);
		i++;
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

// void	echo(char *s)
// {
// 	char	**t;
// 	char	*e;
// 	int		r;
// 	int		i;

// 	r = 0;
// 	i = 1;
// 	t = ft_splitonsteroids(s, ' ');
// 	if (t[i] && ft_strnstr(t[1], "-n", 3))
// 	{
// 		r = 1;
// 		i = 2;
// 	}
// 	if (t)
// 	{
// 		while (t[i] != NULL)
// 		{
// 			e = ft_strdup(ft_strtrim(t[i], "\"'"));
// 			if (!e)
// 				return ;
// 			if (ft_strchr(e, '$'))
// 				printvar(ft_strtrim(e, "$"));
// 			else
// 				printf("%s ", e);
// 			i++;
// 		}
// 	}
// 	else
// 		printf("Splitting failed.\n");
// 	if (r == 0)
// 		printf("\n");
// }

void	print_echo_argument(char *argument, int z)
{
	char	*trimmed_argument;

	if (ft_strlen(argument) == 1)
	{
		printf("%s", argument);
		return ;
	}
	trimmed_argument = ft_strdup(ft_strtrim(argument, "\"'"));
	if (!trimmed_argument)
		return ;
	else if (ft_strnstr(trimmed_argument, "$?", 2))
		printf("%d", z);
	else if (ft_strchr(trimmed_argument, '$') && (argument[0] != '\''
			|| argument[1] == '\''))
		printvar(ft_strtrim(trimmed_argument, "$"), argument);
	else
	{
		if (argument[1] == '\"' && argument[0] == '\'')
			printf("\"%s\" ", trimmed_argument);
		else
			printf("%s ", trimmed_argument);
	}
	free(trimmed_argument);
}

void	handle_echo_options(char **tokens, int *start_index)
{
	if (tokens[*start_index] && ft_strnstr(tokens[*start_index], "-n", 3))
		(*start_index)++;
}

void	echo(char *s, int z)
{
	char	**tokens;
	int		line;
	int		start;
	int		i;

	tokens = ft_splitonsteroids(s, ' ');
	if (!tokens)
	{
		printf("Splitting failed.\n");
		return ;
	}
	line = 1;
	start = 1;
	handle_echo_options(tokens, &start);
	i = start;
	while (tokens[i] != NULL)
	{
		print_echo_argument(tokens[i], z);
		i++;
	}
	if (line)
		printf("\n");
}

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

int	handle_clear(void)
{
	custom_clear();
	return (1);
}

int	handle_env(void)
{
	printenv();
	return (1);
}

int	handle_history(t_cmd_hist *h)
{
	printhst_list(h);
	return (1);
}

int	handle_pwd(t_data *data)
{
	ft_getactivepath(data, 1);
	printf("\n");
	return (1);
}

int	handle_echo(char *cmd, int z)
{
	echo(cmd, z);
	return (1);
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

void	free_history_nodes(t_cmd_hist *h)
{
	t_cmd_hist	*temp;

	while (h != NULL)
	{
		temp = h;
		h = h->next;
		free(temp->history);
		free(temp);
	}
}

void	process_history(t_cmd_hist *command, int *i, t_cmd_hist **h)
{
	if (ft_strlen(command->history))
	{
		add_history(command->history);
		command->history_index = ++(*i);
		command->history_size = ft_strlen(command->history);
		command->next = *h;
		*h = command;
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

//----------------------------------------------------------------------------
// void run_termios(t_data *data, t_cmd_hist **h, int *i, int *z)
// {
// 	t_cmd_hist		*command;

// 	command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
// 	if (!command)
// 		return ;
// 	command->history = read_command(data);
// 	if (!command->history)
// 	{
// 		free(command);
// 		return ;
// 	}
// 	if (ft_strlen(command->history))
// 	{
// 		add_history(command->history);
// 		command->history_index = ++(*i);
// 		command->history_size = ft_strlen(command->history);
// 		command->next = *h;
// 		*h = command;
// 	}
// 	process_command3(command, data, *h, *z);
// 	command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
// 	if (!command)
// 		return ;
// }

// void termios(t_data *data)
// {
// 	struct termios	saved_attributes;
// 	t_cmd_hist		*h;
// 	int				i;
// 	int				z;

// 	h = NULL;
// 	i = 0;
// 	z = 0;
// 	setup_signals();
// 	init_termios(&saved_attributes);
// 	while (1)
// 	{
// 		run_termios(data, &h, &i, &z);
// 	}
// 	restore_termios(&saved_attributes);
// 	free_history_nodes(h);
// }
//----------------------------------------------------
