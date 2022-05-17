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

void	redirection(t_data *a)
{
	if (a->i == 0)
	{
		dup2(a->infile, STDIN);
		dup2(a->fd[a->i][1], STDOUT);
	}
	else if (a->i == a->nb_exec - 1)
	{
		dup2(a->fd[a->i - 1][0], STDIN);
		dup2(a->outfile, STDOUT);
	}
	else
	{
		dup2(a->fd[a->i - 1][0], STDIN);
		dup2(a->fd[a->i][1], STDOUT);
	}
}

void	close_pipes(t_data *a)
{
	int	i;

	i = 0;
	while (i < a->nb_exec - 1)
	{
		close(a->fd[i][0]);
		close(a->fd[i][1]);
		i++;
	}
}

void	child(char *cmd, char **env, t_data a)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		die("fork");
	else if (pid == 0)
	{
		redirection(&a);
		close_pipes(&a);
		exec_cmd(cmd, env);
	}
}

void	get_infile(char **argv, t_data *a)
{
	if (a->heredoc)
		a->infile = handle_heredoc(argv[2]);
	else
		a->infile = open(argv[1], O_RDONLY);
	if (a->infile == -1)
		die("error : cannot open infile");
}

void	get_outfile(char *argv, t_data *a)
{
	a->outfile = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (a->outfile == -1)
		die("error : can't open outfile");
}

void	create_pipes(t_data *a)
{
	int	i;

	a->fd = ft_calloc(a->nb_exec - 1, sizeof(int *));
	if (!a->fd)
		die("error : malloc failed");
	i = 0;
	while (i < a->nb_exec - 1)
	{
		a->fd[i] = ft_calloc(2, sizeof(int));
		if (pipe(&a->fd[i][0]) < 0)
			die("error : pipe");
			//error, free
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	a;
	//int		status;

	ft_bzero(&a, sizeof(a));
	handle_input_error(argc, argv, &a);
	get_infile(argv, &a);
	get_outfile(argv[argc - 1], &a);
	a.nb_exec = argc - 3 - a.heredoc;
	create_pipes(&a);
	while (a.i < a.nb_exec)
	{
		child(argv[2 + a.heredoc + a.i], env, a);
		a.i++;
	}
	close_pipes(&a);
	wait(&a.status);
	return (a.status);
}
