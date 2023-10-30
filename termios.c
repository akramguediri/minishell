/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:47:52 by otuyishi          #+#    #+#             */
/*   Updated: 2023/10/30 16:19:27 by aguediri         ###   ########.fr       */
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
	size_t	input_size;
	size_t	read_bytes;

	input = NULL;
	input_size = 0;
	ft_getactivepath(data);
	read_bytes = getline(&input, &input_size, stdin);
	if (read_bytes == 0)
	{
		signal(SIGQUIT, handle_interrupt);
		perror("getline");
		exit(EXIT_FAILURE);
	}
	input[strcspn(input, "\n")] = '\0';
	return (input);
}
// char *read_command(t_data *data)
// {
//     char *input = NULL;
//     size_t input_size = 0;
//     ssize_t read_bytes;
//     char c;

//     while (1)
//     {
//         read_bytes = read(STDIN_FILENO, &c, 1);

//         if (read_bytes == -1)
//         {
//             perror("read");
//             exit(EXIT_FAILURE);
//         }

//         if (read_bytes == 0 || c == EOF)
//         {
//             printf("\nCtrl+D detected. Exiting...\n");
//             exit(EXIT_SUCCESS);
//         }

//         if (c == '\n')
//         {
//             if (input != NULL)
//             {
//                 input[strcspn(input, "\n")] = '\0';
//             }
//             break ;
//         }

//         // Append character to input
//         input = realloc(input, input_size + 2);
//         if (input == NULL)
//         {
//             perror("realloc");
//             exit(EXIT_FAILURE);
//         }
//         input[input_size++] = c;
//         input[input_size] = '\0';
//     }

//     return (input);
// }
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

void	execute_command(char *command)
{
	int		fd[2];
	pid_t	child_pid;
	char	buffer[4096];
	ssize_t	read_bytes;
	char	*token;
	int		arg_count;
	char	*full_path;
	char	**args;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		token = strtok(command, " ");
		args = NULL;
		arg_count = 0;
		while (token != NULL)
		{
			args = realloc(args, sizeof(char *) * (arg_count + 1));
			if (args == NULL)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
			args[arg_count] = token;
			arg_count++;
			token = strtok(NULL, " ");
		}
		args = realloc(args, sizeof(char *) * (arg_count + 1));
		if (args == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		args[arg_count] = NULL;
		full_path = find_command_in_path(args[0]);
		if (full_path == NULL)
		{
			fprintf(stderr, "Command not found: %s\n", args[0]);
			exit(EXIT_FAILURE);
		}
		execve(full_path, args, NULL);
		perror("execve");
		free(args);
		free(full_path);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(fd[1]);
		waitpid(child_pid, NULL, 0);
		while ((read_bytes = read(fd[0], buffer, sizeof(buffer))) > 0)
			write(STDOUT_FILENO, buffer, read_bytes);
		close(fd[0]);
	}
}

char	*ft_trim(char *s)
{
	int	i;
	int	j;
	int	length;

	i = 0;
	j = strlen(s);
	while (s[i] == ' ')
	{
		i++;
	}
	while (j > i && s[j - 1] == ' ')
	{
		j--;
	}
	length = j - i;
	char *str = (char *)malloc(length + 1); // +1 for the null terminator
	if (str)
	{
		strncpy(str, s + i, length);
		str[length] = '\0'; // Null-terminate the new string
	}
	return (str);
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
void	echo(char *s)
{
	int		n;
	char	output[1024];
	char	*r;
	int		red;
	int		i;
	int		j;
	char	c;

	//	char	**t;
	i = 0;
	j = ft_strlen(s);
	n = 0;
	red = 0;
	r = NULL;
	if (ft_strchr(s, '\"') == 0 || ft_strchr(s, '\'') == 0)
	{
		while (s[i] != '\'' && s[i] != '\"')
			i++;
		c = s[i];
		while (s[j] != c)
			j--;
		if (i != j)
		{
			r = (char *)malloc(j - i + 1);
			if (s[ft_strlen(s)] != c)
				ft_strlcpy(r, s + i + 1, j - i);
			else
				ft_strlcpy(r, s + i + 1, j - i - 1);
		}
		if (count_characters(s, c) % 2 != 0 && s[ft_strlen(s)] != c)
		{
			printf("%d\n\n\n", count_characters(s, c));
			while (count_characters(r, c) % 2 != 0)
			{
				n = read(0, output, sizeof(output));
				r = ft_strjoin(r, output);
			}
		}
		printf("%s", r);
	}
	else
		printecho(s);
	if (strnstr(s, "-n", 2) != 0)
		printf("\n");
}
void	handle_command(char *cmd, t_data *data, t_cmd_hist *h)
{
	char	*trimmed_cmd;

	trimmed_cmd = ft_trim(cmd);
	if (!ft_split(*ft_split(cmd,'\t'), ' ') || !ft_split(cmd, ' ') || !ft_split(cmd, '\t'))
		;
	else if (strcmp(trimmed_cmd, "clear") == 0)
	{
		custom_clear();
	}
	else if (ft_strncmp(trimmed_cmd, "env", 3) == 0)
	{
		printenvList(data->env);
	}
	else if (ft_strncmp(trimmed_cmd, "history", 7) == 0)
	{
		printhstList(h);
	}
	else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
	{
		ft_getactivepath(data);
	}
	else if (ft_strnstr(trimmed_cmd, "cd", 2) != 0)
	{
		cd(cmd);
	}
	else if (ft_strnstr(trimmed_cmd, "echo ", 5) != 0)
	{
		echo(cmd);
	}
	printf("\n");
}

void	run_command(char *cmd, t_data *data, t_cmd_hist *h)
{
//	handle_command(cmd, data, h);
	commandd(cmd, data, h);
	printf("\n");
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
