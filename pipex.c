/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/15 11:04:17 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
**	Check the input for error and give error messages accordingly
**	IF there are NO errors
**		=> open the fd's for inputfile and outputfile
**			=> fd_input in READ ONLY mode
**				- shell command '< file1'
**			=> fd_out in WRITE ONLY mode (OR CREATE the file if it doesn't exist)
**				- shell command '> file2'
**		=> set the mode to PIPE
**		=> save argc, argv and envp into the struct
*/

void	check_pipe_mode(t_pipex *p, int argc, char **argv, char **envp)
{
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
}

void	check_input(t_pipex *p, int argc, char **argv, char **envp)
{
	if (argc < 5)
		usage_error(p, "USAGE", TRUE);
	if (p->mode == HERE_DOC)
	{
		find_command_paths(p, argc, argv, envp);
		p->fd_input = STDIN_FILENO;
		p->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (p->fd_out == ERROR)
			usage_error(p, "OPENING OUTPUT FILE", TRUE);
		p->limiter = ft_strdup(argv[2]);
	}
	else
		check_pipe_mode(p, argc, argv, envp);
	p->argc = argc;
	p->argv = argv;
	p->envp = envp;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;

	init_data(&p, argv);
	check_input(&p, argc, argv, envp);
	get_commands(&p);
	if (p.mode == PIPE)
		pipe_mode(&p);
	else if (p.mode == HERE_DOC)
		here_doc_mode(&p);
	clear_data(&p);
	exit(EXIT_SUCCESS);
}
