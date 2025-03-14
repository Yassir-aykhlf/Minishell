/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print (buggy).c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arajma <arajma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:53:25 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/03/14 21:18:28 by arajma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	dest_len;

	i = 0;
	dest_len = ft_strlen(dest);
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

char	*ft_realloc(char *ptr, size_t size)
{
	char	*new_ptr;

	new_ptr = ft_malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

void	print_indent(int level, int is_last)
{
	static int	levels[128];
	int			i;

	if (level > 0)
		levels[level - 1] = !is_last;
	i = 0;
	while (i < level - 1)
	{
		if (levels[i])
			ft_putstr_fd("│   ", STDOUT_FILENO);
		else
			ft_putstr_fd("    ", STDOUT_FILENO);
		i++;
	}
	if (level > 0)
	{
		if (is_last)
			ft_putstr_fd("└── ", STDOUT_FILENO);
		else
			ft_putstr_fd("├── ", STDOUT_FILENO);
	}
}

void	print_command_node(t_ast *node, int level, int is_last)
{
	int		i;
	char	*joined_cmd;
	size_t	total_len;
	size_t	current_len;

	print_indent(level, is_last);
	if (!node->u_data.s_cmd.argv || !node->u_data.s_cmd.argv[0])
	{
		ft_putstr_fd("CMD: (empty command)\n", STDOUT_FILENO);
		return ;
	}
	
	total_len = 5;
	i = 0;
	while (node->u_data.s_cmd.argv[i])
	{
		total_len += ft_strlen(node->u_data.s_cmd.argv[i]) + 1; // +1 for space
		i++;
	}
	
	joined_cmd = ft_malloc(total_len);
	if (!joined_cmd)
		return ;
	
	ft_strcpy(joined_cmd, "CMD: ");
	current_len = 5;
	i = 0;
	while (node->u_data.s_cmd.argv[i])
	{
		ft_strcpy(joined_cmd + current_len, node->u_data.s_cmd.argv[i]);
		current_len += ft_strlen(node->u_data.s_cmd.argv[i]);
		if (node->u_data.s_cmd.argv[i + 1])
		{
			joined_cmd[current_len] = ' ';
			current_len++;
		}
		i++;
	}
	joined_cmd[current_len] = '\0';
	
	i = 0;
	while (i < node->u_data.s_cmd.redirect_count)
	{
		joined_cmd = ft_realloc(joined_cmd, current_len + 3 + 
			ft_strlen(node->u_data.s_cmd.redirects[i].file));
		if (!joined_cmd)
			return ;
			
		if (node->u_data.s_cmd.redirects[i].type == TOKEN_REDIRECT_IN)
			ft_strcat(joined_cmd, " < ");
		else if (node->u_data.s_cmd.redirects[i].type == TOKEN_REDIRECT_OUT)
			ft_strcat(joined_cmd, " > ");
		else if (node->u_data.s_cmd.redirects[i].type == TOKEN_APPEND)
			ft_strcat(joined_cmd, " >> ");
		else if (node->u_data.s_cmd.redirects[i].type == TOKEN_HEREDOC)
			ft_strcat(joined_cmd, " << ");
			
		ft_strcat(joined_cmd, node->u_data.s_cmd.redirects[i].file);
		i++;
	}
	
	ft_putstr_fd(joined_cmd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(joined_cmd);
}

void	print_pipeline_node(t_ast *node, int level, int is_last)
{
	int	i;

	print_indent(level, is_last);
	ft_putstr_fd("NODE_PIPELINE\n", STDOUT_FILENO);
	
	for (i = 0; i < node->u_data.s_pipeline.count; i++)
	{
		print_ast_recursive(node->u_data.s_pipeline.commands[i], 
			level + 1, i == node->u_data.s_pipeline.count - 1);
	}
}

void	print_logical_node(t_ast *node, int level, int is_last)
{
	print_indent(level, is_last);
	if (node->u_data.s_op.operat == LOGICAL_AND)
		ft_putstr_fd("NODE_LOGICAL (&&)\n", STDOUT_FILENO);
	else
		ft_putstr_fd("NODE_LOGICAL (||)\n", STDOUT_FILENO);
	
	print_ast_recursive(node->u_data.s_op.left, level + 1, 0);
	print_ast_recursive(node->u_data.s_op.right, level + 1, 1);
}

void	print_subshell_node(t_ast *node, int level, int is_last)
{
	print_indent(level, is_last);
	ft_putstr_fd("NODE_SUBSHELL\n", STDOUT_FILENO);
	print_ast_recursive(node->u_data.s_subshell.command, level + 1, 1);
}

void	print_ast_recursive(t_ast *node, int level, int is_last)
{
	if (!node)
		return ;
	
	// printf("level %d:\n", level);
	if (node->type == NODE_COMMAND)
		print_command_node(node, level, is_last);
	else if (node->type == NODE_PIPELINE)
		print_pipeline_node(node, level, is_last);
	else if (node->type == NODE_LOGICAL)
		print_logical_node(node, level, is_last);
	else if (node->type == NODE_SUBSHELL)
		print_subshell_node(node, level, is_last);
	// sleep(1);
}

void	print_ast(t_ast *root)
{
	if (!root)
	{
		ft_putstr_fd("Empty AST\n", STDOUT_FILENO);
		return ;
	}
	print_ast_recursive(root, 0, 1);
}
