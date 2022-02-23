/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 15:16:19 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/23 15:38:57 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	int	pid1;
	int	pid2;
	int	fd[3][2];
	int	i;
	
	i = 0;
	while (i < 3)
	{
		if (pipe(fd[i]) < 0)
			return (1);
		i++;
	}
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
	{
		int	x;

		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		read(fd[0][0], &x, sizeof(int));
		x += 5;
		write(fd[1][1], &x, sizeof(int));
		close(fd[0][0]);
		close(fd[1][1]);
		return (0);
	}
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if (pid2 == 0)
	{
		int	y;

		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);
		read(fd[1][0], &y, sizeof(int));
		y += 5;
		write(fd[2][1], &y, sizeof(int));
		close(fd[1][0]);
		close(fd[2][1]);
		return (0);
	}
	int	x;

	printf("Input a number: ");
	scanf("%d", &x);
	close(fd[0][0]);
	close(fd[1][1]);
	close(fd[1][0]);
	close(fd[2][1]);
	write(fd[0][1], &x, sizeof(int));
	read(fd[2][0], &x, sizeof(int));
	printf("Result is %d\n", x);
	close(fd[0][1]);
	close(fd[2][0]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
