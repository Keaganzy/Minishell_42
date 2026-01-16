/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 14:59:42 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:26:14 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>

# define PROMPT "MS$"
# define BUFFER_SIZE 1024
# define HISTORY_FILE "~/.minishell_history"

extern volatile sig_atomic_t	g_sigint_received;

typedef struct s_token			t_token;
typedef struct s_ast			t_ast;

typedef struct s_shell
{
	int				exit_code;
	int				last_exit_status;
	int				stdin_backup;
	int				stdout_backup;
	int				running;
	char			**envp;
	t_token			*tokens;
	t_ast			*ast;
}					t_shell;

typedef enum e_node_type
{
	N_CMD,
	N_PIPE,
	N_REDIR_IN,
	N_REDIR_OUT,
	N_REDIR_APPEND,
	N_HEREDOC,
	N_OR,
	N_AND
}					t_node_type;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_OPEN_BRACKET,
	T_CLOSE_BRACKET,
	T_AND,
	T_OR,
	T_SPACE
}					t_token_type;

struct				s_token
{
	int				fd;
	char			*value;
	t_token_type	type;
	struct s_token	*next;
};

struct				s_ast
{
	t_node_type		type;
	int				fd;
	char			**argv;
	char			*filename;
	char			*heredoc_content;
	struct s_ast	*left;
	struct s_ast	*right;
};

typedef struct s_parser
{
	t_token			*current;
	t_token			*tokens;
}					t_parser;

typedef struct s_quote_state
{
	int				in_single;
	int				in_double;
	int				in_curly_brackets;
	int				flag;
}					t_quote_state;

typedef struct s_expand_ctx
{
	char			**env;
	t_quote_state	state;
	int				i;
	int				j;
}					t_expand_ctx;

typedef struct s_exp
{
	char			*out;
	char			*map;
	int				i;
	t_quote_state	state;
}					t_exp;

typedef struct s_copy_word
{
	int		start;
	int		end;
	char	*pat;
	char	*exp;
	int		k;
}	t_copy_word;

typedef struct s_copy_state
{
	char	*result;
	int		j;
}	t_copy_state;

typedef struct s_calc_len_params
{
	char	*start;
	t_shell	*shell;
}	t_calc_len_params;

/* Signal & Environment */
void			set_signals(void);
char			*getenv_value(char **env, const char *key);
int				setenv_value(char ***envp, const char *key, const char *value);

/* Lexer Module */
int				check_commas_closed(char *s);
int				check_invalid_pipes(char *s);
int				is_space(char c);
int				check_syntax(char *s);
int				check_has_equals(char *s);
int				is_fd_redir(const char *s, size_t i);
int				extract_fd_for_lexer(const char *s, size_t *i);
int				check_valid_ampersand(char *s);
int				check_valid_brackets(char *s);
int				check_has_semicolon(char *s);
int				parse_quotes(char **wrd, char *s, t_token **tokens, size_t *i);
int				parse_word(char **word, char *s, t_token **tokens, size_t *i);
char			*ft_strjoin_and_free(char **new_s, char *substr, int to_free);
char			*ft_strndup(const char *src, size_t num);
char			*extract_word(const char *s, size_t *i);
char			*process_heredoc(char *delimiter, int is_quoted, t_shell *sh);
char			*expand_heredoc_line(char *line, t_shell *shell);
char			*extract_till_next_inv_comma(const char *s, size_t *i);
char			*extract_word_with_inv_commas(const char *s, size_t *i);
void			free_tokens(t_token *tokens);
void			add_token_back(t_token **lst, t_token *new);
void			token_free(t_token *tok);
void			token_free_all(t_token **lst);
void			print_tokens(t_token *token);
void			print_token_stream_colored(t_token *tokens);
t_token			*lex_input(const char *s, t_shell *shell);
t_token			*tokenize_input(const char *input);
t_token			*token_new(t_token_type type, const char *value, int fd);
t_token_type	get_op_type(const char *s, size_t *i);
t_token_type	handle_space(const char *s, size_t *i);

/* History Module */
void			history_init(void);
void			history_add(const char *line);

/* Executor Module */
int				execute_ast(t_ast *root, t_shell *shell);
int				execute_node(t_ast *node, t_shell *shell);
int				execute_cmd(t_ast *node, t_shell *shell);
int				is_builtin(t_ast *ast);
int				execute_builtin(t_ast *ast, t_shell *shell);
int				execute_pipe(t_ast *node, t_shell *shell);
int				execute_redir(t_ast *node, t_shell *shell);
int				setup_redir_in(t_ast *node, t_shell *shell);
int				setup_redir_out(t_ast *node, t_shell *shell);
int				setup_redir_append(t_ast *node, t_shell *shell);
int				setup_heredoc(t_ast *node, t_shell *shell);
int				execute_logical(t_ast *node, t_shell *shell);
int				setup_redirections(t_ast *node, t_shell *shell);
char			**get_paths_from_env(char **envp);
char			*expand_and_replace(char **s, t_shell *shell);
int				is_file_descriptor(const char *str);
int				match_recursive(char *str, char *pat);
char			*get_home_dir(t_shell *shell);
char			*get_env_value(char *var_name, t_shell *shell);
size_t			get_var_len(char *s);
void			init_quote_state(t_quote_state *state);
char			**get_matches(char *pattern);
int				add_dir_match(char **matches, int *count, char *dname, char *name);
int				add_match(char **matches, int *count, char *name);
int				match_pattern(char *str, char *pattern);
char			*join_matches(char **matches);
size_t	calc_var_len_helper(char *s, t_shell *shell, int *need_free);
size_t	calc_len(char *s, t_shell *shell);
int		exp_tilde(char **s, t_exp *e, t_shell *shell);
int		exp_var(char **s, t_exp *e, t_shell *shell);
int             expand_process_char(char **s, t_exp *e, t_shell *shell);
char	*expand_strip(char *s, t_shell *shell, char **map_out);
size_t	calc_split_len(char *s, char *map);
char	*split_words(char *s, char *map);
int		has_wildcard(char *s, char *map, int start, int end);
int		word_end(char *s, int i);
char	*trim_pattern(char *pattern);
char	**get_matches_in_dir(char *dname, char *pattern);
void	free_matches(char **matches);
char	*expand_pattern(char *pattern);
size_t	calc_wild_len(char *s, char *map);
char	*expand_wild(char *s, char *map);
char	*expand_and_replace(char **s, t_shell *shell);
char	*split_words(char *s, char *map);
void	do_split_words(char *s, char *map, char *result);
char	*split_words_exec(char *s, char *map);
void	copy_word(char *s, char *map, int *i, t_copy_state *cs);
int	exp_var(char **s, t_exp *e, t_shell *shell);
int	exp_var_single(char **s, t_exp *e, size_t len);
void	process_calc_len_char(char **s, size_t *len, t_quote_state *state, t_calc_len_params *p);
int		exec_builtin_part1(char *cmd, int len, t_ast *ast, t_shell *shell);
int		exec_builtin_part2(char *cmd, int len, t_ast *ast, t_shell *shell);
void	execute_external_child(t_ast *ast, t_shell *shell);
int		handle_wait_status(int status);
char	*find_command_path(char *cmd, char **envp);

/* Builtins Module */
void			free_substr(char **substr);
int				add_update_env_vars(t_shell *shell, const char *av);
char			*handle_dollars_tilde(char *chunk, t_shell *shell);
int				validate_identifiers(char **av);
int				ft_echo(char **av, t_shell *shell);
int				builtin_cd(char **av, t_shell *shell);
int				ft_pwd(char **av, t_shell *shell);
int				ft_env(char **av, t_shell *shell);
int				ft_exit(char **av, t_shell *shell);
int				ft_unset(char **av, t_shell *shell);
int				ft_export(char **av, t_shell *shell);
int				unsetenv_value(char ***envp, const char *key);
char			*ft_strjoin_char_and_free(char **new_s, char c, int to_free);
char			**setup_args_arr(char **new_av, int *i, int *n_flag);
int				var_check(char *var);
int				match_last(char *str, char **chunks, int n, int end_star);
int				match_middle(char *str, char **chunks, int n, int *pos);
int				match_first(char *str, char **chunks, int *pos, int start_star);
int				set_env_variable(t_shell *shell, const char *av,
					int eq_pos, char *key);
size_t			expand_variable(char *chunk, size_t k,
					char **s_final, t_shell *shell);
int				find_equals_pos(const char *av);
int				validate_key(char *key);
int				check_numeric_arg(char *str);
int				has_overflow(char *str);
void			print_ast(t_ast *node);
void			sort_envp_vars(char **env_var);
void			print_env_vars_export(char **duped_env);
void			print_env_vars_env(char **duped_env);

/* Parser Module */
t_ast			*parse(t_token *tokens, t_shell *shell);
t_ast			*parse_and(t_parser *p, t_shell *shell);
t_ast			*parse_command(t_parser *p, t_shell *shell);
t_ast			*parse_one_redir(t_parser *p, t_ast *cmd, t_shell *shell);
t_ast			*parse_logical(t_parser *p, t_shell *shell);
void			skip_spaces(t_parser *p);
t_token			*peek(t_parser *p);
t_token			*advance(t_parser *p);
int				match(t_parser *p, t_token_type type);
t_token			*expect(t_parser *p, t_token_type type);
int				is_redirection(t_token_type type);
int				count_words(t_parser *p);
char			*read_heredoc_content(char *delimiter);
t_ast			*create_node(t_node_type type);
t_ast			*create_binary_node(t_node_type type, t_ast *left,
					t_ast *right);
t_ast			*create_redir_node(t_node_type type, char *filename,
					t_ast *cmd, int fd);
t_ast			*new_ast(t_node_type type);
void			free_ast(t_ast *node);
t_ast			*parser_word(t_parser *p);
t_node_type		get_redir_type(t_token_type type);
void	handle_sigint_heredoc(char *line, char *content);
int	check_heredoc_end(char *line, char *delimiter);
char	*append_line_to_content(char *content, char *line);
char	*read_heredoc_content(char *delimiter);
void	setup_sigact_heredoc(char **content);
char	*strip_quotes(const char *str, int *flag);
int		validate_redir_token(t_parser *p, t_ast *cmd, t_shell *shell);
t_token	*get_file_token(t_parser *p, t_ast *cmd, t_shell *shell);
t_ast	*handle_heredoc(t_node_type type, t_token *file_token,
t_ast *cmd, t_shell *shell);
t_ast	*parse_prefix_redirections(t_parser *p, t_shell *shell);
t_ast	*parse_suffix_redirections(t_parser *p, t_ast *cmd, t_shell *shell);
t_ast	*find_bottom_left(t_ast *cmd);
t_ast	*attach_command_to_redir(t_parser *p, t_ast *cmd, t_shell *shell);
t_ast	*handle_command_parse(t_parser *p, t_ast *cmd, t_shell *shell);
t_ast	*handle_no_command(t_ast *cmd, t_shell *shell);
t_ast	*parse_redirection(t_parser *p, t_shell *shell);


/* Shell Utils */
char			**cleanup_dup_envp(t_shell *shell, int index);
void			print_err(const char *s, char *av);
void			cleanup_shell(t_shell *shell);
void			free_array(char **arr);
void			print_indent(int depth, int is_last[], int is_right);
void			print_node_info(t_ast *node, int depth);
void			print_ast_simple(t_ast *node, int depth);
void			print_node_argv(t_ast *node, int depth);
void			print_args_detailed(char **args, int depth);
void			print_ast_tree(t_ast *root);
void			print_node_compact(t_ast *node);
void			print_node_detailed(t_ast *node, int depth, char *position);
void			print_ast_detailed(t_ast *node, int depth, char *position);
void			print_node_detailed_body(t_ast *node, int depth);
void			print_tree_recursive(t_ast *node, int depth, int is_last[],
					int is_right);
const char		*node_type_to_string(t_node_type type);
const char		*token_type_to_string(t_token_type type);

/* Main */
char			**dup_envp(t_shell *shell, char **envp);

#endif
