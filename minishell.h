/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:52:10 by aguediri          #+#    #+#             */
/*   Updated: 2023/11/23 12:49:00 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/*c standard libraries*/
# include <stdio.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
/*other*/
# include "libft/libft.h"

extern char					**environ;

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

typedef struct s_cmd_data
{
	char					**input;
	char					*input_file;
	char					*output_file;
	char					*commandlist;
	char					**t;
	char					*processed;
	int						num_cmds;
	int						r;
	int						here;
	char					*heredoc;
	int						z;
}							t_cmd_data;

typedef struct s_steroids
{
	char					**result;
	char					*s;
	char					*s2;
	int						j;
	int						l;
	int						k;
	int						len;
}							t_steroids;

typedef struct s_steroide
{
	char					**result;
	int						j;
	int						l;
	char					*s2;
	int						k;
	int						len;
}							t_steroide;

typedef struct s_execution_data
{
	int						**pipe_fd;
	pid_t					*children;
	char					**argv;
}							t_execution_data;

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

//=================minishell project functions===============================

//=================ft_split.c============================
void						free_split(char **words);

//============================termios.c==============================
void						termios(t_data *data);
void						exec(char *s, t_data *data, t_cmd_hist *h, int z);
void						printvar(char *s, char *argument);
void						restore_termios(struct termios *saved_attributes);
void						init_termios(struct termios *saved_attributes);

//============================splitonsteroids.c==============================
char						**ft_splitonsteroids(char *s, char c);
void						process_split_result(t_steroids *st, char *s,
								char c);
int							ft_split_start(const char *s, char c, int l);
int							ft_split_len(const char *s, int l, char c);

//============================signals.c==============================
void						setup_signals(void);
void						reset_handler(int signo);
void						custom_clear(void);
void						eof_handler(int errnum);
void						handle_interrupt(int signal);

//============================minishell.c.c==============================
void						ft_get(t_data *data);
const char					*ft_getactivepath(t_data *data, int j);
void						ft_init(char **env, t_data **data);
void						printhst_list(t_cmd_hist *envlist);
void						printenvlist(t_env *envlist);

//============================main.c.c==============================
int							main(int argc, char **argv, char **env);
void						free_up(char **arr);
void						wait_for_children(int num_cmds, pid_t children[],
								int z);
int							error_exit(char *s);

//============================io_files.c.c==============================
char						*process_output_file(char **input, int *i, int *r);
char						*process_input_file(char **input, int *i);
char						*extract_output_file(char *command, int *i,
								int len);
char						*extract_input_file(char *command, int *i, int len);
void						redirect_output(int i, struct s_cmd_data cmddata,
								int **pipe_fd);
void						redirect_input(int i, char *input_file,
								int **pipe_fd);

//============================history.c.c==============================
void						process_history(t_cmd_hist *command, int *i,
								t_cmd_hist **h);
void						free_history_nodes(t_cmd_hist *h);
int							handle_history(t_cmd_hist *h);
void						ft_lstaddback(t_env **lst, t_env *new);

//============================heredoc.c.c==============================
char						*heredoc(const char *delimiter);
void						*ft_realloc(void *ptr, size_t old_size,
								size_t new_size, char **new_ptr);
void						process_heredoc(struct s_cmd_data *cmddata,
								char *input);
void						redirect_heredoc(char *s);

//============================handling_cmd2.c.c==============================
char						*process_regular_command(struct s_cmd_data *cmddata,
								char *command);
void						process_s_cmd_data(struct s_cmd_data *cmddata,
								char *input2);
void						initialize_s_cmd_data(struct s_cmd_data *cmddata,
								char *input2);
char						*process_command(char *command, char *input_file,
								char *output_file);
int							execute_command2(char *cmd, t_data *data,
								t_cmd_hist *h, int z);

//============================handling_cmd1.c.c==============================
int							commandd(char *input2, t_data *data, t_cmd_hist *h,
								int z);
void						run_command(char *cmd, t_data *data, t_cmd_hist *h,
								int z);
int							handle_command(char *cmd, t_data *data,
								t_cmd_hist *h, int z);
void						process_command3(t_cmd_hist *command, t_data *data,
								t_cmd_hist *h, int z);
char						*read_command(t_data *data);

//=============================handle_path.c===================================
void						handle_dotdot(const char *input,
								char *current_directory);
void						change_directory(const char *new_path);
char						*get_parent_path(const char *path);
char						*get_current_directory(void);

//===============================ft_piping.c===================================
char						*outputvar(char *s);
char						*extract_regular_chars(char *command, int *i,
								int len);
void						pipes_io_redir(struct s_cmd_data cmddata,
								t_data *data, t_cmd_hist *h);
void						create_pipes(int num_pipes, int **pipe_fd);

//===============================ft_execvp.c===================================
void						free_resources(char *path_copy, char *full_path);
char						*get_path_copy(void);
char						*build_full_path(const char *dir, const char *file);
int							execute_with_path(const char *file,
								char *const argv[], char *path_copy);
int							ft_execvp(const char *file, char *const argv[]);

//===============================env.c======================================
void						printenv(void);
void						my_unset(char *arg);
void						my_export(char *arg);
int							handle_env(void);
void						addtoenv(char *s);

//===============================echo.c======================================
void						echo(char *s, int z);
void						handle_echo_options(char **tokens,
								int *start_index);
void						print_echo_argument(char *argument, int z);
void						printecho(char *s);
int							handle_echo(char *cmd, int z);

//===========================counting.c========================================
int							countpipes(char **t, char c);
int							count_characters(const char *s, char c);
int							ft_wordcount(char const *s, char c);

//===========================builtins.c========================================
int							handle_pwd(t_data *data);
int							handle_clear(void);
void						cd(char *input);
void						handle_cd_command(char *current_directory);

#endif
