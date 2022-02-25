/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:00:16 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/25 17:00:39 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	die(char *s)
{
	ft_putstr_fd("pipex: ", STDERR);
	perror(s);
	exit(EXIT_FAILURE);
}

void	handle_input_error(int argc, char **argv)
{
	if (argc < 5)
	{
		write(STDERR, "Invalid arguments\n", 18);
		exit(EXIT_FAILURE);
	}
	if (access(argv[1], F_OK) == -1)
		die("pipex");
}
