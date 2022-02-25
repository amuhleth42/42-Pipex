/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 18:08:51 by amuhleth          #+#    #+#             */
/*   Updated: 2022/02/25 18:09:19 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *env_path, char *cmd)
{
	char	**dirs;
	int		i;
	char	*tmp;
	char	*file;

	dirs = ft_split(env_path, ':');
	i = 0;
	while (dirs[i] != NULL)
	{
		tmp = ft_strjoin(dirs[i], "/");
		file = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(file, F_OK) == 0)
			return (file);
		free(file);
		i++;
	}
	ft_putstr_fd(cmd, STDERR);
	ft_putstr_fd(" : command not found\n", STDERR);
	exit(EXIT_FAILURE);
}
