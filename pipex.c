/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/23 22:33:17 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *env_path, char *cmd)
{
	char	**dirs;
	int		i;
	char	*tmp;
	char	*file;

	(void) cmd;
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
	return (NULL);
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
		//strerror
		exit(1);
	}
	path = get_path(env[i] + 5, arg[0]);
	execve(path, arg, env);
	//die
}

void	exec_and_redirect(char *cmd, char **env)
{
	int	pid;
	int	fd[2];
	int	wstatus;

	if (pipe(fd) == -1)
		write(STDERR, "error\n", 6);
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

void	exec_last(char *cmd, char **env, int *wstatus)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		write(STDERR, "error\n", 6);
		// die, fork
	else if (pid == 0)
	{
		exec_cmd(cmd, env);
		// die, exec
	}
	else
		wait(wstatus);
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
	io[1] = open(argv[argc - 1], O_WRONLY | O_CREAT, 0777);
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
	close(io[1]);
	exec_last(argv[i], env, &wstatus);
	return (wstatus);
}
