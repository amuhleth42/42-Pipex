/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:10:21 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/20 19:21:17 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include <fcntl.h>

void	cmd_1(char **argv, int fd_r, int fd_w)
{
	char	**arg;
	int		file_fd;

	file_fd = open(argv[1], O_RDONLY);
	dup2(file_fd, 0);
	close(file_fd);
	dup2(fd_w, 1);
	close(fd_r);
	close(fd_w);
	arg = ft_split(argv[2], ' ');
	execvp(arg[0], arg);
}

void	cmd_2(char **argv, int fd_r, int fd_w)
{
	int		file_fd;
	char	**arg;

	dup2(fd_r, 0);
	close(fd_w);
	close(fd_r);
	file_fd = open(argv[4], O_RDWR | O_CREAT, 0777);
	dup2(file_fd, 1);
	close(file_fd);
	arg = ft_split(argv[3], ' ');
	wait(NULL);
	execvp(arg[0], arg);
}

int	pipex(char **argv)
{
	int	pid1;
	int	pid2;
	int	fd[2];

	if (pipe(fd) == -1)
		return (2);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		cmd_1(argv, fd[0], fd[1]);
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if (pid2 == 0)
		cmd_2(argv, fd[0], fd[1]);
	return (0);
}

int	main(int argc, char **argv)
{
	int	error;
	if (argc != 5)
	{
		ft_printf("Les arguments broo!\n");
		return (0);
	}
	error = pipex(argv);
	return (error);
}
