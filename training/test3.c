/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:09:39 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/18 19:24:10 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	int pid;
	char	*arg[] = {
		"/sbin/ping",
		"-c",
		"3",
		"google.com",
		NULL
	};
	pid = fork();
	if (pid == -1) { return (1); };
	if (pid == 0)
	{
		printf("Start of execution\n");
		execve("/sbin/ping", arg, NULL);
	}
	else
	{
		wait(NULL);
		printf("You've done it bro :)\n");
	}
	return (0);
}
