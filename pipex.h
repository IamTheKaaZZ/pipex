/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:54:45 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 17:31:34 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft_bonus.h"
# include <sys/wait.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

enum
{
	FALSE,
	TRUE
};

/*
**	Data structure
*/

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	char	**env_paths;
	char	***commands;
	int		fd_input;
	int		fd_out;
	int		pipe[2];
	int		pid_in;
	int		pid_cmd1;
	int		pid_cmd2;
	int		pid_out;
	int		wstatus;
}			t_pipex;

void	init_data(t_pipex *p);
void	clear_data(t_pipex *p);
void	reset_pipe(t_pipex *p);

/*
**	Error handling
*/

void	usage_error(t_pipex *p, char *usage_name, t_bool stop);
void	find_command_paths(t_pipex *p, int argc, char **argv, char **envp);
void	program_errors(t_pipex *p, char *errname, t_bool stop);

/*
**	Main program
*/

void	get_commands(t_pipex *p);
char	*read_input_file(t_pipex *p);
void	write_input(t_pipex *p);

#endif