# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <string.h>

# define EMPTY_AST 1

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_allocation
{
	void				*ptr;
	struct s_allocation	*next;
}	t_allocation;


typedef enum e_scan_status
{
	SCAN_SUCCESS,
	SCAN_UNCLOSED_QUOTE,
	SCAN_INVALID_OPERATOR
}	t_scan_status;

typedef enum e_logical_op
{
	LOGICAL_AND,
	LOGICAL_OR
}	t_logical_op;

typedef enum e_ast_type
{
	NODE_LOGICAL,
	NODE_PIPELINE,
	NODE_COMMAND,
	NODE_SUBSHELL
}	t_ast_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
}	t_redir;

typedef struct	s_ast t_ast;

typedef struct	s_ast
{
	t_ast_type type;
	union
	{
		struct
		{
			t_logical_op	operat;
			t_ast			*left;
			t_ast			*right;
		}		op;
		struct
		{
			t_ast	**commands;
			int		count;
		}		pipeline;
		struct
		{
			char	**argv;
			t_redir	*redirects;
			size_t	redirect_count;
		}		cmd;
		struct
		{
			t_ast	*command;
		}		subshell;
	}	data;
}	t_ast;

int				ft_toupper(int c);
int				ft_tolower(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isalnum(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
int				ft_atoi(const char *nptr);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_itoa(int n);
char			*ft_strdup(const char *s);
char			*ft_strchr(const char *s, int c);
char			**ft_split(const char *s, char c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(const char *s1, const char *set);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_substr(const char *s, unsigned int start, size_t len);
char			*ft_strnstr(const char *haystack, const char *needle, size_t len);
void			ft_putnbr_fd(int n, int fd);
void			ft_bzero(void *s, size_t n);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			*ft_memset(void	*s, int c, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
int				ft_strlen(const char *s);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
size_t			ft_strlcat(char	*dest, const char *src,	size_t size);
t_ast			*parse_subshell(t_token **tokens);
t_ast			*parse_command(t_token **tokens);
t_ast			*parse_pipeline(t_token **tokens);
t_ast			*parse_logical_expr(t_token **tokens);
t_ast			*ft_parse(t_token **tokens);
t_ast			*parse_factor(t_token **tokens);
t_ast			*create_logical_node(t_logical_op op, t_ast *left, t_ast *right);
t_ast			*create_pipeline_node(void);
t_ast			*create_command_node(void);
t_ast			*create_subshell_node(void);
void			add_redirect(t_ast *cmd_node, t_token_type type, char *file);
void			add_argument(t_ast *cmd_node, char *arg);
void			add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node);
int				is_redirect(t_token *token);
int				is_cmd_finished(t_token *token);
void			print_ast_recursive(t_ast *node, int level, int is_last);
char			*ft_strcat(char *dest, const char *src);
char			*ft_realloc(char *ptr, size_t size);
char			*ft_strcpy(char *dst, const char *src);
void			print_indent(int level, int is_last);
void			print_command_node(t_ast *node, int level, int is_last);
void			print_pipeline_node(t_ast *node, int level, int is_last);
void			print_logical_node(t_ast *node, int level, int is_last);
void			print_subshell_node(t_ast *node, int level, int is_last);
void			print_ast(t_ast *root);
char			*take_command(void);
char			*search_path(char *cmd, char *envp[]);
int				process_command(const char *cmnd, char *envp[]);
t_token			*here_doc(t_token *tokens);
char			*handle_heredoc(char *delim);
t_ast			*ft_parse(t_token **tokens);
char			*take_command(void);
char			*search_path(char *cmd, char *envp[]);
int				process_command(const char *cmnd, char *envp[]);
t_token			*here_doc(t_token *tokens);
char			*handle_heredoc(char *delim);
t_ast			*ft_parse(t_token **tokens);
void			handle_quotes(char c, bool *in_single, bool *in_double);
bool			valid_operator(const char *input, int pos);
bool			dual_operator(const char *input, int pos);
const char		*translate_message(t_scan_status status);
void			skip_operator(const char *input, int *i);
t_scan_status	ft_scanner(const char *input);
bool			is_operator_char(char c);
t_allocation	**get_alloc_list(void);
void			free_all(void);
void			*ft_malloc(size_t size);
bool			is_whitespace(char c);
int				ft_strcmp(const char *s1, const char *s2);
bool			is_operator_char(char c);
char			*ft_strndup(const char *s, size_t n);
int				err_exit(int status, char *message);
int				ft_execute(t_ast *root, char *envp[]);
int				execute_command(t_ast *cmd, char *envp[]);
int				execute_pipeline(t_ast *node, char *envp[]);
int				execute_recursive(t_ast *node, char *envp[]);
int				redirect(t_redir *redirects, size_t count);
char			*search_path(char *cmd, char *envp[]);
int				ft_swap_nodes(t_token *node1, t_token *node2);
char			*handle_heredoc(char *delim);
t_token			*here_doc(t_token *tokens);

t_allocation	**get_alloc_list(void)
{
	static t_allocation	*alloc_list;

	return (&alloc_list);
}

void	free_all(void)
{
	t_allocation	**alloc_list_ptr;
	t_allocation	*current;
	t_allocation	*next;

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

void	*ft_malloc(size_t size)
{
	t_allocation	**alloc_list_ptr;
	t_allocation	*new_alloc;
	void			*result;

	alloc_list_ptr = get_alloc_list();
	if (size == 0)
		return (NULL);
	new_alloc = malloc(sizeof(t_allocation));
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

int	err_exit(int status, char *message)
{
	if (message)
		perror(message);
	return (status);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	ft_strcmp(const char *s1, const char *s2)
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

bool	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

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

bool	dual_operator(const char *input, int pos)
{
	if (input[pos] == '<')
	{
		if (input[pos + 1] == '<'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '>')
	{
		if (input[pos + 1] == '>'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '|')
	{
		if (input[pos + 1] == '|'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	else if (input[pos] == '&')
	{
		if (input[pos + 1] == '&'
			&& ft_strchr("<>|&", input[pos + 2]))
			return (false);
		return (true);
	}
	return (true);
}

bool	valid_operator(const char *input, int pos)
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

void	handle_quotes(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
}

void	skip_operator(const char *input, int *i)
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

/**
 * @brief Scans the input string for syntax errors.
 * 
 * This function scans the input string to check for syntax errors such as
 * unclosed quotes or invalid operator sequences.
 * 
 * @param input The input string to be scanned.
 * @return t_scan_status The status of the scan, which can be one of the following:
 * - SCAN_SUCCESS: The input string has no syntax errors.
 * - SCAN_UNCLOSED_QUOTE: There is an unclosed quote in the input string.
 * - SCAN_INVALID_OPERATOR: There is an invalid operator sequence in the input string.
 */
t_scan_status	ft_scanner(const char *input)
{
	bool	in_single;
	bool	in_double;
	int		len;
	int		i;

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

const char	*translate_message(t_scan_status status)
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

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*extract_quoted_string(const char *input, int *pos, char quote)
{
	int		start;
	int		end;
	char	*str;

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

void	add_operator_token(t_token **tokens, t_token_type type
						, const char *value, int *pos)
{
	add_token(tokens, create_token(type, ft_strdup(value)));
	if (ft_strlen(value) == 1)
		*pos += 1;
	else
		*pos += 2;
}

void	handle_operator(const char *input, int *pos, t_token **tokens)
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

char	*process_word_segment(const char *input, int *pos, int len)
{
	int		start;
	char	*segment;
	char	quote;
	char	*quoted_part;

	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		quote = input[*pos];
		return (extract_quoted_string(input, pos, quote));
	}
	else
	{
		start = *pos;
		while (*pos < len && !is_whitespace(input[*pos])
			&& !ft_strchr("<>|&()'\"", input[*pos]))
			(*pos)++;
		return (ft_strndup(input + start, *pos - start));
	}
}

t_token	*ft_tokenize(const char *input)
{
	int		pos;
	int		len;
	char	*word_buffer;
	char	*segment;
	t_token	*tokens;

	pos = 0;
	len = strlen(input);
	segment = NULL;
	tokens = NULL;
	while (pos < len)
	{
		while (pos < len && is_whitespace(input[pos]))
			pos++;
		if (pos >= len)
			break ;
		if (ft_strchr("<>|&()", input[pos]))
		{
			handle_operator(input, &pos, &tokens);
			continue;
		}
		word_buffer = NULL;
		while (pos < len && !is_whitespace(input[pos])
			&& !ft_strchr("<>|&()", input[pos]))
		{
			segment = process_word_segment(input, &pos, len);
			word_buffer = ft_strjoin(word_buffer, segment);
		}
		if (word_buffer)
			add_token(&tokens, create_token(TOKEN_WORD, word_buffer));
	}
	return (tokens);
}

// utility to swap nodes returns 0 in error cases
int	ft_swap_nodes(t_token *node1, t_token *node2)
{
	t_token_type	type;
	char			*value;

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

// handeling heredoc utility to change to get multiline string from STDIN
char	*handle_heredoc(char *delim)
{
	int (i) = 0;
	char *(input), (*result) = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delim) == 0)
			break ;
		if (i)
			result = ft_strjoin(result, "\n");
		result = ft_strjoin(result, input);
		free(input);
		i = 1;
	}
	return (result);
}

// handeling heredoc if it exists
t_token	*here_doc(t_token *tokens)
{
	int (i) = 0;
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
					(ft_swap_nodes(temp->next, temp->next->next)
						, ft_swap_nodes(temp, temp->next));
		}
		i++;
		prv = temp;
		temp = temp->next;
	}
	return (tokens);
}

/* Helper function to create a new subshell node */
t_ast	*create_subshell_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_SUBSHELL;
	node->data.subshell.command = NULL;
	return (node);
}

/* Helper function to create a new command node */
t_ast	*create_command_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->data.cmd.argv = NULL;
	node->data.cmd.redirects = NULL;
	node->data.cmd.redirect_count = 0;
	return (node);
}

/* Helper function to create a new pipeline node */
t_ast	*create_pipeline_node(void)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_PIPELINE;
	node->data.pipeline.commands = NULL;
	node->data.pipeline.count = 0;
	return (node);
}

/* Helper function to create a new logical node */
t_ast	*create_logical_node(t_logical_op op, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast));
	node->type = NODE_LOGICAL;
	node->data.op.operat = op;
	node->data.op.left = left;
	node->data.op.right = right;
	return (node);
}

/* Function to handle subshells */
t_ast	*parse_factor(t_token **tokens)
{
	if (*tokens && (*tokens)->type == TOKEN_PAREN_OPEN)
		return (parse_subshell(tokens));
	else
		return (parse_command(tokens));
}

/* Helper function to add a redirect to a command node */
void	add_redirect(t_ast *cmd_node, t_token_type type, char *file)
{
	size_t	new_size;
	t_redir	*new_redirects;

	new_size = cmd_node->data.cmd.redirect_count + 1;
	new_redirects = ft_malloc(sizeof(t_redir) * new_size);
	ft_memmove(new_redirects, cmd_node->data.cmd.redirects,
		sizeof(t_redir) * (new_size - 1));
	cmd_node->data.cmd.redirects = new_redirects;
	cmd_node->data.cmd.redirects[cmd_node->data.cmd.redirect_count].type = type;
	cmd_node->data.cmd.redirects[cmd_node
		->data.cmd.redirect_count].file = ft_strdup(file);
	cmd_node->data.cmd.redirect_count++;
}

/* Helper function to add an argument to a command node */
void	add_argument(t_ast *cmd_node, char *arg)
{
	char	**new_args;
	size_t	arg_count;

	arg_count = 0;
	if (cmd_node->data.cmd.argv)
		while (cmd_node->data.cmd.argv[arg_count])
			arg_count++;
	new_args = ft_malloc(sizeof(char *) * (arg_count + 2));
	ft_memmove(new_args, cmd_node->data.cmd.argv, sizeof(char *) * arg_count);
	cmd_node->data.cmd.argv = new_args;
	cmd_node->data.cmd.argv[arg_count] = ft_strdup(arg);
	cmd_node->data.cmd.argv[arg_count + 1] = NULL;
}

/* Helper function to add a command to a pipeline node */
void	add_command_to_pipeline(t_ast *pipeline_node, t_ast *cmd_node)
{
	size_t	new_size;
	t_ast	**new_commands;

	new_size = pipeline_node->data.pipeline.count + 1;
	new_commands = ft_malloc(sizeof(t_ast *) * new_size);
	ft_memmove(new_commands, pipeline_node->data
		.pipeline.commands, sizeof(t_ast *) * (new_size - 1));
	pipeline_node->data.pipeline.commands = new_commands;
	pipeline_node->data.pipeline.commands[pipeline_node
		->data.pipeline.count] = cmd_node;
	pipeline_node->data.pipeline.count++;
}

/* C'mon it is just here for normes */
int	is_redirect(t_token *token)
{
	if (token->type == TOKEN_REDIRECT_IN
		|| token->type == TOKEN_REDIRECT_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/* C'mon it is just here for normes */
int	is_cmd_finished(t_token *token)
{
	if (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR)
		return (1);
	return (0);
}

/* Function to parse a subshell */
t_ast	*parse_subshell(t_token **tokens)
{
	t_ast	*subshell_node;
	t_ast	*command_node;

	*tokens = (*tokens)->next;
	command_node = parse_logical_expr(tokens);
	if (!*tokens || !((*tokens)->type == TOKEN_PAREN_CLOSE))
		return (NULL);
	*tokens = (*tokens)->next;
	subshell_node = create_subshell_node();
	subshell_node->data.subshell.command = command_node;
	return (subshell_node);
}

/* Function to parse a command */
t_ast	*parse_command(t_token **tokens)
{
	t_token_type	redirect_type;
	t_token			*current;
	t_ast			*cmd_node;

	cmd_node = create_command_node();
	while (*tokens)
	{
		current = *tokens;
		if (is_cmd_finished(current))
			break ;
		if (is_redirect(current))
		{
			redirect_type = current->type;
			*tokens = current->next;
			if (!*tokens || ((*tokens)->type != TOKEN_WORD))
				return (NULL);
			add_redirect(cmd_node, redirect_type, (*tokens)->value);
			*tokens = (*tokens)->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			add_argument(cmd_node, current->value);
			*tokens = current->next;
			continue ;
		}
		return (NULL);
	}
	if (!cmd_node->data.cmd.argv || !cmd_node->data.cmd.argv[0])
		return (NULL);
	return (cmd_node);
}

/* Function to parse a pipeline */
t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*pipeline_node;
	t_ast	*cmd_node;

	pipeline_node = create_pipeline_node();
	cmd_node = parse_factor(tokens);
	add_command_to_pipeline(pipeline_node, cmd_node);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd_node = parse_factor(tokens);
		add_command_to_pipeline(pipeline_node, cmd_node);
	}
	if (pipeline_node->data.pipeline.count == 1)
	{
		cmd_node = pipeline_node->data.pipeline.commands[0];
		return (cmd_node);
	}
	return (pipeline_node);
}

/* Function to parse logical expressions */
t_ast	*parse_logical_expr(t_token **tokens)
{
	t_logical_op	op;
	t_ast			*result;
	t_ast			*right;
	t_ast			*left;

	left = parse_pipeline(tokens);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		op = (*tokens)->type - 6;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		result = create_logical_node(op, left, right);
		left = result;
	}
	return (left);
}

/* Main parsing function */
t_ast	*ft_parse(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_expr(tokens));
}

char	*search_path(char *cmd, char *envp[])
{
	char	*tmp_slash;
	char	*path_env;
	char	**path;
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path_env = envp[i] + 5;
	path = ft_split(path_env, ':');
	if (!path)
		err_exit(EXIT_FAILURE, "ft_split");
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

int	redirect(t_redir *redirects, size_t count)
{
	int	fd;
	int	flags;
	int	mode;
	int	i;

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
			return (err_exit(EXIT_FAILURE, "open"));
		if (redirects[i].type == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (err_exit(EXIT_FAILURE, "dup2"));
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (err_exit(EXIT_FAILURE, "dup2"));
		}
		if (close(fd) == -1)
			return (err_exit(EXIT_FAILURE, "close"));
		i++;
	}
	return (0);
}

int	execute_command(t_ast *cmd, char *envp[])
{
	char	*path;
	int		status;
	pid_t	pid;
	
	if (ft_strchr(cmd->data.cmd.argv[0], '/'))
	{
		path = ft_strdup(cmd->data.cmd.argv[0]);
		if (!path)
			return (err_exit(EXIT_FAILURE, "ft_strdup"));
	}
	else
	{
		path = search_path(cmd->data.cmd.argv[0], envp);
		if (!path)
			return (err_exit(EXIT_FAILURE, "search_path"));
	}
	pid = fork();
	if (pid < 0)
		return (err_exit(EXIT_FAILURE, "fork"));
	else if (pid == 0)
	{
		if (cmd->data.cmd.redirect_count)
			redirect(cmd->data.cmd.redirects, cmd->data.cmd.redirect_count);
		if (execve(path, cmd->data.cmd.argv, envp) == -1)
			return (err_exit(EXIT_FAILURE, "execve"));
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (err_exit(EXIT_FAILURE, "waitpid"));
	}
	return (status);
}

int	execute_pipeline(t_ast *node, char *envp[])
{
	printf("ain't done yet !\n");
	return(1);
}

int	execute_recursive(t_ast *node, char *envp[])
{
	int	status;

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

int	ft_execute(t_ast *root, char *envp[])
{
	if (!root)
		return (EMPTY_AST);
	return (execute_recursive(root, envp));
}

char	*take_command(void)
{
	char	*cmnd;

	cmnd = readline("\033[92mEnigma\033[94m$ \033[0m");
	add_history(cmnd);
	return (cmnd);
}

/**
 * @brief Handles the tokenization, parsing, and execution of a command.
 *
 * This function tokenizes the input command, parses the tokens into an
 * abstract syntax tree (AST), and then executes the AST. It handles
 * memory allocation and deallocation for tokens and the AST.
 *
 * @param cmnd The command to be executed.
 * @param envp The environment variables to be passed to the new process.
 * @return The status code of the executed command. Returns -1 if an error occurs
 * during tokenization, -2 if an error occurs during parsing.
 */
int	process_command(const char *cmnd, char *envp[])
{
	t_scan_status	status;
	t_token			*tokens;
	t_ast			*ast;
	int				ret_status;
	
	status = ft_scanner(cmnd);
	if (status != SCAN_SUCCESS)
	{
		printf("Error: %s\n", translate_message(status));
		return (-1);
	}
	tokens = ft_tokenize(cmnd);
 	if (!tokens)
		return (-1);
	tokens = here_doc(tokens);
	ast = ft_parse(&tokens);
	if (!ast)
		return (-2);
	ret_status = ft_execute(ast, envp);
	return (ret_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmnd;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmnd = take_command();
		if (!cmnd || !ft_strcmp(cmnd, "exit"))
			break ;
		process_command(cmnd, envp);
		free_all();
	}
	return (0);
}
