/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguediri <aguediri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:52:10 by aguediri          #+#    #+#             */
/*   Updated: 2023/10/16 16:27:30 by aguediri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/*c standard libraries*/
# include <stdio.h>
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
/*other*/
# include "executer/executer.h"
# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include "parser/parser.h"

/*linked list*/
typedef struct s_env
{
	char					*l;
	struct s_env			*next;
}							t_env;
typedef struct s_data
{
	char					*path;
	t_env					*env;

}							t_data;

// history
# define MAX_BUFFER_SIZE 1024
# define MAX_HISTORY_SIZE 100

typedef struct s_cmd_history
{
	char					*history;
	int						history_index;
	int						history_size;
	struct s_cmd_history	*next;

}							t_cmd_hist;

void						ft_getactivepath(t_data *data);
void						termios(t_data *data);
void						execute_command(char *command);
char						*find_command_in_path(const char *command_name);
void						custom_clear(void);
char						*read_command(t_data *data);
void						restore_termios(struct termios *saved_attributes);
void						init_termios(struct termios *saved_attributes);
void						handle_interrupt(int signal);
void						printenvList(t_env *envlist);
void						add_to_history(t_cmd_hist *history,
								const char *command);
void						printhstList(t_cmd_hist *envlist);
void						ft_lstaddh(t_cmd_hist **lst, t_cmd_hist *new);

#endif
