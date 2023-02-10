/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgonnot <vgonnot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:49:51 by vgonnot           #+#    #+#             */
/*   Updated: 2023/02/10 07:56:04 by vgonnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

static void	open_files(int argc, char **argv, t_env *st)
{
	int	hdoc;

	hdoc = ft_strcmp(argv[1], "here_doc");
	if (hdoc == 0)
		st->infile = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	else
		st->infile = open(argv[1], O_RDWR);
	if (st->infile == -1)
	{
		ft_printf("Cannot open file : %s\n", argv[1]);
		free(st);
		exit(EXIT_FAILURE);
	}
	if (hdoc == 0)
		st->outfile = open(argv[argc -1], O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		st->outfile = open(argv[argc -1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (st->outfile == -1)
		quit_function(st, 0);
}

void	close_function(t_env *st)
{
	int	i;

	i = 0;
	while (i < st->nbr_cmd - 1)
	{
		close(st->fd[i][0]);
		close(st->fd[i][1]);
		i++;
	}
}

static void	get_dup_done(t_env *st, char **argv, char *env[])
{
	if (st->actual_pipe == 0)
	{
		dup2(st->infile, STDIN_FILENO);
		dup2(st->fd[st->actual_pipe][1], STDOUT_FILENO);
		close_function(st);
		close(st->infile);
	}
	else if (st->actual_pipe == st->nbr_cmd - 1)
	{
		dup2(st->fd[st->actual_pipe - 1][0], STDIN_FILENO);
		dup2(st->outfile, STDOUT_FILENO);
		close_function(st);
		close(st->outfile);
	}
	else
	{
		dup2(st->fd[st->actual_pipe - 1][0], STDIN_FILENO);
		dup2(st->fd[st->actual_pipe][1], STDOUT_FILENO);
		close_function(st);
	}
	get_exec_done(argv, env, st);
}

static void	fork_manager(char **argv, char *env[], t_env *st, int i)
{	
	st->pid[i] = fork();
	if (st->pid[i] == -1)
		free_env_exit(st, st->nbr_cmd - 1);
	if (st->pid[i] < 0)
		quit_function(st, 1);
	if (st->pid[i] == 0)
		get_dup_done(st, argv, env);
}

int	main(int argc, char **argv, char *env[])
{
	t_env	*st;
	int		i;

	i = 0;
	if (argc < 5)
		return (ft_printf("Error : not enough arguments\n"));
	st = malloc(sizeof(t_env));
	if (st == NULL)
		return (ft_printf("Error : %s\n", strerror(errno)));
	open_files(argc, argv, st);
	set_up_struct(st, argc, argv);
	while (i < st->nbr_cmd)
	{
		fork_manager(argv, env, st, i);
		st->actual_pipe += 1;
		i++;
	}
	close(st->infile);
	close(st->outfile);
	close_function(st);
	i = -1;
	while (++i < st->nbr_cmd)
		waitpid(st->pid[i], NULL, 0);
	return (free_pipe(st), 0);
}
