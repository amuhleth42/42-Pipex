/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:26:40 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/20 16:41:36 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	main(void)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	if (pid < 0)
		return (2);
	if (pid == 0)
	{
		char	s[200];

		close(fd[0]);
		printf("Input a string: ");
		fgets(s, 200, stdin);
		s[strlen(s) - 1] = '\0';

		write(fd[1], s, strlen(s) + 1);
		close(fd[1]);
	}
	else
	{
		char	s2[200];
		int		i;

		close(fd[1]);
		i = 0;
		while (read(fd[0], s2 + i, 1) != 0)
			i++;
		s2[i] = '\0';
		close(fd[0]);
		printf("The string is : %s\n", s2);
		wait(NULL);
	}
	return (0);
}
