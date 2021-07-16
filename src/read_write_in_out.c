/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_in_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 11:29:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/16 13:38:02 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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

static void	check_retval(t_pipex *p, int retval, char **line, char **readstr)
{
	if (retval == ERROR || retval == 0)
	{
		if (*line)
			ft_strdel(line);
		if (*readstr)
			ft_strdel(readstr);
		if (retval == ERROR)
			program_errors(p, "INPUT READING", TRUE);
		else
			program_errors(p, "EOF BEFORE LIMITER", TRUE);
	}
	else if (retval == 1)
	{
		*readstr = ft_strjoin(*readstr, *line);
		*readstr = ft_strjoin_char(*readstr, '\n');
		ft_strdel(line);
	}
}

static void	open_pipe_unlink(t_pipex *p, char *inputstream)
{
	p->fd_input = open(inputstream, O_RDONLY);
	if (p->fd_input == ERROR)
		usage_error(p, "OPENING INPUTSTREAM", TRUE);
	pipe_mode(p);
	if (unlink(inputstream) == ERROR)
		usage_error(p, "UNLINKING INPUTSTREAM", TRUE);
}

void	here_doc_mode(t_pipex *p)
{
	char	*line;
	char	*readstr;
	int		size;
	int		retval;

	line = NULL;
	readstr = NULL;
	retval = get_next_line(STDIN_FILENO, &line);
	check_retval(p, retval, &line, &readstr);
	while (retval)
	{
		retval = get_next_line(STDIN_FILENO, &line);
		if (ft_strequal(line, p->limiter))
		{
			if (*line)
				ft_strdel(&line);
			break ;
		}
		check_retval(p, retval, &line, &readstr);
	}
	size = ft_strlen(readstr);
	write(p->fd_input, readstr, size);
	close(p->fd_input);
	ft_strdel(&readstr);
	open_pipe_unlink(p, "inputstream.txt");
}
