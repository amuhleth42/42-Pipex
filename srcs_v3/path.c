/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amuhleth <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 18:08:51 by amuhleth          #+#    #+#             */
/*   Updated: 2022/05/17 18:08:29 by amuhleth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env_path(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		die("pipex: path not found");
	return (env[i] + 5);
}

char	*get_path(char **env, char *cmd)
{
	char	**dirs;
	int		i;
	char	*tmp;
	char	*file;
	char	*env_path;

	env_path = get_env_path(env);
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
	return (NULL);
}
