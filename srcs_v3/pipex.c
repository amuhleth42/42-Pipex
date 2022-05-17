/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/17 18:27:24 by amuhleth         ###   ########.fr       */
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
		ft_putendl_fd("pipex: command not found", STDERR);
	}
	else
	{
		path = arg[0];
		if (!access(path, X_OK))
			execve(path, arg, env);
		else
			ft_putendl_fd("pipex: command not found", STDERR);
	}
	exit(127);
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

void	redirect(int in, int out)
{
	dup2(in, 0);
	dup2(out, 1);
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

void	get_infile(char **argv, t_data *a)
{
	if (a->heredoc)
		a->infile = handle_heredoc(argv[2]);
	else
		a->infile = open(argv[1], O_RDONLY);
	if (a->infile == -1)
	{
		ft_putendl_fd("error : can't open infile", STDERR);
		exit(0);
	}
}

void	get_outfile(char *argv, t_data *a)
{
	if (a->heredoc)
		a->outfile = open(argv, O_WRONLY | O_TRUNC | O_APPEND, 0664);
	else
		a->outfile = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (a->outfile == -1)
		die("error : can't open outfile");
}

void	create_pipes(t_data *a)
{
	int	i;

	a->pipes = ft_calloc(a->nb_pipes, sizeof(t_pipe));
	if (!a->pipes) // to do free clean
		exit(EXIT_FAILURE);
	i = 0;
	while (i < a->nb_pipes)
	{
		if (pipe(a->pipes[i].fd) < 0)
			i += 0;
			// free main
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	a;
	int		i;
	int		status;

	ft_bzero(&a, sizeof(t_data));
	handle_input_error(argc, argv, &a);
	get_infile(argv, &a);
	get_outfile(argv[argc - 1], &a);
	a.nb_pipes = argc - 3 - a.heredoc - 1;
	a.nb_cmd = argc - 3 - a.heredoc;
	create_pipes(&a);
	i = 0;
	while (i < a.nb_cmd)
	{
		exec_and_redirect(argv[i + 2 + a.heredoc], env, &a, i);
		i++;
	}
	close_pipes(&a);
	wait(&status);
	//return (WEXITSTATUS(status));
	return (0);
}
