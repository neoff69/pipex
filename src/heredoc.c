/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgonnot <vgonnot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 15:31:40 by vgonnot           #+#    #+#             */
/*   Updated: 2023/02/09 07:53:46 by vgonnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	heredoc(t_env *st, char **argv)
{
	char	*get_str;

	if (ft_strcmp(argv[1], "here_doc") != 0)
		return (0);
	st->nbr_cmd -= 1;
	while (1)
	{
		get_str = get_next_line(0);
		if (get_str == NULL)
		{
			ft_printf("End of file : the delimiter was %s\n", argv[2]);
			break ;
		}
		else if (ft_strcmp(argv[2], get_str) == 0)
		{
			free(get_str);
			break ;
		}
		write(st->infile, get_str, ft_strlen(get_str));
		free(get_str);
	}
	close(st->infile);
	st->infile = open(argv[1], O_RDWR);
	return (1);
}
