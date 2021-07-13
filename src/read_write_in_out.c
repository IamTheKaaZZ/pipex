/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_in_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 11:29:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/13 16:07:26 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	child_loop(t_pipex *p, int prev_fd, int i)
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
