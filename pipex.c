/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 13:21:02 by bcosters         ###   ########.fr       */
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
	p->fd_input = open(argv[0], O_RDONLY);
	if (p->fd_input == -1)
		usage_error(p, "OPENING INPUT FILE", TRUE);
	p->fd_out = open(argv[0], O_WRONLY | O_CREAT, 0777);
	if (p->fd_out == -1)
		usage_error(p, "OPENING OUTPUT FILE", TRUE);
	p->argc = argc;
	p->argv = argv;
	p->envp = envp;
	//open the pipe
	if (pipe(p->pipe) == -1)
		program_errors(p, "OPENING PIPE", TRUE);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex p;

	init_data(&p);
	check_input(&p, argc, argv, envp);
	get_commands(&p);
	int	i = -1;
	int	j = -1;
	while (p.commands[++i])
	{
		j = -1;
		while (p.commands[i][++j])
			printf("%s ", p.commands[i][j]);
		printf("\n");
	}
	i = -1;
	while (++i < argc)
	{
		if (i == 1)
		{
			//redirect the input file fd to stdin
			dup2(p.fd_input, STDIN_FILENO);
			close (p.fd_input);
			execve(p.commands[0][0], p.commands[0], p.envp);
			/*p.pid = fork();
			if (p.pid == -1)
				program_errors(&p, "FORKING", TRUE);
			if (p.pid == 0)
			{
				dup2(p.pipe[1], p.fd_input);
				close(p.pipe[1]);
				close(p.pipe[2]);

			}*/
		}
	}

	clear_data(&p);
	exit(EXIT_SUCCESS);
}
