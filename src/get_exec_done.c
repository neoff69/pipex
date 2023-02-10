/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exec_done.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgonnot <vgonnot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:31:08 by vgonnot           #+#    #+#             */
/*   Updated: 2023/02/10 08:02:21 by vgonnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

void	set_path(char **path_split, char *cmd)
{
	int		i;
	char	*temp;

	i = 1;
	temp = ft_strjoin(&path_split[0][5], "/");
	free(path_split[0]);
	path_split[0] = ft_strjoin(temp, cmd);
	free(temp);
	while (path_split[i])
	{
		temp = ft_strjoin(path_split[i], "/");
		free(path_split[i]);
		path_split[i] = ft_strjoin(temp, cmd);
		free(temp);
		i++;
	}
}

char	*get_path(char *cmd, char *paths)
{
	char	**path_split;
	int		i;
	int		ac;
	char	*temp;

	i = 0;
	ac = -1;
	temp = NULL;
	if (access(cmd, X_OK) != -1)
		return (ft_strjoin(temp, cmd));
	path_split = ft_split(paths, ':');
	if (path_split == NULL)
		return (NULL);
	set_path(path_split, cmd);
	while (ac == -1 && path_split[i])
		ac = access(path_split[i++], X_OK);
	if (ac != -1)
	{
		temp = ft_strjoin(temp, path_split[i -1]);
		ft_free_2d_array(path_split);
		return (temp);
	}
	ft_free_2d_array(path_split);
	return (NULL);
}

void	get_exec_done(char **argv, char **env, t_env *st)
{
	char	**arg_vec;
	char	*path;
	int		i;
	int		check;

	check = 1;
	i = -1;
	arg_vec = ft_split(argv[st->actual_pipe + 2 + st->hdoc], ' ');
	while (env[++i])
	{
		if (ft_strstart(env[i], "PATH="))
		{
			check = 0;
			break ;
		}
	}
	if (check)
		no_path(st, arg_vec);
	if (env[i])
		path = get_path(arg_vec[0], env[i]);
	if (path == NULL)
		path_is_null(st, arg_vec, argv);
	execve(path, arg_vec, env);
	error_execve(arg_vec, path, st);
}
