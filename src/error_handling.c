/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:52:17 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/26 10:13:11 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	Function used to throw errors related to USAGE
**	1. Throws specific error depending on call
**	2. If stop is TRUE, it will clear all data before exiting
*/

void	usage_error(t_pipex *p, char *usage_name, t_bool stop)
{
	if (ft_strequal(usage_name, "USAGE"))
	{
		ft_putendl_fd("Not enough arguments given.", 2);
		ft_putstr_fd(usage_name, 2);
		ft_putstr_fd(": ./pipex \"filename of file 1\"", 2);
		ft_putstr_fd(" \"n Commands (with flags or args)\"", 2);
		ft_putstr_fd(" \"filename of file 2\"\n", 2);
	}
	else if (ft_strequal("NO INPUT FILE", usage_name)
		|| ft_strequal("NO PERMISSION", usage_name))
		perror(usage_name);
	else if (!ft_strncmp("COMMAND ", usage_name, 8))
		perror(usage_name);
	else if (ft_strequal("OPENING INPUT FILE", usage_name))
		perror(usage_name);
	else if (ft_strequal("OPENING OUTPUT FILE", usage_name))
		perror(usage_name);
	if (stop == TRUE)
	{
		clear_data(p);
		exit(EXIT_FAILURE);
	}
}

/*
**	Function to throw specific errors during program runtime
**	1.	Send errname to perror
**	2. If stop is TRUE, it will clear all data before exiting
*/

void	program_errors(t_pipex *p, char *errname, t_bool stop)
{
	perror(errname);
	if (stop == TRUE)
	{
		clear_data(p);
		exit(EXIT_FAILURE);
	}
}

/*
**	Function to check for error related to the waitpid function
**	1.	Error if waitpid returns -1
**	2.	Check wstatus, if it's not 0, the command in the child process failed
*/

void	wait_error_check(t_pipex *p, pid_t pid)
{
	if (waitpid(pid, &p->wstatus, 0) == ERROR)
		program_errors(p, "WAIT", TRUE);
	if (WIFEXITED(p->wstatus) != TRUE)
	{
		if (WEXITSTATUS(p->wstatus) != 0)
			program_errors(p, "COMMAND FAIL", TRUE);
	}
}
