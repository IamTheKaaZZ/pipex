/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:54:45 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 16:56:58 by bcosters         ###   ########.fr       */
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

typedef struct s_input
{
	int		argc;
	char	**argv;
	char	**envp;
	char	**env_paths;
	char	**commands;
	int		fd_input;
	int		fd_out;
	int		**pipes;
}		t_input;

void	init_data(t_input *in);
void	clear_data(t_input *i);

/*
**	Error handling
*/

void	usage_error(t_input *in, char *usage_name, t_bool stop);
void	find_command_paths(t_input *in, int argc, char **argv, char **envp);
void	program_errors(t_input *in, char *errname, t_bool stop);

/*
**	Main program
*/

void	get_commands(t_input *in);

#endif