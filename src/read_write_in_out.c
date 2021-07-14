/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_in_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 11:29:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/14 14:05:35 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	child_loop(t_pipex *p, int prev_fd, int i)
{
	if (prev_fd != STDIN_FILENO && i != 0)
		dup2(prev_fd, STDIN_FILENO);
	else
		dup2(p->fd_input, STDIN_FILENO);
	if (i != p->n_cmds - 1)
		dup2(p->pipe[WRITE_END], STDOUT_FILENO);
	else
		dup2(p->fd_out, STDOUT_FILENO);
	close_pipe(p->pipe);
	close(p->fd_input);
	close(p->fd_out);
	if (execve(p->commands[i][0], p->commands[i], p->envp) == ERROR)
		program_errors(p, "EXECUTION ERROR", TRUE);
}

void	pipe_mode(t_pipex *p)
{
	int	i;
	int	prev_fd;

	i = -1;
	prev_fd = p->fd_input;
	while (++i < p->n_cmds)
	{
		open_pipe(p, p->pipe);
		p->pid_cmd = fork();
		if (p->pid_cmd == ERROR)
			program_errors(p, "FORKING", TRUE);
		if (p->pid_cmd == CHILD_PROCESS)
			child_loop(p, prev_fd, i);
		close(prev_fd);
		close(p->pipe[WRITE_END]);
		prev_fd = p->pipe[READ_END];
		wait_error_check(p, p->pid_cmd);
	}
}
