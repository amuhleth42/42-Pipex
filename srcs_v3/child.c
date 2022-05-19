/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 15:46:55 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/19 15:49:14 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd, char **env)
{
	char	*path;
	char	**arg;

	arg = ft_split(cmd, ' ');
	if (!ft_strchr(arg[0], '/'))
	{
		path = get_path(env, arg[0]);
		execve(path, arg, env);
		if (path)
			free(path);
	}
	else
	{
		path = arg[0];
		execve(path, arg, env);
	}
	free_split(arg);
	ft_putendl_fd("pipex: command not found", STDERR);
	exit(127);
}

void	redirect(int in, int out)
{
	dup2(in, STDIN);
	dup2(out, STDOUT);
}

void	close_pipes(t_data *a)
{
	int	i;

	i = 0;
	while (i < a->nb_pipes)
	{
		close(a->pipes[i].fd[0]);
		close(a->pipes[i].fd[1]);
		i++;
	}
}

void	exec_and_redirect(char *cmd, char **env, t_data *a, int i)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		die("fork");
	else if (pid == 0)
	{
		if (i == 0)
			redirect(a->infile, a->pipes[i].fd[1]);
		else if (i == a->nb_cmd - 1)
			redirect(a->pipes[i - 1].fd[0], a->outfile);
		else
			redirect(a->pipes[i - 1].fd[0], a->pipes[i].fd[1]);
		close_pipes(a);
		exec_cmd(cmd, env);
	}
}
