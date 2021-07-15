/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 15:15:10 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/15 10:57:30 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	init_data(t_pipex *p, char **argv)
{
	p->commands = NULL;
	p->env_paths = NULL;
	p->n_cmds = 0;
	p->fd_input = -1;
	p->fd_out = -1;
	p->pipe[0] = -1;
	p->pipe[1] = -1;
	p->pid_cmd = -1;
	if (ft_strequal(argv[1], "here_doc"))
		p->mode = HERE_DOC;
	else
		p->mode = PIPE;
	p->limiter = NULL;
}

/*
**	Open the pipe for the processes
*/

void	open_pipe(t_pipex *p, int fd[2])
{
	if (pipe(fd) == ERROR)
		program_errors(p, "OPENING PIPE", TRUE);
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
	close(p->fd_input);
	close(p->fd_out);
	close_pipe(p->pipe);
	if (p->limiter)
		ft_strdel(&p->limiter);
}

void	close_pipe(int *pipe)
{
	close (pipe[READ_END]);
	close (pipe[WRITE_END]);
	pipe[READ_END] = -1;
	pipe[WRITE_END] = -1;
}
