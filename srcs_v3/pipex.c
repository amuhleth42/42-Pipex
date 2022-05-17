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
	/*else
	{
		close(a->infile);
		a->infile = fd[0];
		close(fd[1]);
		wait(NULL);
	}*/
}

void	get_infile(char **argv, int *i, t_data *a)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		a->infile = handle_heredoc(argv[2]);
		a->heredoc = 1;
	}
	else
		a->infile = open(argv[1], O_RDONLY);
	*i = 2 + a->heredoc;
	if (a->infile == -1)
		die("open");
}

void	get_outfile(char *argv, t_data *a)
{
	a->outfile = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (a->outfile == -1)
		die("open");
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
	handle_input_error(argc, argv);
	get_infile(argv, &i, &a);
	get_outfile(argv[argc - 1], &a);
	a.nb_pipes = argc - 3 - a.heredoc - 1;
	a.nb_cmd = argc - 3 - a.heredoc;
	create_pipes(&a);
	while (i < argc - 1)
	{
		exec_and_redirect(argv[i], env, &a, i - 2 - a.heredoc);
		i++;
	}
	wait(&status);
	return (status);
}

/*int	main(int argc, char **argv, char **env)
{
	t_data	a;
	int		i;
	int		status;

	handle_input_error(argc, argv);
	get_infile(argv, &i, &a);
	get_outfile(argv[argc - 1], &a);
	while (i < argc - 2)
	{
		exec_and_redirect(argv[i], env, &a);
		i++;
	}
	exec_last(argv[i], env, &a);
	wait(&status);
	return (status);
}*/
