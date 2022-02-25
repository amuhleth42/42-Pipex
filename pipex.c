/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/25 17:01:01 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *env_path, char *cmd)
{
	char	**dirs;
	int		i;
	char	*tmp;
	char	*file;

	dirs = ft_split(env_path, ':');
	i = 0;
	while (dirs[i] != NULL)
	{
		tmp = ft_strjoin(dirs[i], "/");
		file = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file, F_OK) == 0)
			return (file);
		free(file);
		i++;
	}
	ft_putstr_fd(cmd, STDERR);
	ft_putstr_fd(" : command not found\n", STDERR);
	exit(EXIT_FAILURE);
}

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

int	main(int argc, char **argv, char **env)
{
	int		fd_in;
	int		fd_out;
	int		status;
	int		i;

	handle_input_error(argc, argv);
	fd_in = open(argv[1], O_RDONLY);
	fd_out = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd_in == -1 || fd_out == -1)
		die("open");
	i = 2;
	while (i < argc - 2)
	{
		exec_and_redirect(argv[i], env, &fd_in);
		i++;
	}
	exec_last(argv[i], env, fd_in, fd_out);
	wait(&status);
	return (status);
}
