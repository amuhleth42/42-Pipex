/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:28:35 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/18 20:01:14 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int	main(void)
{
	int	id;
	int	fd[2];

	if (pipe(fd) == -1)
		return (4);

	id = fork();
	if (id == -1)
		return (1);
	if (id == 0)
	{
		close(fd[0]);
		int	n;
		int	i;
		int	arr[10];
		srand(time(NULL));
		n = rand() % 10 + 1;
		i = 0;
		printf("Generated: ");
		while (i < n)
		{
			arr[i] = rand() % 151;
			printf("%d ", arr[i]);
			i++;
		}
		printf("\n");
		write(fd[1], &n, sizeof(int));
		printf("Sent n = %d\n", n);
		write(fd[1], arr, sizeof(int) * n);
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int arrr[10];
		int	n, sum, j;
		read(fd[0], &n, sizeof(int));
		read(fd[0], arrr, sizeof(int) * n);
		close(fd[0]);
		sum = 0;
		j = 0;
		while (j < n)
		{
			sum += arrr[j];
			j++;
		}
		printf("Result is : %d\n", sum);
		wait(NULL);
	}
	return (0);
}
