/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/25 19:20:07 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd, char **env)
{
	char	*path;
	char	**arg;
	int		i;

	arg = ft_split(cmd, ' ');
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
	{
		ft_putstr_fd("pipex : Path not found\n", STDERR);
		exit(EXIT_FAILURE);
	}
	path = get_path(env[i] + 5, arg[0]);
	execve(path, arg, env);
	die("exec");
}

void	exec_and_redirect(char *cmd, char **env, int *fd_in)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		die("pipe");
	pid = fork();
	if (pid < 0)
		die("fork");
	else if (pid == 0)
	{
		dup2(*fd_in, STDIN);
		close(*fd_in);
		close(fd[0]);
		dup2(fd[1], STDOUT);
		close(fd[1]);
		exec_cmd(cmd, env);
	}
	else
	{
		close(*fd_in);
		*fd_in = fd[0];
		close(fd[1]);
		wait(NULL);
	}
}

void	exec_last(char *cmd, char **env, int fd_in, int fd_out)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		die("fork");
	else if (pid == 0)
	{
		dup2(fd_in, STDIN);
		close(fd_in);
		dup2(fd_out, STDOUT);
		close(fd_out);
		exec_cmd(cmd, env);
	}
	else
	{
		close(fd_in);
		close(fd_out);
	}
}

void	get_infile(char **argv, int *i, int *fd_in)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		*fd_in = handle_heredoc(argv[2]);
		*i = 3;
	}
	else
	{
		*fd_in = open(argv[1], O_RDONLY);
		*i = 2;
	}
}

int	main(int argc, char **argv, char **env)
{
	int		fd_in;
	int		fd_out;
	int		status;
	int		i;

	handle_input_error(argc, argv);
	get_infile(argv, &i, &fd_in);
	fd_out = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd_in == -1 || fd_out == -1)
		die("open");
	while (i < argc - 2)
	{
		exec_and_redirect(argv[i], env, &fd_in);
		i++;
	}
	exec_last(argv[i], env, fd_in, fd_out);
	wait(&status);
	return (status);
}
