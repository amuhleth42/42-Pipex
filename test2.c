/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 17:31:11 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/18 17:46:24 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(void)
{
	int	arr[] = {1, 412, 3, 4, 1, 2, 7, 18, 27, 1, 99};
	int	fd[2];
	int	id;

	int	start;
	int	end;
	int	arr_size;

	arr_size = sizeof(arr) / sizeof(int);
	if (pipe(fd) == -1)
		return (1);
	id = fork();
	if (id == -1)
		return (2);
	if (id == 0)
	{
		start = 0;
		end = arr_size / 2;
	}
	else
	{
		start = arr_size / 2;
		end = arr_size;
	}
	
	int	sum = 0;
	int i;
	i = start;
	while (i < end)
	{
		sum += arr[i];
		i++;
	}
	printf("Calculated partial sum : %d\n", sum);

	if (id == 0)
	{
		close(fd[0]);
		write(fd[1], &sum, sizeof(int));
		close(fd[1]);
	}
	else
	{
		int child_sum;
		close(fd[1]);
		read(fd[0], &child_sum, sizeof(int));
		close(fd[0]);

		int total = sum + child_sum;
		printf("Total sum is %d\n", total);
		wait(NULL);
	}
	return (0);
}
