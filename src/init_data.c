/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 15:15:10 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 15:50:54 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	init_data(t_input *i)
{
	i->commands = NULL;
	i->env_paths = NULL;
	i->fd_input = -1;
	i->fd_out = -1;
	i->pipes = NULL;
}

void	clear_data(t_input *i)
{
	if (i->commands)
		ft_str_array_del(&i->commands);
	else if (i->env_paths)
		ft_str_array_del(&i->env_paths);
	else if (i->fd_input != -1)
		close(i->fd_input);
	else if (i->fd_out != -1)
		close(i->fd_out);
}