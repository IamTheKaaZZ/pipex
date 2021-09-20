/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/20 13:28:12 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// #include "42_memleak_check/malloc_leak_checker.h"

/*
**	Check the input for errors and give error messages accordingly
**	IF there are NO errors
**		=> Find the command paths
**		=> Open the fd's for inputfile and outputfile
**			=> fd_input in READ ONLY mode
**				- shell command '< file1'
**			=> fd_out in WRITE ONLY mode (OR CREATE the file if it doesn't exist)
**				- shell command '> file2'
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

/*
**	Basic input error checking
**	1.	argc has to be AT LEAST 5 for any mode
**	2.	IF we are in HERE_DOC mode
**		=> argc has to be AT LEAST 6 => ELSE: ERROR
**		=> find the command paths
**		=> Open the temporary inputstream
**			=> Check for OPEN errors
**		=> Open the output file or create it WITH THE APPEND FLAG
**			=> Check for OPEN errors
**		=> Save the LIMITER in the struct
**	3.	ELSE: we are in pipe mode
**		=> check_pipe_mode
**	4.	Save the necessary variables in the struct for later
*/

void	check_input(t_pipex *p, int argc, char **argv, char **envp)
{
	if (argc < 5)
		usage_error(p, "USAGE", TRUE);
	if (p->mode == HERE_DOC)
	{
		if (argc < 6)
			usage_error(p, "HERE_DOC MODE USAGE", TRUE);
		find_command_paths(p, argc, argv, envp);
		p->fd_input = open("inputstream.txt", O_WRONLY | O_CREAT, 0777);
		if (p->fd_input == ERROR)
			usage_error(p, "OPENING INPUTSTREAM", TRUE);
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

/*
**	MAIN for pipex
**	1.	Init the data and check the mode
**	2.	Check the input for errors
**	3.	If there are no errors, get the commands
**	4.	IF mode == PIPE
**		=> pipe_mode
**	5.	IF mode == HERE_DOC
**		=> here_doc_mode
**	6.	Clear the saved data and close unused fd's after
**	SUCCES!
*/

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
