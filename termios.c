/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:47:52 by otuyishi          #+#    #+#             */
/*   Updated: 2023/11/17 14:34:57 by aguediri         ###   ########.fr       */
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
	ft_getactivepath(data);
	input = readline("");
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

char	*find_command_in_path(const char *command_name)
{
	char	*path;
	char	*path_copy;
	char	*token;
	char	*full_path;
	size_t	full_path_length;

	path = getenv("PATH");
	if (path == NULL)
		return (NULL);
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		full_path_length = strlen(token) + 1 + strlen(command_name) + 2;
		full_path = (char *)malloc(full_path_length);
		if (full_path == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		sprintf(full_path, "%s/%s", token, command_name);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

char	*getprpath(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (s[i] != '/')
		i--;
	return (ft_substr(s, 0, i));
}
void	cd(char *str)
{
	size_t	i;
	char	*s;
	char	**t;
	char	**tmp;

	t = ft_split(str, ' ');
	i = pathconf(".", _PC_PATH_MAX);
	s = (char *)malloc((size_t)i);
	if (!s)
		return ;
	s = getcwd(s, i);
	if (t[1])
	{
		if (strnstr(t[1], "..", 2) != 0)
			s = getprpath(s);
		if (ft_strtrim(t[1], ".."))
		{
			s = ft_strjoin(s, "/");
			chdir(ft_strjoin(s, ft_strtrim(t[1], "..")));
		}
	}
	else if (!ft_strncmp(str, "cd", 2))
	{
		tmp = ft_split(s, '/');
		if (tmp[0] && tmp[1])
		{
			s = ft_strjoin("/", tmp[0]);
			s = ft_strjoin(s, "/");
			s = ft_strjoin(s, tmp[1]);
			s = ft_strjoin(s, "/");
			chdir(s);
		}
	}
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
void	printvar(char *s)
{
	int		i;
	char	**t;

	i = 0;
	while (environ[i])
	{
		if (ft_strnstr(environ[i], s, ft_strlen(s)))
		{
			t = ft_split(environ[i], '=');
			printf("%s", t[1]);
			break ;
		}
		i++;
	}
}
void	echo(char *s)
{
	char	**t;
	char	*e;
	int		r;
	int		i;

	r = 0;
	i = 1;
	t = ft_splitonsteroids(s, ' ');
	if (t[i] && ft_strnstr(t[1], "-n", 3))
	{
		r = 1;
		i = 2;
	}
	if (t)
	{
		while (t[i] != NULL)
		{
			e = ft_strdup(ft_strtrim(t[i], "\"'"));
			if (!e)
				return;
			if (ft_strchr(e, '$'))
				printvar(ft_strtrim(e, "$"));
			else
				printf("%s ", e);
			i++;
		}
	}
	else
	{
		printf("Splitting failed.\n");
	}
	if (r == 1)
		printf("\n");
}

void addtoenv(char *s)
{
    int i = 0;

    // Find the end of the environ array
    while (environ[i] != NULL)
    {
        i++;
    }

    // Allocate space for the new environment variable
    environ[i] = ft_strjoin(s, "");

    // Check if memory allocation was successful
    if (environ[i] == NULL)
    {
        perror("export");
    }
}

void	export(char *s)
{
	char	*var;

	var = malloc(strlen(s) - 6 + 1);
	if (var != NULL)
	{
		strcpy(var, s + 6);
		addtoenv(var);
		free(var);
	}
}
// int	handle_command(char *cmd, t_data *data, t_cmd_hist *h)
// {
// 	char	*trimmed_cmd;
// 	int		i;

// 	i = 0;
// 	trimmed_cmd = ft_strtrim(cmd, " ");
// 	trimmed_cmd = ft_strtrim(cmd, "\t");
// 	if (!trimmed_cmd)
// 		return (1);
// 	else if (strncmp(trimmed_cmd, "clear") == 0)
// 	{
// 		custom_clear();
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "env", 3) == 0)
// 	{
// 		printenvList(data->env);
// 		printf("\n");
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "history", 7) == 0)
// 	{
// 		printhstList(h);
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
// 	{
// 		ft_getactivepath(data);
// 		i = 1;
// 	}
// 	else if (ft_strnstr(trimmed_cmd, "export", 6) != 0)
// 	{
// 		if (ft_strncmp(trimmed_cmd, "export", 6) != 0)
// 			printenvList(data->env);
// 		else
// 			export(data->env, trimmed_cmd);
// 		i = 1;
// 	}
// 	else if (ft_strnstr(cmd, "echo", 4) != 0)
// 	{
// 		echo(cmd);
// 		i = 1;
// 	}
// 	return (i);
// }
int	handle_command(char *cmd, t_data *data, t_cmd_hist *h)
{
	char	*trimmed_cmd;
	int		i;

	i = 0;
	trimmed_cmd = ft_strtrim(cmd, " \t");
	if (!trimmed_cmd)
		return (1);
	else if (ft_strncmp(trimmed_cmd, "clear", 5) == 0)
	{
		custom_clear();
		i = 1;
	}
	else if (ft_strncmp(trimmed_cmd, "env", 3) == 0)
	{
		printenv();
		i = 1;
	}
	else if (ft_strncmp(trimmed_cmd, "history", 7) == 0)
	{
		printhstList(h);
		i = 1;
	}
	else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
	{
		ft_getactivepath(data);
		i = 1;
	}
	else if (ft_strncmp(cmd, "echo", 4) == 0)
	{
		echo(cmd);
		i = 1;
	}
	free(trimmed_cmd);
	return (i);
}

void	run_command(char *cmd, t_data *data, t_cmd_hist *h)
{
	if (ft_strnstr(cmd, "cd", 2) != 0)
	{
		cd(cmd);
	}
	else if (ft_strnstr(cmd, "export", 6) != 0)
		export(cmd);
	else
		commandd(cmd, data, h);
}

void	exec(char *s, t_data *data, t_cmd_hist *h)
{
	char	**t;
	int		i;

	t = ft_split(s, '&');
	i = 0;
	while (t[i])
	{
		run_command(t[i], data, h);
		i++;
	}
}

void	termios(t_data *data)
{
	struct termios	saved_attributes;
	t_cmd_hist		*command;
	int				i;
	t_cmd_hist		*temp;
	t_cmd_hist		*h;

	h = NULL;
	i = 0;
	command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
	if (!command)
		return ;
	setup_signals();
	init_termios(&saved_attributes);
	while (1)
	{
		command->history = read_command(data);
		if (!command->history)
		{
			free(command);
			break ;
		}
		if (ft_strlen(command->history))
		{
			add_history(command->history);
			command->history_index = ++i;
			command->history_size = ft_strlen(command->history);
			command->next = h;
			h = command;
		}
		if (ft_strnstr(command->history, "exit", 4) != 0)
			break ;
		exec(command->history, data, h);
		command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
		if (!command)
			break ;
	}
	restore_termios(&saved_attributes);
	while (h != NULL)
	{
		temp = h;
		h = h->next;
		free(temp->history);
		free(temp);
	}
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <termios.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// char	*getprpath(char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	while (s[i] != '/')
// 		i--;
// 	return (ft_substr(s, 0, i));
// }

// void	cd(char *str)
// {
// 	size_t	i;
// 	char	*s;
// 	char	**t;
// 	char	**tmp;

// 	t = ft_split(str, ' ');
// 	i = pathconf(".", _PC_PATH_MAX);
// 	s = (char *)malloc((size_t)i);
// 	if (!s)
// 		return ;
// 	s = getcwd(s, i);
// 	if (t[1])
// 	{
// 		if (strnstr(t[1], "..", 2) != 0)
// 			s = getprpath(s);
// 		if (ft_strtrim(t[1], ".."))
// 		{
// 			s = ft_strjoin(s, "/");
// 			chdir(ft_strjoin(s, ft_strtrim(t[1], "..")));
// 		}
// 	}
// 	else if (!ft_strncmp(str, "cd", 2))
// 	{
// 		tmp = ft_split(s, '/');
// 		if (tmp[0] && tmp[1])
// 		{
// 			s = ft_strjoin("/", tmp[0]);
// 			s = ft_strjoin(s, "/");
// 			s = ft_strjoin(s, tmp[1]);
// 			s = ft_strjoin(s, "/");
// 			chdir(s);
// 		}
// 	}
// }
// void	run_command(char *cmd, t_data *data, t_cmd_hist *h)
// {
// 	if (ft_strnstr(cmd, "cd", 2) != 0)
// 	{
// 		cd(cmd);
// 	}
// 	else
// 		commandd(cmd, data, h);
// }
// void	echo(char *s)
// {
// 	char	**t;

// 	t = ft_splitonsteroids(s, ' ');
// 	if (t)
// 	{
// 		for (int i = 1; t[i] != NULL; i++)
// 		{
// 			printf("Element %d: \"%s\"\n", i, t[i]);
// 			free(t[i]);
// 		}
// 		free(t);
// 	}
// 	else
// 	{
// 		printf("Splitting failed.\n");
// 	}
// }
// void	addtoenv(const char *env, t_env **data)
// {
// 	t_env	*new_env;
// 	t_env	*temp;

// 	new_env = (t_env *)malloc(sizeof(t_env));
// 	if (new_env == NULL)
// 	{
// 		return ;
// 	}
// 	new_env->l = strdup(env);
// 	new_env->next = NULL;
// 	if (*data == NULL)
// 	{
// 		*data = new_env;
// 	}
// 	else
// 	{
// 		temp = *data;
// 		while (temp->next != NULL)
// 		{
// 			temp = temp->next;
// 		}
// 		temp->next = new_env;
// 	}
// }
// void reset_handler(int signo)
// {
//     (void)signo;
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

// void handle_interrupt(int signal)
// {
//     if (signal == SIGQUIT)
//     {
//         printf("\nCtrl+D detected. Exiting...\n");
//         exit(EXIT_SUCCESS);
//     }
// }

// void init_termios(struct termios *saved_attributes)
// {
//     struct termios new_attributes;

//     if (tcgetattr(STDIN_FILENO, saved_attributes) == -1)
//     {
//         perror("tcgetattr");
//         exit(EXIT_FAILURE);
//     }

//     new_attributes = *saved_attributes;
//     new_attributes.c_lflag &= ~(ICANON | ECHO);
//     new_attributes.c_cc[VMIN] = 1;
//     new_attributes.c_cc[VTIME] = 0;

//     if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attributes) == -1)
//     {
//         perror("tcsetattr");
//         exit(EXIT_FAILURE);
//     }
// }

// void restore_termios(struct termios *saved_attributes)
// {
//     if (tcsetattr(STDIN_FILENO, TCSANOW, saved_attributes) == -1)
//     {
//         perror("tcsetattr");
//         exit(EXIT_FAILURE);
//     }
// }
// void	export(t_env *env, char *s)
// {
// 	char	*var;

// 	var = malloc(ft_strlen(s) - 6 + 1);
// 	ft_strlcpy(var, s + 6, ft_strlen(s) - 6);
// 	addtoenv(var, &env);
// }

// char *read_command(t_data *data)
// {
//     ft_getactivepath(data);
//     return (readline(""));
// }

// void custom_clear(void)
// {
//     const char *clear_sequence;

//     if (isatty(STDOUT_FILENO))
//     {
//         clear_sequence = "\x1b[H\x1b[2J";
//         write(STDOUT_FILENO, clear_sequence, strlen(clear_sequence));
//     }
// }

// void	exec(char *s, t_data *data, t_cmd_hist *h)
// {
// 	char	**t;
// 	int		i;

// 	t = ft_split(s, '&');
// 	i = 0;
// 	while (t[i])
// 	{
// 		run_command(t[i], data, h);
// 		i++;
// 	}
// }

// void	setup_signals(void)
// {
// 	signal(SIGQUIT, SIG_IGN);
// 	signal(SIGTERM, SIG_IGN);
// 	signal(SIGINT, reset_handler);
// }
// int	handle_command(char *cmd, t_data *data, t_cmd_hist *h)
// {
// 	char	*trimmed_cmd;
// 	int		i;

// 	i = 0;
// 	trimmed_cmd = ft_strtrim(cmd," ");
// 	if (!ft_split(*ft_split(cmd, '\t'), ' ') || !ft_split(cmd, ' ')
// 		|| !ft_split(cmd, '\t'))
// 		i = 1;
// 	else if (strncmp(trimmed_cmd, "clear") == 0)
// 	{
// 		custom_clear();
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "env", 3) == 0)
// 	{
// 		printenvList(data->env);
// 		printf("\n");
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "history", 7) == 0)
// 	{
// 		printhstList(h);
// 		i = 1;
// 	}
// 	else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
// 	{
// 		ft_getactivepath(data);
// 		i = 1;
// 	}
// 	else if (ft_strnstr(trimmed_cmd, "export", 6) != 0)
// 	{
// 		if (ft_strncmp(trimmed_cmd, "export", 6) != 0)
// 			printenvList(data->env);
// 		else
// 			export(data->env, trimmed_cmd);
// 		i = 1;
// 	}
// 	else if (ft_strnstr(trimmed_cmd, "echo ", 5) != 0)
// 	{
// 		echo(trimmed_cmd);
// 		i = 1;
// 	}
// 	return (i);
// }
// // ... (other functions remain unchanged)

// void termios(t_data *data)
// {
//     struct termios saved_attributes;
//     t_cmd_hist *command;
//     int i;
//     t_cmd_hist *temp;
//     t_cmd_hist *h;

//     h = NULL;
//     i = 0;
//     command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
//     if (!command)
//         return ;

//     setup_signals();
//     init_termios(&saved_attributes);

//     while (1)
//     {
//         char *input = read_command(data);

//         if (!input)
//         {
//             free(command);
//             break ;
//         }

//         command->history = strdup(input);
//         free(input);

//         if (strlen(command->history))
//         {
//             command->history_index = ++i;
//             command->history_size = strlen(command->history);
//             command->next = h;
//             h = command;
//         }

//         if (ft_strnstr(command->history, "exit", 4) != 0)
//             break ;

//         exec(command->history, data, h);
//         command = (t_cmd_hist *)malloc(sizeof(t_cmd_hist));
//         if (!command)
//             break ;
//     }

//     restore_termios(&saved_attributes);

//     while (h != NULL)
//     {
//         temp = h;
//         h = h->next;
//         free(temp->history);
//         free(temp);
//     }
// }
