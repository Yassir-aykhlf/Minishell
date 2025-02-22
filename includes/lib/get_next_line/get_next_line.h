/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:34:54 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/01/30 20:06:32 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif

# ifndef FD_MAX
#  define FD_MAX 8192
# endif

# include "../libft/libft.h"
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <unistd.h>

char	*allocate_line(char **line, char *buffer);
int		append_update(char **line, char *buffer);
int		read_next_line(int fd, char **line);
int		get_next_line(int fd, char **line);

#endif
