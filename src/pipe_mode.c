/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 11:43:16 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/26 11:43:18 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	This function is the entire child process of pipe mode
**	1. IF we are at the first command, we are piping the inputfile into the cmd
**		=> Redirect fd_input to STDIN_FILENO (0)
**	ELSE:
**		=> Redirect the previous fd to STDIN_FILENO (0)
**	2. IF we are not at the last command of the pipeline
**		=> Redirect the WRITE_END of the pipe to STDOUT_FILENO (1)
**	ELSE we are at the end and writng into the outputfile
**		=> Redirect fd_out to STDOUT_FILENO (0)
**	3. Close all redirected AND unused fd's
**		=>	fd's get inherited/duplicated every child process
**			=>	so they need to be closed
**	4.	Execute the command with execve(COMMAND_PATH, IT'S ARGS(FLAGS), ENV)
**		=> Check if the function returns -1 => ERROR
**			=> call program_errors in that case
*/

static void	child_loop_pipe(t_pipex *p, int prev_fd, int i)
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

/*
**	MANDATORY + BONUS (multiple pipes) for piping commands
**	=> The trick is to save the previous fd in prev_fd
**	=> Then we can loop child processes until we run out of commands
**
**		WHILE (there are commands to be executed and piped)
**	1.	open the pipe we are going to use and check for pipe errors
**	2.	Fork the current process and check for fork errors
**	3.	IF (the Process ID == 0 AKA we are in the child process)
**		=> Call child_loop_pipe for what happens in the child process
**	4.	Back in the main process:
**		=> Close the previous fd (we don't need it)
**		=> Close the WRITE_END of the pipe (see above)
**		=> Save the READ_END fd of the pipe in the prev_fd variable
**			for the next iteration
**		=> Wait for the child process to finish and check if any errors occured
*/

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
			child_loop_pipe(p, prev_fd, i);
		close(prev_fd);
		close(p->pipe[WRITE_END]);
		prev_fd = p->pipe[READ_END];
		wait_error_check(p, p->pid_cmd);
	}
}
