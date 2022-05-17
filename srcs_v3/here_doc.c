/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:24:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/25 17:51:21 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	read_til_limiter(char *limiter, int p_write, int p_read)
{
	char	*line;

	limiter = ft_strjoin(limiter, "\n");
	close(p_read);
	line = get_next_line(STDIN);
	while (line != NULL)
	{
		if (ft_strncmp(limiter, line, ft_strlen(limiter) + 1) == 0)
		{
			close(p_write);
			free(line);
			free(limiter);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, p_write);
		free(line);
		line = get_next_line(STDIN);
	}
}

int	handle_heredoc(char *limiter)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		die("pipe");
	pid = fork();
	if (pid < 0)
		die("fork");
	if (pid == 0)
		read_til_limiter(limiter, fd[1], fd[0]);
	else
	{
		close(fd[1]);
		wait(NULL);
	}
	return (fd[0]);
}
