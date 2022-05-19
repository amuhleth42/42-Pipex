/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:00:16 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/19 15:34:33 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	die(char *s)
{
	ft_putstr_fd("pipex: ", STDERR);
	perror(s);
	exit(EXIT_FAILURE);
}

void	quit(t_data *a, char *s)
{
	if (a->pipes)
	{
		free(a->pipes);
		a->pipes = NULL;
	}
	die(s);
}

void	free_split(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

void	handle_input_error(int argc, char **argv, t_data *a)
{
	if (argc < 2)
		die("Invalid number of argument");
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		a->heredoc = 1;
	if (argc < 5 + a->heredoc)
		die("Invalid number of argument");
	if ((access(argv[1], F_OK) == -1) && !a->heredoc)
		die("access");
}
