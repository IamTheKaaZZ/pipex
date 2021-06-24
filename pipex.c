/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 17:51:35 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
**	Check the input for error and give error messages accordingly
**	IF there are NO errors
**		=> open the fd's for inputfile and outputfile
**			=> fd_input in READ ONLY mode
**				- shell command '< file1'
**			=> fd_out in WRITE ONLY mode (OR CREATE the file f it doesn't exist)
**				- shell command '> file2'
**		=> save argc, argv and envp into the struct
**		=> open the pipe for the program
**			pipe[0] == fd[0] => Reading end
**			pipe[1] == fd[1] => Writing end
*/

void	check_input(t_pipex *p, int argc, char **argv, char **envp)
{
	if (argc < 5)
		usage_error(p, "USAGE", TRUE);
	if (access(argv[1], F_OK) == -1)
		usage_error(p, "NO INPUT FILE", TRUE);
	if (access(argv[1], R_OK) == -1)
		usage_error(p, "NO PERMISSION", TRUE);
	find_command_paths(p, argc, argv, envp);
	p->fd_input = open(argv[1], O_RDONLY);
	if (p->fd_input == -1)
		usage_error(p, "OPENING INPUT FILE", TRUE);
	p->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p->fd_out == -1)
		usage_error(p, "OPENING OUTPUT FILE", TRUE);
	p->argc = argc;
	p->argv = argv;
	p->envp = envp;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex p;

	init_data(&p);
	check_input(&p, argc, argv, envp);
	get_commands(&p);
	//open the pipe
	if (pipe(p.pipe) == -1)
		program_errors(&p, "OPENING PIPE", TRUE);
	p.pid_in = fork();
	if (p.pid_in == -1)
		program_errors(&p, "FORKING", TRUE);
	if (p.pid_in == 0)
	{
		close(p.pipe[1]);
		dup2(p.pipe[0], STDIN_FILENO);
		close(p.pipe[0]);
		if (execve(p.commands[0][0], p.commands[0], p.envp) == -1)
			program_errors(&p, "EXECUTION ERROR", TRUE);
	}
	close(p.pipe[0]);
	write_input(&p);
	close(p.pipe[1]);
	if (waitpid(p.pid_in, &p.wstatus, 0) == -1)
		program_errors(&p, "WAIT 1", TRUE);
	if (WIFEXITED(p.wstatus) != TRUE)
	{
		if (WEXITSTATUS(p.wstatus) != 0)
			program_errors(&p, "COMMAND FAIL", TRUE);
	}

	//open the pipe
	if (pipe(p.pipe) == -1)
		program_errors(&p, "OPENING PIPE", TRUE);
	p.pid_cmd1 = fork();
	if (p.pid_cmd1 == -1)
		program_errors(&p, "FORKING", TRUE);
	if (p.pid_cmd1 == 0)
	{
		close(p.pipe[0]);
		dup2(p.pipe[1], STDOUT_FILENO);
		close(p.pipe[1]);
		if (execve(p.commands[0][0], p.commands[0], p.envp) == -1)
			program_errors(&p, "EXECUTION ERROR", TRUE);
	}
	p.pid_cmd2 = fork();
	if (p.pid_cmd2 == -1)
		program_errors(&p, "FORKING", TRUE);
	if (p.pid_cmd2 == 0)
	{
		close(p.pipe[1]);
		dup2(p.pipe[0], STDIN_FILENO);
		close(p.pipe[0]);
		if (execve(p.commands[1][0], p.commands[1], p.envp) == -1)
			program_errors(&p, "EXECUTION ERROR", TRUE);
	}
	close(p.pipe[0]);
	close(p.pipe[1]);
	if (waitpid(p.pid_cmd1, &p.wstatus, 0) == -1)
		program_errors(&p, "WAIT 2", TRUE);
	if (WIFEXITED(p.wstatus) != TRUE)
	{
		if (WEXITSTATUS(p.wstatus) != 0)
			program_errors(&p, "COMMAND FAIL", TRUE);
	}
	if (waitpid(p.pid_cmd2, &p.wstatus, 0) == -1)
		program_errors(&p, "WAIT 3", TRUE);
	if (WIFEXITED(p.wstatus) != TRUE)
	{
		if (WEXITSTATUS(p.wstatus) != 0)
			program_errors(&p, "COMMAND FAIL", TRUE);
	}
	clear_data(&p);
	exit(EXIT_SUCCESS);
}
