/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:40:18 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/19 17:31:04 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	get_infile(char **argv, t_data *a)
{
	if (a->heredoc)
		a->infile = handle_heredoc(argv[2]);
	else
		a->infile = open(argv[1], O_RDONLY);
	if (a->infile == -1)
	{
		ft_putendl_fd("error : can't open infile", STDERR);
		exit(1);
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
	if (!a->pipes)
		die("malloc failed");
	i = 0;
	while (i < a->nb_pipes)
	{
		if (pipe(a->pipes[i].fd) < 0)
			quit(a, "error : can't create pipe");
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	a;
	int		i;
	int		wstatus;

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
	wait(&wstatus);
	if (WIFEXITED(wstatus))
		a.status = WEXITSTATUS(wstatus);
	return (a.status);
}
