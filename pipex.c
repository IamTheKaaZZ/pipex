/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 10:49:46 by bcosters         ###   ########.fr       */
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

void	check_input(t_input *in, int argc, char **argv, char **envp)
{
	if (argc < 5)
		usage_error(in, "USAGE", TRUE);
	if (access(argv[1], F_OK) == -1)
		usage_error(in, "NO INPUT FILE", TRUE);
	if (access(argv[1], R_OK) == -1)
		usage_error(in, "NO PERMISSION", TRUE);
	find_command_paths(in, argc, argv, envp);
	in->fd_input = open(argv[0], O_RDONLY);
	if (in->fd_input == -1)
		usage_error(in, "OPENING INPUT FILE", TRUE);
	in->fd_out = open(argv[0], O_WRONLY | O_CREAT, 0777);
	if (in->fd_out == -1)
		usage_error(in, "CREATING/OVERWRITING OUTPUT FILE", TRUE);
	in->argc = argc;
	in->argv = argv;
	in->envp = envp;
	//open the pipe
	if (pipe(in->pipe) == -1)
		program_errors(in, "OPENING PIPE", TRUE);
}

int	main(int argc, char **argv, char **envp)
{
	t_input input;

	init_data(&input);
	check_input(&input, argc, argv, envp);
	get_commands(&input);
	int	i = -1;
	while (input.commands[++i])
		printf("%s\n", input.commands[i]);

	clear_data(&input);
	exit(EXIT_SUCCESS);
}
