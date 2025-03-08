/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:37:22 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/08 18:23:18 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
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
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
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

void	handle_operator(const char *input, int *pos, t_token **tokens)
{
	if (strncmp(input + *pos, "<<", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_HEREDOC, ft_strdup("<<")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, ">>", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_APPEND, ft_strdup(">>")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, "&&", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_AND, ft_strdup("&&")));
		*pos += 2;
	}
	else if (strncmp(input + *pos, "||", 2) == 0)
	{
		add_token(tokens, create_token(TOKEN_OR, ft_strdup("||")));
		*pos += 2;
	}
	else if (input[*pos] == '<')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_IN, ft_strdup("<")));
		(*pos)++;
	}
	else if (input[*pos] == '>')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_OUT, ft_strdup(">")));
		(*pos)++;
	}
	else if (input[*pos] == '|')
	{
		add_token(tokens, create_token(TOKEN_PIPE, ft_strdup("|")));
		(*pos)++;
	}
	else if (input[*pos] == '(')
	{
		add_token(tokens, create_token(TOKEN_PAREN_OPEN, ft_strdup("(")));
		(*pos)++;
	}
	else if (input[*pos] == ')')
	{
		add_token(tokens, create_token(TOKEN_PAREN_CLOSE, ft_strdup(")")));
		(*pos)++;
	}
}

/* 
** Determine if we should start or continue building a word
** A word starts or continues if the current character is not whitespace, 
** not an operator, and not the beginning of a quoted string that stands alone.
*/
bool	should_build_word(const char *input, int pos, int len)
{
	if (pos >= len)
		return (false);
	if (is_whitespace(input[pos]))
		return (false);
	if (ft_strchr("<>|&()", input[pos]))
		return (false);
	return (true);
}

/* 
** Check if a quoted string is standalone or part of a word
** A quoted string is standalone if it's not adjacent to any non-operator, 
** non-whitespace characters before or after.
*/
bool	is_standalone_quoted(const char *input, int pos, int len)
{
	int	prev_pos;
	int	next_pos;
	char	quote;

	quote = input[pos];
	
	// Check what's before the quote
	prev_pos = pos - 1;
	while (prev_pos >= 0 && is_whitespace(input[prev_pos]))
		prev_pos--;
	
	// Check what's after the quote
	next_pos = pos + 1;
	while (next_pos < len && input[next_pos] != quote)
		next_pos++;
	
	if (next_pos < len) //Found closing quote
		next_pos++;
	
	// Skip whitespace after the closing quote
	while (next_pos < len && is_whitespace(input[next_pos]))
		next_pos++;
	
	// Standalone if surrounded by whitespace or operators or at string boundaries
	return ((prev_pos < 0 || ft_strchr("<>|&()", input[prev_pos])) &&
			(next_pos >= len || ft_strchr("<>|&()", input[next_pos]) || 
			 is_whitespace(input[next_pos])));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (s1)
		result = ft_strjoin(s1, s2);
	else
		result = ft_strjoin("", s2);
	return (result);
}

t_token	*ft_tokenize(const char *input)
{
	t_token	*tokens;
	int		pos;
	int		len;
	char	*word_buffer;
	char	*quoted_part;

	tokens = NULL;
	pos = 0;
	len = strlen(input);
	
	while (pos < len)
	{
		// Skip whitespace
		while (pos < len && is_whitespace(input[pos]))
			pos++;
		
		if (pos >= len)
			break;
		
		// Handle operators
		if (ft_strchr("<>|&()", input[pos]))
		{
			handle_operator(input, &pos, &tokens);
			continue;
		}
		
		// Handle quoted strings (standalone or as part of a word)
		if ((input[pos] == '\'' || input[pos] == '"') && 
			is_standalone_quoted(input, pos, len))
		{
			char quote = input[pos];
			quoted_part = extract_quoted_string(input, &pos, quote);
			if (quote == '\'')
				add_token(&tokens, create_token(TOKEN_SINGLE_QUOTED, quoted_part));
			else
				add_token(&tokens, create_token(TOKEN_DOUBLE_QUOTED, quoted_part));
			continue;
		}
		
		// Handle words (including those with embedded quotes)
		word_buffer = NULL;
		while (pos < len && !is_whitespace(input[pos]) && 
			   !ft_strchr("<>|&()", input[pos]))
		{
			if (input[pos] == '\'' || input[pos] == '"')
			{
				// Extract the quoted part and append to word buffer
				char quote = input[pos];
				quoted_part = extract_quoted_string(input, &pos, quote);
				word_buffer = ft_strjoin_free(word_buffer, quoted_part);
			}
			else
			{
				// Extract unquoted segment
				int start = pos;
				while (pos < len && !is_whitespace(input[pos]) && 
					   !ft_strchr("<>|&()'\"", input[pos]))
					pos++;
				
				char *unquoted = ft_strndup(input + start, pos - start);
				word_buffer = ft_strjoin_free(word_buffer, unquoted);
			}
		}
		
		if (word_buffer)
			add_token(&tokens, create_token(TOKEN_WORD, word_buffer));
	}
	
	return (tokens);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

char	*translate_type(int type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_SINGLE_QUOTED)
		return ("SINGLE_QUOTED");
	else if (type == TOKEN_DOUBLE_QUOTED)
		return ("DOUBLE_QUOTED");
	else if (type == TOKEN_REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_PAREN_OPEN)
		return ("LPAREN");
	else if (type == TOKEN_PAREN_CLOSE)
		return ("RPAREN");
	else
		return ("UNKNOWN");
}

// test the tokenizer
int main()
{
	t_token	*tokens;
	t_token	*current;
	char	*inputs[] = {
		"echo 'hello world' >> file.txt",    // Basic command with redirection
		"ls -l | grep '.c' && echo Done",    // Pipe and logical AND
		"cat <<EOF",                         // Heredoc operator
		"echo \"nested 'quotes' test\"",     // Mixed quotes
		"echo 'unterminated",                // Unterminated single quote
		"echo \"unterminated",               // Unterminated double quote
		"cmd >file1 <file2",                 // Input and output redirection
		"echo $HOME",                        // Variable expansion (not handled yet)
		"(echo hello)&&(echo world)",        // Logical AND without spaces and parentheses
		"echo hello||echo world",            // Logical OR without spaces
		"echo \"\"",                         // Empty double quotes
		"echo ''",                           // Empty single quotes
		"\"ls\"\"-la\"",
		"\"\"l\"\"s -l\"\"a",
		NULL
	};

	for (int i = 0; inputs[i]; i++)
	{
		printf("\nTokenizing the string: %s\n", inputs[i]);
		tokens = ft_tokenize(inputs[i]);
		current = tokens;
		while (current)
		{
			printf("Token type: %s, value: %s\n", translate_type(current->type), current->value);
			current = current->next;
		}
	}
	free_all();
	return (0);
}
