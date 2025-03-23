#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define EMPTY_AST 1

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

typedef struct s_allocation
{
	void *ptr;
	struct s_allocation *next;
} t_alloc;

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef enum e_scan_status
{
	SCAN_SUCCESS,
	SCAN_UNCLOSED_QUOTE,
	SCAN_INVALID_OPERATOR
} t_scan_status;

typedef enum e_logical_op
{
	LOGICAL_AND,
	LOGICAL_OR
} t_logical_op;

typedef enum e_ast_type
{
	NODE_LOGICAL,
	NODE_PIPELINE,
	NODE_COMMAND,
	NODE_SUBSHELL
} t_ast_type;

typedef struct s_redir
{
	t_token_type type;
	char *file;
} t_redir;

typedef struct s_ast t_ast;

typedef struct s_ast
{
	t_ast_type type;
	union
	{
		struct
		{
			t_logical_op operat;
			t_ast *left;
			t_ast *right;
		} s_op;
		struct
		{
			t_ast **commands;
			int count;
		} s_pipeline;
		struct
		{
			char **argv;
			t_redir *redirects;
			size_t redirect_count;
		} s_cmd;
		struct
		{
			t_ast *command;
			t_redir *redirects;
			size_t redirect_count;
		} s_subshell;
	} u_data;
} t_ast;

int ft_toupper(int c);
int ft_tolower(int c);
int ft_isalpha(int c);
int ft_isascii(int c);
int ft_isalnum(int c);
int ft_isdigit(int c);
int ft_isprint(int c);
int ft_atoi(const char *nptr);
int ft_memcmp(const void *s1, const void *s2, size_t n);
int ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strlen(const char *s);
char *ft_itoa(int n);
char *ft_strdup(const char *s);
char *ft_strchr(const char *s, int c);
char **ft_split(const char *s, char c);
char *ft_strrchr(const char *s, int c);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(const char *s1, const char *set);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
char *ft_substr(const char *s, unsigned int start, size_t len);
char *ft_strnstr(const char *haystack, const char *needle, size_t len);
void ft_putnbr_fd(int n, int fd);
void ft_bzero(void *s, size_t n);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
void *ft_memset(void *s, int c, size_t n);
void *ft_calloc(size_t nmemb, size_t size);
void *ft_memchr(const void *s, int c, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
void ft_striteri(char *s, void (*f)(unsigned int, char *));
size_t ft_strlcpy(char *dest, const char *src, size_t size);
size_t ft_strlcat(char *dest, const char *src, size_t size);
t_ast *parse_subshell(t_token **tokens);
t_ast *parse_pipeline(t_token **tokens);
t_ast *parse_logical_expr(t_token **tokens);
t_ast *ft_parse(t_token **tokens);
t_ast *parse_factor(t_token **tokens);
t_ast *create_logical_node(t_logical_op op, t_ast *left, t_ast *right);
t_ast *handle_redirections(t_ast *node, t_token **tokens);
t_ast *create_pipeline_node(void);
t_ast *create_command_node(void);
t_ast *create_subshell_node(void);
void add_redirect(t_ast *cmd_node, t_token_type type, char *file);
void add_argument(t_ast *cmd_node, char *arg);
void add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node);
int is_redirect(t_token *token);
int is_cmd_finished(t_token *token);
void print_ast_recursive(t_ast *node, int level, int is_last);
char *ft_strcat(char *dest, const char *src);
char *ft_realloc(char *ptr, size_t size);
char *ft_strcpy(char *dst, const char *src);
void print_indent(int level, int is_last);
void print_command_node(t_ast *node, int level, int is_last);
void print_pipeline_node(t_ast *node, int level, int is_last);
void print_logical_node(t_ast *node, int level, int is_last);
void print_subshell_node(t_ast *node, int level, int is_last);
void print_ast(t_ast *root);
t_env **get_env_list(void);
void init_env(char **envp);
char *get_env_value(char *key);
void set_env(char *key, char *value);
void unset_env(char *key);
char **env_to_array(void);
t_env *create_env_node(char *key, char *value);
void link_env_node(t_env **head, t_env *node);
void parse_env_var(const char *var);
int ft_execute(t_ast *root, char *envp[]);
int execute_command(t_ast *cmd, char *envp[]);
int execute_pipeline(t_ast *node, char *envp[]);
int execute_recursive(t_ast *node, char *envp[]);
int redirect(t_redir *redirects, size_t count);
char *search_path(char *cmd, char *envp[]);
int ft_swap_nodes(t_token *node1, t_token *node2);
char *handle_heredoc(char *delim);
t_token *ft_heredoc(t_token *tokens);
t_alloc **get_alloc_list(void);
void free_all(void);
void *ft_malloc(size_t size);
void handle_quotes(char c, bool *in_single, bool *in_double);
bool valid_operator(const char *input, int pos);
bool dual_operator(const char *input, int pos);
char *translate_message(t_scan_status status);
void skip_operator(const char *input, int *i);
t_scan_status ft_scan(const char *input);
bool is_operator_char(char c);
char *extract_quoted_string(const char *input, int *pos, char quote);
void handle_operator(const char *input, int *pos, t_token **tokens);
void add_token(t_token **head, t_token *new_token);
t_token *create_token(t_token_type type, char *value);
t_token *ft_tokenize(const char *input);
bool is_whitespace(char c);
int ft_strcmp(const char *s1, const char *s2);
bool is_operator_char(char c);
char *ft_strndup(const char *s, size_t n);
int spit_error(int status, char *message);
char *ft_strjoin_three(char const *s1, char const *s2, char const *s3);
int env_listsize(t_env *lst);

int env_listsize(t_env *lst)
{
	int count;
	t_env *current;

	count = 0;
	current = lst;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}

char *ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char *str;
	size_t len1;
	size_t len2;
	size_t len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	str = ft_malloc(len1 + len2 + len3 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	ft_memcpy(str + len1 + len2, s3, len3);
	str[len1 + len2 + len3] = '\0';
	return (str);
}

int spit_error(int status, char *message)
{
	if (message)
		perror(message);
	return (status);
}

bool is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

bool is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char *ft_strndup(const char *s, size_t n)
{
	char *str;
	size_t i;

	if (!s)
		return (NULL);
	str = ft_malloc(n + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < n && s[i] != '\0')
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

t_ast *create_subshell_node(void)
{
	t_ast *node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	node->u_data.s_subshell.command = NULL;
	node->u_data.s_subshell.redirects = NULL;
	node->u_data.s_subshell.redirect_count = 0;
	return (node);
}

t_ast *create_command_node(void)
{
	t_ast *node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->u_data.s_cmd.argv = NULL;
	node->u_data.s_cmd.redirects = NULL;
	node->u_data.s_cmd.redirect_count = 0;
	return (node);
}

t_ast *create_pipeline_node(void)
{
	t_ast *node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_PIPELINE;
	node->u_data.s_pipeline.commands = NULL;
	node->u_data.s_pipeline.count = 0;
	return (node);
}

t_ast *create_logical_node(t_logical_op op, t_ast *left, t_ast *right)
{
	t_ast *node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_LOGICAL;
	node->u_data.s_op.operat = op;
	node->u_data.s_op.left = left;
	node->u_data.s_op.right = right;
	return (node);
}

static void get_redirect_info(t_ast *node, t_redir **redirects, size_t *count)
{
	if (node->type == NODE_COMMAND)
	{
		*redirects = node->u_data.s_cmd.redirects;
		*count = node->u_data.s_cmd.redirect_count;
	}
	else if (node->type == NODE_SUBSHELL)
	{
		*redirects = node->u_data.s_subshell.redirects;
		*count = node->u_data.s_subshell.redirect_count;
	}
	else
	{
		*redirects = NULL;
		*count = 0;
	}
}

void add_redirect(t_ast *node, t_token_type type, char *file)
{
	t_redir *old_redirects;
	t_redir *new_redirects;
	size_t old_count;

	get_redirect_info(node, &old_redirects, &old_count);
	new_redirects = ft_malloc(sizeof(t_redir) * (old_count + 1));
	ft_memmove(new_redirects, old_redirects, sizeof(t_redir) * old_count);
	new_redirects[old_count].type = type;
	new_redirects[old_count].file = ft_strdup(file);
	if (node->type == NODE_COMMAND)
	{
		node->u_data.s_cmd.redirects = new_redirects;
		node->u_data.s_cmd.redirect_count = old_count + 1;
	}
	else if (node->type == NODE_SUBSHELL)
	{
		node->u_data.s_subshell.redirects = new_redirects;
		node->u_data.s_subshell.redirect_count = old_count + 1;
	}
}

void add_argument(t_ast *cmd_node, char *arg)
{
	char **new_args;
	size_t arg_count;

	arg_count = 0;
	if (cmd_node->u_data.s_cmd.argv)
		while (cmd_node->u_data.s_cmd.argv[arg_count])
			arg_count++;
	new_args = ft_malloc(8 * (arg_count + 2));
	ft_memmove(new_args, cmd_node->u_data.s_cmd.argv, 8 * arg_count);
	cmd_node->u_data.s_cmd.argv = new_args;
	cmd_node->u_data.s_cmd.argv[arg_count] = ft_strdup(arg);
	cmd_node->u_data.s_cmd.argv[arg_count + 1] = NULL;
}

void add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node)
{
	size_t new_size;
	t_ast **new_commands;

	new_size = pipeline_node->u_data.s_pipeline.count + 1;
	new_commands = ft_malloc(sizeof(t_ast *) * new_size);
	ft_memmove(new_commands, pipeline_node->u_data.s_pipeline.commands, sizeof(t_ast *) * (new_size - 1));
	pipeline_node->u_data.s_pipeline.commands = new_commands;
	pipeline_node->u_data.s_pipeline.commands[pipeline_node
												  ->u_data.s_pipeline.count] = cmd_node;
	pipeline_node->u_data.s_pipeline.count++;
}

t_ast *handle_redirections(t_ast *node, t_token **tokens)
{
	t_token_type redirect_type;
	t_token *current;

	while (*tokens)
	{
		current = *tokens;
		if (!is_redirect(current))
			break;
		redirect_type = current->type;
		*tokens = current->next;
		if (!*tokens || ((*tokens)->type != TOKEN_WORD))
			return (NULL);
		add_redirect(node, redirect_type, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
	return (node);
}

t_ast *ft_parse(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_expr(tokens));
}

int is_redirect(t_token *token)
{
	if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int is_cmd_finished(t_token *token)
{
	if (token->type == TOKEN_PIPE || token->type == TOKEN_AND || token->type == TOKEN_OR)
		return (1);
	return (0);
}

t_ast *parse_subshell(t_token **tokens)
{
	t_ast *subshell_node;
	t_ast *command_node;

	*tokens = (*tokens)->next;
	command_node = parse_logical_expr(tokens);
	if (!*tokens || !((*tokens)->type == TOKEN_PAREN_CLOSE))
		return (NULL);
	*tokens = (*tokens)->next;
	subshell_node = create_subshell_node();
	subshell_node->u_data.s_subshell.command = command_node;
	return (subshell_node);
}

t_ast *parse_simple_command(t_token **tokens)
{
	t_token *current;
	t_ast *cmd_node;

	cmd_node = create_command_node();
	while (*tokens)
	{
		current = *tokens;
		if (is_cmd_finished(current) || is_redirect(current))
			break;
		if (current->type == TOKEN_WORD)
		{
			add_argument(cmd_node, current->value);
			*tokens = current->next;
			continue;
		}
		return (NULL);
	}
	return (cmd_node);
}

t_ast *parse_command_with_redirects(t_token **tokens)
{
	t_ast *node;

	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
		node = parse_subshell(tokens);
	else
		node = parse_simple_command(tokens);
	return (handle_redirections(node, tokens));
}

t_ast *parse_pipeline(t_token **tokens)
{
	t_ast *pipeline_node;
	t_ast *cmd_node;

	pipeline_node = create_pipeline_node();
	cmd_node = parse_command_with_redirects(tokens);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd_node = parse_command_with_redirects(tokens);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	if (pipeline_node->u_data.s_pipeline.count == 1)
	{
		cmd_node = pipeline_node->u_data.s_pipeline.commands[0];
		return (cmd_node);
	}
	return (pipeline_node);
}

t_ast *parse_logical_expr(t_token **tokens)
{
	t_logical_op op;
	t_ast *result;
	t_ast *right;
	t_ast *left;

	left = parse_pipeline(tokens);
	while (*tokens && ((*tokens)->type == TOKEN_AND || (*tokens)->type == TOKEN_OR))
	{
		op = (*tokens)->type - 6;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		result = create_logical_node(op, left, right);
		left = result;
	}
	return (left);
}

t_env *create_env_node(char *key, char *value)
{
	t_env *node;

	if (!key || !value)
		return (NULL);
	node = ft_malloc(sizeof(t_env));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

void link_env_node(t_env **head, t_env *node)
{
	t_env *current;

	if (!head || !node)
		return;
	if (!*head)
	{
		*head = node;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = node;
}

void parse_env_var(const char *var)
{
	int i;
	char *key;
	char *pos;
	char *value;
	t_env **env_list;

	i = 0;
	env_list = get_env_list();
	pos = ft_strchr(var, '=');
	key = ft_strndup(var, pos - var);
	value = ft_strdup(pos + 1);
	link_env_node(env_list, create_env_node(key, value));
}

t_env **get_env_list(void)
{
	static t_env *env_list;

	return (&env_list);
}

void init_env(char **envp)
{
	int i;

	i = 0;
	if (!envp || !*envp)
		return;
	while (envp[i])
	{
		parse_env_var(envp[i]);
		i++;
	}
}

char *get_env_value(char *key)
{
	t_env **env_list;
	t_env *current;

	env_list = get_env_list();
	if (!env_list || !*env_list)
		return (NULL);
	current = *env_list;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void set_env(char *key, char *value)
{
	t_env **env_list;
	t_env *current;

	env_list = get_env_list();
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	link_env_node(env_list, create_env_node(key, value));
}

void unset_env(char *key)
{
	t_env **env_list;
	t_env *current;

	env_list = get_env_list();
	current = *env_list;
	if (ft_strcmp(current->key, key) == 0)
		*env_list = current->next;
	while (current)
	{
		if (current->next && ft_strcmp(current->next->key, key) == 0)
			current->next = current->next->next;
		current = current->next;
	}
}

char **env_to_array(void)
{
	t_env **env_list;
	t_env *current;
	char **envp;
	int size;
	int i;

	i = 0;
	env_list = get_env_list();
	size = env_listsize(*env_list);
	envp = ft_malloc((size + 1) * sizeof(char *));
	current = *env_list;
	while (current)
	{
		envp[i] = ft_strjoin_three(current->key, "=", current->value);
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char *search_path(char *cmd, char *envp[])
{
	char *tmp_slash;
	char *path_env;
	char **path;
	char *tmp;
	int i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path_env = envp[i] + 5;
	path = ft_split(path_env, ':');
	if (!path)
		spit_error(EXIT_FAILURE, "ft_split");
	i = 0;
	while (path[i])
	{
		tmp_slash = ft_strjoin(path[i], "/");
		tmp = ft_strjoin(tmp_slash, cmd);
		if (!access(tmp, X_OK))
			return (tmp);
		i++;
	}
	return (NULL);
}

int redirect(t_redir *redirects, size_t count)
{
	int fd;
	int flags;
	int mode;
	int i;

	i = 0;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	while (i < count)
	{
		if (redirects[i].type == TOKEN_REDIRECT_IN)
			flags = O_RDONLY;
		else if (redirects[i].type == TOKEN_REDIRECT_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redirects[i].type == TOKEN_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(redirects[i].file, flags, mode);
		if (fd == -1)
			return (spit_error(EXIT_FAILURE, "open"));
		if (redirects[i].type == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (spit_error(EXIT_FAILURE, "dup2"));
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (spit_error(EXIT_FAILURE, "dup2"));
		}
		if (close(fd) == -1)
			return (spit_error(EXIT_FAILURE, "close"));
		i++;
	}
	return (0);
}

int execute_command(t_ast *cmd, char *envp[])
{
	char *path;
	int status;
	pid_t pid;

	if (ft_strchr(cmd->u_data.s_cmd.argv[0], '/'))
	{
		path = ft_strdup(cmd->u_data.s_cmd.argv[0]);
		if (!path)
			return (spit_error(EXIT_FAILURE, "ft_strdup"));
	}
	else
	{
		path = search_path(cmd->u_data.s_cmd.argv[0], envp);
		if (!path)
			return (spit_error(EXIT_FAILURE, "search_path"));
	}
	pid = fork();
	if (pid < 0)
		return (spit_error(EXIT_FAILURE, "fork"));
	else if (pid == 0)
	{
		if (cmd->u_data.s_cmd.redirect_count)
			status = redirect(cmd->u_data.s_cmd.redirects, cmd->u_data.s_cmd.redirect_count);
		if (execve(path, cmd->u_data.s_cmd.argv, envp) == -1)
			return (spit_error(EXIT_FAILURE, "execve"));
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (spit_error(EXIT_FAILURE, "waitpid"));
	}
	return (status);
}

int execute_child_process(t_ast *node, int i, int prev_pipe_read, int pipe_fds[2], char *envp[])
{
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2"));
		close(prev_pipe_read);
	}
	if (i < node->u_data.s_pipeline.count - 1)
	{
		close(pipe_fds[0]);
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
			exit(spit_error(EXIT_FAILURE, "dup2"));
		close(pipe_fds[1]);
	}
	exit(execute_recursive(node->u_data.s_pipeline.commands[i], envp));
	return (EXIT_FAILURE);
}

int handle_parent_process(int i, int *prev_pipe_read, int pipe_fds[2], t_ast *node)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (i < node->u_data.s_pipeline.count - 1)
	{
		close(pipe_fds[1]);
		*prev_pipe_read = pipe_fds[0];
	}
	return (0);
}

int validate_pipeline(t_ast *node)
{
	if (!node || node->type != NODE_PIPELINE || node->u_data.s_pipeline.count < 1)
		return (spit_error(EXIT_FAILURE, "Invalid pipeline"));
	return (0);
}

int execute_pipeline(t_ast *node, char *envp[])
{
	int prev_pipe_read;
	int pipe_fds[2];
	int status;
	pid_t pid;
	int i;

	status = 0;
	prev_pipe_read = -1;
	if (validate_pipeline(node) != 0)
		return (EXIT_FAILURE);
	i = 0;
	while (i < node->u_data.s_pipeline.count)
	{
		if (i < node->u_data.s_pipeline.count - 1)
			pipe(pipe_fds);
		pid = fork();
		if (pid == 0)
			execute_child_process(node, i, prev_pipe_read, pipe_fds, envp);
		else
			handle_parent_process(i, &prev_pipe_read, pipe_fds, node);
		i++;
	}
	i = 0;
	while (i < node->u_data.s_pipeline.count)
	{
		wait(&status);
		i++;
	}
	return (status);
}

int execute_recursive(t_ast *node, char *envp[])
{
	int status;

	status = 0;
	if (node->type == NODE_COMMAND)
		status = execute_command(node, envp);
	else if (node->type == NODE_PIPELINE)
		status = execute_pipeline(node, envp);
	// else if (node->type == NODE_LOGICAL)
	// 	status = execute_logical(node, envp);
	// else if (node->type == NODE_SUBSHELL)
	// 	status = execute_subshell(node, envp);
	return (status);
}

int ft_execute(t_ast *root, char *envp[])
{
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root, envp));
}

int ft_swap_nodes(t_token *node1, t_token *node2)
{
	t_token_type type;
	char *value;

	if (!node1 || !node2)
		return (0);
	type = node1->type;
	node1->type = node2->type;
	node2->type = type;
	value = node1->value;
	node1->value = node2->value;
	node2->value = value;
	return (1);
}

char *handle_heredoc(char *delim)
{
	int(i) = 0;
	char *(input), (*result) = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delim) == 0)
			break;
		if (i)
			result = ft_strjoin(result, "\n");
		result = ft_strjoin(result, input);
		free(input);
		i = 1;
	}
	return (result);
}

t_token *ft_heredoc(t_token *tokens)
{
	int(i) = 0;
	t_token *(prv), (*temp) = tokens;
	prv = NULL;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			temp->type = TOKEN_REDIRECT_IN;
			temp->value = ft_strdup("<");
			if (!temp->next || temp->next->type != TOKEN_WORD)
				return (NULL);
			temp->next->value = handle_heredoc(temp->next->value);
			if (temp->next->value == NULL)
				temp->next->value = ft_strdup("");
			if (temp->next->next)
				if (!prv || (prv && prv->type != TOKEN_WORD))
					(ft_swap_nodes(temp->next, temp->next->next), ft_swap_nodes(temp, temp->next));
		}
		i++;
		prv = temp;
		temp = temp->next;
	}
	return (tokens);
}

t_alloc **get_alloc_list(void)
{
	static t_alloc *alloc_list;

	return (&alloc_list);
}

void free_all(void)
{
	t_alloc **alloc_list_ptr;
	t_alloc *current;
	t_alloc *next;

	alloc_list_ptr = get_alloc_list();
	current = *alloc_list_ptr;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	*alloc_list_ptr = NULL;
}

void *ft_malloc(size_t size)
{
	t_alloc **alloc_list_ptr;
	t_alloc *new_alloc;
	void *result;

	alloc_list_ptr = get_alloc_list();
	if (size == 0)
		return (NULL);
	new_alloc = malloc(sizeof(t_alloc));
	if (!new_alloc)
	{
		write(2, "Allocation failed\n", 18);
		free_all();
		exit(EXIT_FAILURE);
	}
	new_alloc->ptr = malloc(size);
	if (!new_alloc->ptr)
	{
		free(new_alloc);
		write(2, "Allocation failed\n", 18);
		free_all();
		exit(EXIT_FAILURE);
	}
	new_alloc->next = *alloc_list_ptr;
	*alloc_list_ptr = new_alloc;
	return (new_alloc->ptr);
}

bool dual_operator(const char *input, int pos)
{
	if (input[pos] == '<')
	{
		if (input[pos + 1] == '<' && ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '>')
	{
		if (input[pos + 1] == '>' && ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '|')
	{
		if (input[pos + 1] == '|' && ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '&')
	{
		if (input[pos + 1] == '&' && ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	return (true);
}

bool valid_operator(const char *input, int pos)
{
	if (input[pos] == '<' && ft_strchr(">|&", input[pos + 1]))
		return (false);
	else if (input[pos] == '>' && ft_strchr("<|&", input[pos + 1]))
		return (false);
	else if (input[pos] == '|' && ft_strchr("<>&", input[pos + 1]))
		return (false);
	else if (input[pos] == '&' && ft_strchr("<>|", input[pos + 1]))
		return (false);
	return (dual_operator(input, pos));
}

void handle_quotes(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
}

void skip_operator(const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		(*i)++;
	else if (input[*i] == '>' && input[*i + 1] == '>')
		(*i)++;
	else if (input[*i] == '|' && input[*i + 1] == '|')
		(*i)++;
	else if (input[*i] == '&' && input[*i + 1] == '&')
		(*i)++;
}

t_scan_status ft_scan(const char *input)
{
	bool in_single;
	bool in_double;
	int len;
	int i;

	in_single = false;
	in_double = false;
	len = ft_strlen(input);
	i = 0;
	while (i < len)
	{
		if (!in_single && !in_double && is_operator_char(input[i]))
		{
			if (!valid_operator(input, i))
				return (SCAN_INVALID_OPERATOR);
			skip_operator(input, &i);
		}
		else
			handle_quotes(input[i], &in_single, &in_double);
		i++;
	}
	if (in_single || in_double)
		return (SCAN_UNCLOSED_QUOTE);
	return (SCAN_SUCCESS);
}

char *translate_message(t_scan_status status)
{
	if (status == SCAN_SUCCESS)
		return ("Syntax OK");
	else if (status == SCAN_UNCLOSED_QUOTE)
		return ("Error: Unclosed quote");
	else if (status == SCAN_INVALID_OPERATOR)
		return ("Error: Invalid operator sequence");
	else
		return ("Unknown error");
}

t_token *create_token(t_token_type type, char *value)
{
	t_token *token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void add_token(t_token **head, t_token *new_token)
{
	t_token *current;

	if (!head || !new_token)
		return;
	if (!*head)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char *extract_quoted_string(const char *input, int *pos, char quote)
{
	int start;
	int end;
	char *str;

	start = *pos + 1;
	end = start;
	while (input[end] && input[end] != quote)
		end++;
	str = ft_strndup(input + start, end - start);
	if (input[end] == quote)
		*pos = end + 1;
	else
		*pos = end;
	return (str);
}

void add_operator_token(t_token **tokens, t_token_type type, const char *value, int *pos)
{
	add_token(tokens, create_token(type, ft_strdup(value)));
	if (ft_strlen(value) == 1)
		*pos += 1;
	else
		*pos += 2;
}

void handle_operator(const char *input, int *pos, t_token **tokens)
{
	if (strncmp(input + *pos, "<<", 2) == 0)
		add_operator_token(tokens, TOKEN_HEREDOC, "<<", pos);
	else if (strncmp(input + *pos, ">>", 2) == 0)
		add_operator_token(tokens, TOKEN_APPEND, ">>", pos);
	else if (strncmp(input + *pos, "&&", 2) == 0)
		add_operator_token(tokens, TOKEN_AND, "&&", pos);
	else if (strncmp(input + *pos, "||", 2) == 0)
		add_operator_token(tokens, TOKEN_OR, "||", pos);
	else if (input[*pos] == '<')
		add_operator_token(tokens, TOKEN_REDIRECT_IN, "<", pos);
	else if (input[*pos] == '>')
		add_operator_token(tokens, TOKEN_REDIRECT_OUT, ">", pos);
	else if (input[*pos] == '|')
		add_operator_token(tokens, TOKEN_PIPE, "|", pos);
	else if (input[*pos] == '(')
		add_operator_token(tokens, TOKEN_PAREN_OPEN, "(", pos);
	else if (input[*pos] == ')')
		add_operator_token(tokens, TOKEN_PAREN_CLOSE, ")", pos);
}

char *process_word_segment(const char *input, int *pos, int len)
{
	int start;
	char *segment;
	char quote;
	char *quoted_part;

	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		quote = input[*pos];
		return (extract_quoted_string(input, pos, quote));
	}
	else
	{
		start = *pos;
		while (*pos < len && !is_whitespace(input[*pos]) && !ft_strchr("<>|&()'\"", input[*pos]))
			(*pos)++;
		return (ft_strndup(input + start, *pos - start));
	}
}

t_token *ft_tokenize(const char *input)
{
	int pos;
	int len;
	char *word_buffer;
	char *segment;
	t_token *tokens;

	pos = 0;
	len = strlen(input);
	segment = NULL;
	tokens = NULL;
	while (pos < len)
	{
		while (pos < len && is_whitespace(input[pos]))
			pos++;
		if (pos >= len)
			break;
		if (ft_strchr("<>|&()", input[pos]))
		{
			handle_operator(input, &pos, &tokens);
			continue;
		}
		word_buffer = NULL;
		while (pos < len && !is_whitespace(input[pos]) && !ft_strchr("<>|&()", input[pos]))
		{
			segment = process_word_segment(input, &pos, len);
			word_buffer = ft_strjoin(word_buffer, segment);
		}
		if (word_buffer)
			add_token(&tokens, create_token(TOKEN_WORD, word_buffer));
	}
	return (tokens);
}

char *take_command(void)
{
	char *cmnd;

	cmnd = readline("\033[92mEnigma\033[94m$ \033[0m");
	add_history(cmnd);
	return (cmnd);
}

int interpret(const char *cmnd, char *envp[])
{
	t_scan_status status;
	t_token *tokens;
	t_ast *ast;
	int ret_status;

	status = ft_scan(cmnd);
	if (status != SCAN_SUCCESS)
	{
		printf("Error: %s\n", translate_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmnd);
	if (!tokens)
		return (-1);
	tokens = ft_heredoc(tokens);
	ast = ft_parse(&tokens);
	if (!ast)
		return (-2);
	ret_status = ft_execute(ast, envp);
	return (ret_status);
}

int main(int argc, char *argv[], char *envp[])
{
	char *cmnd;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmnd = take_command();
		if (!cmnd || !ft_strcmp(cmnd, "exit"))
			break;
		printf("\033[31mBASH says the above and returns:\033[0m %d\n", system(cmnd));
		interpret(cmnd, envp);
		free_all();
		free(cmnd);
	}
	return (0);
}
