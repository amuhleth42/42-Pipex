/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:22:20 by amuhleth          #+#    #+#             */
/*   Updated: 2022/04/13 15:33:38 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# include "libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include <errno.h>

typedef struct s_data
{
	int	infile;
	int	outfile;
}		t_data;

void	die(char *s);
void	handle_input_error(int argc, char **argv);

char	*get_path(char *env_path, char *cmd);

int		handle_heredoc(char *limiter);
void	read_til_limiter(char *limiter, int p_write, int p_read);

#endif
