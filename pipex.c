/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/13 16:16:47 by bcosters         ###   ########.fr       */
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
	int		prev_fd;

	init_data(&p);
	check_input(&p, argc, argv, envp);
	get_commands(&p);
	i = -1;
	prev_fd = p.fd_input;
	while (++i < p.n_cmds)
	{
		open_pipe(&p, p.pipe);
		p.pid_cmd = fork();
		if (p.pid_cmd == ERROR)
			program_errors(&p, "FORKING", TRUE);
		if (p.pid_cmd == CHILD_PROCESS)
			child_loop(&p, prev_fd, i);
		close(prev_fd);
		close(p.pipe[WRITE_END]);
		prev_fd = p.pipe[READ_END];
		wait_error_check(&p, p.pid_cmd);
	}
	clear_data(&p);
	exit(EXIT_SUCCESS);
}
