/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/13 14:05:33 by bcosters         ###   ########.fr       */
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
	if (access(argv[1], F_OK) == ERROR)
		usage_error(p, "NO INPUT FILE", TRUE);
	if (access(argv[1], R_OK) == ERROR)
		usage_error(p, "NO PERMISSION", TRUE);
	find_command_paths(p, argc, argv, envp);
	p->fd_input = open(argv[1], O_RDONLY);
	if (p->fd_input == ERROR)
		usage_error(p, "OPENING INPUT FILE", TRUE);
	p->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p->fd_out == ERROR)
		usage_error(p, "OPENING OUTPUT FILE", TRUE);
	p->argc = argc;
	p->argv = argv;
	p->envp = envp;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;
	int		i;
	int		prev_pipe;

	init_data(&p);
	check_input(&p, argc, argv, envp);
	get_commands(&p);
	open_pipe(&p, p.pipe1);
	p.pid_in = fork();
	if (p.pid_in == ERROR)
		program_errors(&p, "FORKING", TRUE);
	if (p.pid_in == CHILD_PROCESS)
		write_input_to_cmd(&p);
	wait_error_check(&p, p.pid_in);
	i = 0;
	prev_pipe = p.pipe1[READ_END];
	while (++i < p.n_cmds - 1)
	{
		open_pipe(&p, p.pipe2);
		p.pid_cmd = fork();
		if (p.pid_cmd == ERROR)
			program_errors(&p, "FORKING CMD", TRUE);
		if (p.pid_cmd == CHILD_PROCESS)
		{
			if (prev_pipe != STDIN_FILENO)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close_pipe(p.pipe1);
			}
			dup2(p.pipe2[WRITE_END], STDOUT_FILENO);
			close_pipe(p.pipe2);
			if (execve(p.commands[i][0], p.commands[i], p.envp) == ERROR)
				program_errors(&p, "EXECUTION ERROR", TRUE);
		}
		close(prev_pipe);
		close(p.pipe2[WRITE_END]);
		prev_pipe = p.pipe2[READ_END];
		printf("i = %d and readfd = %d\n", i, prev_pipe);
	}
	close_pipe(p.pipe1);
	close(p.pipe2[WRITE_END]);
		printf("readfd = %d\n", p.pipe2[READ_END]);
	p.pid_out = fork();
	if (p.pid_out == ERROR)
		program_errors(&p, "FORKING", TRUE);
	if (p.pid_out == CHILD_PROCESS)
		write_cmd_to_output(&p, p.pipe2[READ_END]);
	clear_data(&p);
	wait_error_check(&p, p.pid_out);
	exit(EXIT_SUCCESS);
}
