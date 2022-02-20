/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 17:09:26 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/18 17:31:05 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int	fd[2];
	int	id;

	if (pipe(fd) == -1)
	{
		printf("Error\n");
		return (1);
	}
	id = fork();
	if (id == 0)
	{
		int	x;
		close(fd[0]);
		printf("Input a number: ");
		scanf("%d", &x);
		write(fd[1], &x, sizeof(int));
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int y;
		read(fd[0], &y, sizeof(int));
		close(fd[0]);
		printf("Got from child process %d.\n", y);
	}
	return (0);
}
