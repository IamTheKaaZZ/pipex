/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 15:15:10 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/20 12:11:01 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	Initialize the variables of the pipex struct
**	=> Check for the mode in argv[1]
*/

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
**	Open a pipe for the child processes
**	=>	Return an error if pipe(fd) == -1
*/

void	open_pipe(t_pipex *p, int fd[2])
{
	if (pipe(fd) == ERROR)
		program_errors(p, "OPENING PIPE", TRUE);
}

/*
**	Clear all data and allocated memory if it was not free'd before
**	=> Free allocated memory
**	=> Close file descriptors if they are still open
*/

void	clear_data(t_pipex *p)
{
	int	i;

	if (p->commands)
	{
		i = -1;
		while (p->commands[++i])
			ft_str_array_del(&p->commands[i]);
		free(p->commands);
		p->commands = NULL;
	}
	if (p->env_paths)
		ft_str_array_del(&p->env_paths);
	close(p->fd_input);
	close(p->fd_out);
	close_pipe(p->pipe);
	if (p->limiter)
		ft_strdel(&p->limiter);
	if (access("inputstream.txt", X_OK) != -1)
	{
		if (unlink("inputstream.txt") == ERROR)
			usage_error(p, "UNLINKING INPUTSTREAM", TRUE);
	}
}

/*
**	Close a given pipe from both ends
**	=> Set them back to -1 for later use
*/

void	close_pipe(int *pipe)
{
	close(pipe[READ_END]);
	close(pipe[WRITE_END]);
	pipe[READ_END] = -1;
	pipe[WRITE_END] = -1;
}
