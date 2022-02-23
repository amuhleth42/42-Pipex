/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:10:21 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/23 16:37:29 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	execvp(arg[0], arg);
}

int	pipex(char **argv)
{
	int	pid1;
	int	pid2;
	int	fd[2];
	int	wstatus;

	pipe(fd);
	pid1 = fork();
	if (pid1 == 0)
		cmd_1(argv, fd[0], fd[1]);
	pid2 = fork();
	if (pid2 == 0)
		cmd_2(argv, fd[0], fd[1]);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &wstatus, 0);
	return (wstatus);
}

int	main(int argc, char **argv)
{
	int	wstatus;
	if (argc < 5)
	{
		write(STDERR, "Invalid number of arguments\n", 28);
		return (EXIT_FAILURE);
	}
	wstatus = pipex(argv);
	return (wstatus);
}
