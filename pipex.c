/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/23 17:38:17 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd, char **env)
{
	char	**arg;

	(void) env;
	arg = ft_split(cmd, ' ');
	execvp(arg[0], arg);
	//die
}

void	exec_and_redirect(char *cmd, char **env)
{
	int	pid;
	int	fd[2];
	int	wstatus;

	if (pipe(fd) == -1)
		// handle error, die, pipe
	pid = fork();
	if (pid < 0)
		// die, fork
		exit(1);
	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT);
		close(fd[1]);
		exec_cmd(cmd, env);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
		close(fd[0]);
		wait(&wstatus);
		if (wstatus)
			exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **env)
{
	int	wstatus;
	int	io[2];
	int	i;
	
	if (argc < 5)
	{
		write(STDERR, "Invalid arguments\n", 18);
		return (EXIT_FAILURE);
	}
	if (access(argv[1], F_OK) == -1)
	{
		write(STDERR, "No such file of directory\n", 26);
		return (EXIT_FAILURE);
	}
	io[0] = open(argv[1], O_RDONLY);
	io[1] = open(argv[argc - 2], O_WRONLY | O_CREAT, 0777);
	if (io[0] == -1 || io[1] == -1)
		return (EXIT_FAILURE);
	dup2(io[0], STDIN);
	close(io[0]);
	i = 1;
	while (i < argc - 2)
	{
		exec_and_redirect(argv[i], env);
		i++;
	}
	dup2(io[1], STDOUT);
	wstatus = exec_last(argv[i]);
	return (wstatus);
}
