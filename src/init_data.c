/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 15:15:10 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 12:24:47 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	init_data(t_pipex *p)
{
	p->commands = NULL;
	p->env_paths = NULL;
	p->fd_input = -1;
	p->fd_out = -1;
	p->pipe[0] = -1;
	p->pipe[1] = -1;
	p->pid = -1;
}

void	clear_data(t_pipex *p)
{
	int	i;

	if (p->commands)
	{
		i = -1;
		while (p->commands[++i])
			ft_str_array_del(&p->commands[i]);
	}
	else if (p->env_paths)
		ft_str_array_del(&p->env_paths);
	else if (p->fd_input != -1)
		close(p->fd_input);
	else if (p->fd_out != -1)
		close(p->fd_out);
	else if (p->pipe[0] != -1)
		close (p->pipe[0]);
	else if (p->pipe[1] != -1)
		close (p->pipe[1]);
}