/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:54:45 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/13 13:24:37 by bcosters         ###   ########.fr       */
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
# define WRITE_END 1
# define READ_END 0
# define CHILD_PROCESS 0
# define ERROR -1

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
	int		n_cmds;
	int		fd_input;
	int		fd_out;
	int		pipe1[2];
	int		pipe2[2];
	pid_t	pid_in;
	pid_t	pid_cmd;
	pid_t	pid_out;
	int		wstatus;
}			t_pipex;

void	init_data(t_pipex *p);
void	clear_data(t_pipex *p);
void	open_pipe(t_pipex *p, int fd[2]);
void	close_pipe(int *pipe);

/*
**	Error handling
*/

void	usage_error(t_pipex *p, char *usage_name, t_bool stop);
void	find_command_paths(t_pipex *p, int argc, char **argv, char **envp);
void	program_errors(t_pipex *p, char *errname, t_bool stop);
void	wait_error_check(t_pipex *p, pid_t pid);

/*
**	Main program
*/

void	get_commands(t_pipex *p);
char	*read_from_fd(t_pipex *p, int fd);
void	write_input_to_cmd(t_pipex *p);
void	write_cmd_to_output(t_pipex *p, int readfd);
void	cmd_pipe1_to_pipe2(t_pipex *p, int cmd_i);
void	cmd_pipe2_to_pipe1(t_pipex *p, int cmd_i);

#endif
