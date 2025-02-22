/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaykhlf <yaykhlf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:34:46 by yaykhlf           #+#    #+#             */
/*   Updated: 2025/01/29 14:46:14 by yaykhlf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*allocate_line(char **line, char *buffer)
{
	char	*memory;
	int		line_len;
	int		buffer_len;

	line_len = 0;
	while (*line && (*line)[line_len] && (*line)[line_len] != '\n')
		line_len++;
	buffer_len = 0;
	while (buffer[buffer_len] && buffer[buffer_len] != '\n')
		buffer_len++;
	memory = (char *)malloc(sizeof(char) * (buffer_len + line_len + 1));
	if (!memory)
		return (NULL);
	return (memory);
}

int	append_update(char **line, char *buffer)
{
	char	*tmp_line;
	int		line_index;
	int		buff_index;

	tmp_line = allocate_line(line, buffer);
	if (!tmp_line)
		return (-1);
	line_index = 0;
	buff_index = 0;
	while (*line && (*line)[line_index] && (*line)[line_index] != '\n')
		tmp_line[line_index++] = (*line)[buff_index++];
	buff_index = 0;
	while (buffer[buff_index] && buffer[buff_index] != '\n')
		tmp_line[line_index++] = buffer[buff_index++];
	tmp_line[line_index] = buffer[buff_index];
	free(*line);
	*line = tmp_line;
	line_index = 0;
	while (buffer[buff_index])
		buffer[line_index++] = buffer[++buff_index];
	buffer[line_index] = '\0';
	line_index = 0;
	while ((*line)[line_index] && (*line)[line_index] != '\n')
		line_index++;
	return (line_index);
}

int	read_next_line(int fd, char **line)
{
	static char		buffer[FD_MAX][BUFFER_SIZE + 1];
	int				bytes_read;

	*line = NULL;
	bytes_read = append_update(line, buffer[fd]);
	while (bytes_read != -1 && (*line)[bytes_read] != '\n')
	{
		bytes_read = read(fd, buffer[fd], BUFFER_SIZE);
		if (bytes_read < 1)
		{
			if (bytes_read < 0)
			{
				free(*line);
				*line = NULL;
			}
			return (bytes_read);
		}
		buffer[fd][bytes_read] = '\0';
		bytes_read = append_update(line, buffer[fd]);
	}
	if (bytes_read == -1)
		return (-1);
	(*line)[bytes_read] = '\0';
	return (1);
}

int	get_next_line(int fd, char **line)
{
	if (fd < 0 || fd > FD_MAX || !line || BUFFER_SIZE < 1)
		return (-1);
	return (read_next_line(fd, line));
}
