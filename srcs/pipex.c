/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/04/13 15:48:16 by amuhleth         ###   ########.fr       */
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

void	exec_and_redirect(char *cmd, char **env, t_data *a)
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
		dup2(a->infile, STDIN);
		close(a->infile);
		close(fd[0]);
		dup2(fd[1], STDOUT);
		close(fd[1]);
		exec_cmd(cmd, env);
	}
	else
	{
		close(a->infile);
		a->infile = fd[0];
		close(fd[1]);
		wait(NULL);
	}
}

void	exec_last(char *cmd, char **env, t_data *a)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		die("fork");
	else if (pid == 0)
	{
		dup2(a->infile, STDIN);
		close(a->infile);
		dup2(a->outfile, STDOUT);
		close(a->outfile);
		exec_cmd(cmd, env);
	}
	else
	{
		close(a->infile);
		close(a->outfile);
	}
}

void	get_infile(char **argv, int *i, t_data *a)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		a->infile = handle_heredoc(argv[2]);
		*i = 3;
	}
	else
	{
		a->infile = open(argv[1], O_RDONLY);
		*i = 2;
	}
	if (a->infile == -1)
		die("open");
}

void	get_outfile(char *argv, t_data *a)
{
	a->outfile = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (a->outfile == -1)
		die("open");
}

int	main(int argc, char **argv, char **env)
{
	t_data	a;
	int		i;
	int		status;

	handle_input_error(argc, argv);
	get_infile(argv, &i, &a);
	get_outfile(argv[argc - 1], &a);
	if (a.infile == -1 || a.outfile == -1)
		die("open");
	while (i < argc - 2)
	{
		exec_and_redirect(argv[i], env, &a);
		i++;
	}
	exec_last(argv[i], env, &a);
	wait(&status);
	return (status);
}
