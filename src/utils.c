/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgonnot <vgonnot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:39:48 by vgonnot           #+#    #+#             */
/*   Updated: 2023/02/10 08:15:19 by vgonnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

void	free_pipe(t_env *st)
{
	int	i;

	i = 0;
	while (i < st->nbr_cmd - 1 && st->fd && st->fd[i])
	{
		free(st->fd[i]);
		i++;
	}
	if (st->pid != NULL)
		free(st->pid);
	if (st->fd != NULL)
		free(st->fd);
	if (st != NULL)
		free(st);
}

void	free_env_exit(t_env *st, int i)
{
	if (i >= 0)
	{
		while (i >= 0)
		{
			free(st->fd[i]);
			i--;
		}
		free(st->pid);
		free(st->fd);
	}
	free(st);
	exit(EXIT_FAILURE);
}

void	set_up_struct(t_env *st, int argc, char **argv)
{
	int	i;

	i = -1;
	st->nbr_cmd = argc - 3;
	st->hdoc = heredoc(st, argv);
	if (st->hdoc == 1 && argc < 6)
	{
		ft_printf("test");
		exit(ft_printf("Error : not enough arguments\n"));
	}
	st->actual_pipe = 0;
	st->pid = malloc(sizeof(int) * (st->nbr_cmd + 1));
	if (st->pid == NULL)
		free_env_exit(st, i);
	st->fd = malloc(sizeof(int *) * (st->nbr_cmd));
	if (st->fd == NULL)
	{
		free(st->pid);
		free_env_exit(st, i);
	}
	while (++i < st->nbr_cmd - 1)
	{
		st->fd[i] = malloc(sizeof(int) * 2);
		if (st->fd[i] == NULL || pipe(st->fd[i]) == -1)
			free_env_exit(st, i);
	}
}

void	quit_function(t_env *st, int error_code)
{
	close_function(st);
	free_pipe(st);
	if (error_code == 0)
		exit(ft_printf("Error : %s\n", strerror(errno)));
	else if (error_code == 1)
		exit(ft_printf("Error : Fork\n"));
	exit(EXIT_FAILURE);
}

void	error_execve(char **arg_vec, char *path, t_env *st)
{
	ft_free_2d_array(arg_vec);
	free(path);
	quit_function(st, -1);
}
