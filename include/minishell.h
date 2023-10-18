/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:17:42 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/14 21:16:19 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX			4096

// Colors for the terminal
# define ANSI_RED		"\x1b[31m"
# define ANSI_GREEN		"\x1b[32m"
# define ANSI_YELLOW	"\x1b[33m"
# define ANSI_BLUE		"\x1b[34m"
# define ANSI_MAGENTA	"\x1b[35m"
# define ANSI_CYAN		"\x1b[36m"
# define ANSI_RESET		"\x1b[0m"

// Bash error codes
# define B_SUCCESS		0	// Command executed with no errors
# define B_GENERIC		1	// Code for generic errors
# define B_USAGE		2	// Incorrect command (or argument) usage
# define B_NOT_PERM		126	// Permission denied
# define B_NOT_FOUND	127	// Command not found, or PATH error
# define B_CTRL_C		130	// Termination by Ctrl+C or SIGINT

// Opening modes for redirections
# define R_INPUT		O_RDONLY
# ifdef __linux__
#  define R_OUTPUT		577 // O_WRONLY | O_CREAT | O_TRUNC
#  define R_APPEND		1089 // O_WRONLY | O_CREAT | O_APPEND
# elif __APPLE__
#  define R_OUTPUT		1537 // O_WRONLY | O_CREAT | O_TRUNC
#  define R_APPEND		521 // O_WRONLY | O_CREAT | O_APPEND
# endif /* OS Check */

// Signal handling
# define ON_FT_READLINE	0
# define ON_BACKGROUND	1
# define ON_FD			3
// Stylized perror
# define MSH_ERR		"\x1b[32mminishell: \x1b[0m"

# include "ft.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>

typedef struct s_msh
{
	t_list	**tenvp;
	t_list	**pids;
	char	**line_read_split;
	char	**l_exp;
	char	**paths;
	char	**envp;
	char	*line_read;
	char	*ofile;
	int		*is_quoted;
	int		fdout;
	int		fdin;
	int		out_mode;
}			t_msh;

typedef struct s_process
{
	char	*cpath;
	int		fork_id;
	int		pipe_fd[2];
	int		wstatus;
}			t_process;

extern sig_atomic_t	g_error;

void	minishell(t_msh *tm);

int		has_quotes(char *token);

void	is_quoted_array(t_msh *tm);

//===============================UTILS================================
/**
 * @brief Frees a double pointer from memory
 * @param ptr The double pointer to be freed
*/
void	dbl_free(char **ptr);

char	*path_join(char *s1, char *s2);

char	*create_prompt(t_msh *tm);

int		is_built_in(char *cmd);

void	free_on_loop(t_msh *tm);

//===============================LEXER================================

char	*ft_readline(t_msh *tm);
void	get_quote_state(char *quote_state \
						, const char *char_checked \
						, const char *string_start);
int		is_quote(char c);
void	get_quote_state_i(char const *line_read, char **quote_state_i);
void	exit_on_ctrl_d(char *line_read, t_msh *tm);
int		check_quote_completion(char *line_read);
void	fix_open_quotes(char **line_read);
int		line_has_content(char *line_read, t_msh *tm);
char	*get_line_with_content(char **line_read, char *prompt, t_msh *tm);
char	*get_line_quote_complete(char **line_read, char *prompt, t_msh *tm);

void	lexer(char **line_read);
int		is_special(char **str, int i);
int		is_metacharacter(char *c);
int		is_operator(char *c);
int		insert_spaces(char **str, int pos, int gap);
char	*get_brace_state_i(const char *token);
int		check_braces_completion(char *line_read);
char	*get_line_brace_complete(char **line_read, char *prompt, t_msh *tm);
int		get_spaces_to_insert(char *str, int pos, int size);

//=============================EXPANDER================================

char	**line_expander(char ***line_split, t_list *lenv);
char	**unpack_char_pp(char ***char_pp);
void	fill_char_pp(char ***ret_addr, char ***src_addr);
char	*replace_string_chunk(char **host, char **insert, size_t d_pos);
void	quote_remove(char *token);

char	*sh_par_expander(char *token, t_list *lenv);
char	*get_env_var_name(char *lexed_line, int d_pos);
char	*get_env_var_value(char *lexed_line, int d_pos, t_list *lenv);
char	*get_var_start(char *lexed_line, size_t d_pos);
char	*get_var_end(char *lexed_line, size_t d_pos);
char	*get_parameter_end(char *token, int d_pos);
char	*get_env(t_list *env_list, char *var_name);
char	*get_env_sp(char c);

char	*fn_expander(char *pattern);
int		glob_engine(char *pattern, char *target);

//===============================PARSER================================

/**
 * @brief Populates a double pointer with the command table
 * @param split_rl Split read line from prompt
 * @param paths Double pointer from PATH enviroment variable
*/
void	get_command_table(t_msh *tm);

/**
 * @brief A customized split function to properly parse quoted arguments
 * i.e. awk '{print $2}' or sed 's/word1/word2/g'
*/
char	**ft_split_args(char *str);
int		ft_wordcount(char *str);

/**
 * @brief Opens the file for redirection and dumps the content
 * to STDIN or STDOUT
*/
int		create_input(char *file, int mode, int std_io);

int		restore_io(int fdin, int fdout);

int		is_redirector(char *str);

int		op_selector(t_msh *tm, int index, char **ofile, int no_hd);

int		check_inputs(t_msh *tm);

int		create_output(t_msh *tm, char *file, int mode, char **ofile);

int		is_pipe(t_msh *tm, int index, char *command);

int		is_empty_cmd(char *cmd);

//===============================EXECUTOR================================

/**
 * @brief Dumps the STDOUT to a file, if specified
*/
void	dup_ofile(t_msh **tm);

/**
 * @brief Executes as many processes as needed
*/
void	exec_child(char *command, t_msh *tm);

/**
 * @brief Executes the last command from input
*/
void	exec_parent(char *command, t_msh *tm);

/**
 * @brief Starts a here_doc input method
*/
void	here_doc(char *delimiter);

/**
 * @brief Function to free allocated memory and exit.
*/
int		ft_exit(t_msh *tm, int wstatus);

int		ft_perror(int err_num, char **cmd, char *str, int err_mode);

void	export_last_command(char *cmd, t_msh *tm);

void	dup_input(int pipe_fd[2], char **args);

void	lst_wait_children(void *msg);

void	wait_for_children(t_msh *tm);

void	add_fork_id(pid_t id, t_msh *tm);

int		pipe_and_fork(t_process *proc);

//===============================BUILT-INS================================
int		env(t_list **tenvp);
int		pwd(void);
int		cd(char **path, t_list *lenv, char ***envp_addr);
int		echo(char **cmd);

int		export(char **args, t_list *envs, char ***envp, int inner_call);
int		read_new_env_var(char **var_name, char *var);
int		valid_var_name(char *var_name);
void	sort_list(t_list *lst);
t_list	*copy_list(t_list *src);
void	print_node_export(void *msg);

int		unset(char **args, t_list **envs_addr, char ***penv_addr);
void	remove_matched_nodes(t_list **envs_addr, char *arg);

//=========================MODIFIED FT_SPLIT===========================

int		get_w_nbr_msh(char const *s, char c, char *quote_state_i, char *bsi);
int		get_w_len_msh(char const *s, char c, char *quote_state_i, char *bsi);
char	**err_free_msh(char **list, int i);
char	**build_list_msh(char **ret, char const *s, \
						char c, char **token_groupers_i);
char	**ft_split_msh(char const *s, char c);

//=========================SIGNAL HANDLING=============================

void	set_signal_action(int pid);

//=========================ENV LINKED LIST=============================

t_list	**fill_penv_list(char **penv);
char	**fill_penv_dptr(t_list **lst);

//=============================ERRORS=================================

int		input_error(t_msh *tm, char **ofile);

void	cmd_not_found(t_msh *tm, char **args);

int		check_exit_arg(char *arg, char **cmd);

#endif /* MINISHELL_H */
