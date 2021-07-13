/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_in_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 11:29:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/13 14:15:41 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	Read from the input file in a loop
**	ERROR CHECK
**		- Check if read gives an error
**	IF reading has been succesful
**		RETURN the read string
*/

char	*read_from_fd(t_pipex *p, int fd)
{
	int		ret_read;
	char	buff;
	char	*str;

	str = NULL;
	ret_read = read(fd, &buff, 1);
	if (ret_read == ERROR)
		program_errors(p, "READING FROM FD", TRUE);
	while (ret_read)
	{
		str = ft_strjoin_char(str, buff);
		ret_read = read(fd, &buff, 1);
		if (ret_read == ERROR)
		{
			if (str)
				ft_strdel(&str);
			program_errors(p, "READING FROM FD", TRUE);
		}
	}
	printf("The string:\n%s has been read\n", str);
	return (str);
}

/*
**	Write the string read from the input file to the first command
**	1.	REDIRECT the input string to STDIN
**	2.	CLOSE fd_input
**	3.	REDIRECT the WRITE_END of the pipe to STDOUT
**	4.	CLOSE the pipe
**	5.	CLOSE fd_out (gets inherited through fork)
*/

void	write_input_to_cmd(t_pipex *p)
{
	dup2(p->fd_input, STDIN_FILENO);
	close(p->fd_input);
	dup2(p->pipe1[WRITE_END], STDOUT_FILENO);
	close_pipe(p->pipe1);
	close(p->fd_out);
	if (execve(p->commands[0][0], p->commands[0], p->envp) == ERROR)
		program_errors(p, "EXECUTION ERROR", TRUE);
}

/*
**	Write the STDOUT from the second command to the output file
**	1.	REDIRECT the READ_END of the pipe to STDIN
**	2.	CLOSE the pipe
**	3.	REDIRECT the output file to STDOUT
**	4.	CLOSE fd_out
**	5.	CLOSE fd_input (gets inherited through fork)
**	ALWAYS executes the LAST command in the list -> argc - 4
*/

void	write_cmd_to_output(t_pipex *p, int readfd)
{
	int	n;

	n = p->n_cmds - 1;
	dup2(readfd, STDIN_FILENO);
	printf("readfd = %i\n", readfd);
	close(readfd);
	dup2(p->fd_out, STDOUT_FILENO);
	close(p->fd_out);
	close(p->fd_input);
	if (execve(p->commands[n][0], p->commands[n], p->envp) == ERROR)
		program_errors(p, "EXECUTION ERROR", TRUE);
}

/*
**	Send the STDOUT from the command to pipe2
**	1.	REDIRECT the READ_END of pipe1 to STDIN
**	2.	CLOSE the pipe
**	3.	REDIRECT the WRITE_END of pipe2 to STDOUT
**	4.	CLOSE the pipe
**	5.	CLOSE fd_out and fd_in (get inherited through fork)
*/

void	cmd_pipe1_to_pipe2(t_pipex *p, int cmd_i)
{
	dup2(p->pipe1[READ_END], STDIN_FILENO);
	close_pipe(p->pipe1);
	dup2(p->pipe2[WRITE_END], STDOUT_FILENO);
	close_pipe(p->pipe2);
	close(p->fd_out);
	close(p->fd_input);
	if (execve(p->commands[cmd_i][0], p->commands[cmd_i], p->envp) == ERROR)
		program_errors(p, "EXECUTION ERROR", TRUE);
}

/*
**	Send the STDOUT from the command to pipe1
**	1.	REDIRECT the READ_END of pipe2 to STDIN
**	2.	CLOSE the pipe
**	3.	REDIRECT the WRITE_END of pipe1 to STDOUT
**	4.	CLOSE the pipe
**	5.	CLOSE fd_out and fd_in (get inherited through fork)
*/

void	cmd_pipe2_to_pipe1(t_pipex *p, int cmd_i)
{
	dup2(p->pipe2[READ_END], STDIN_FILENO);
	close_pipe(p->pipe2);
	dup2(p->pipe1[WRITE_END], STDOUT_FILENO);
	close_pipe(p->pipe1);
	close(p->fd_out);
	close(p->fd_input);
	if (execve(p->commands[cmd_i][0], p->commands[cmd_i], p->envp) == ERROR)
		program_errors(p, "EXECUTION ERROR", TRUE);
}
