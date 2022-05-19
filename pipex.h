/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:22:20 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/19 16:20:44 by amuhleth         ###   ########.fr       */
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

typedef struct s_pipe
{
	int	fd[2];
}		t_pipe;

typedef struct s_data
{
	int		infile;
	int		outfile;
	int		heredoc;
	int		nb_pipes;
	int		nb_cmd;
	int		status;
	t_pipe	*pipes;
}		t_data;

void	die(char *s);
void	quit(t_data *a, char *s);
void	free_split(char **strs);
void	handle_input_error(int argc, char **argv, t_data *a);

char	*get_path(char **env, char *cmd);

void	exec_and_redirect(char *cmd, char **env, t_data *a, int i);
void	close_pipes(t_data *a);

int		handle_heredoc(char *limiter);
void	read_til_limiter(char *limiter, int p_write, int p_read);

#endif
