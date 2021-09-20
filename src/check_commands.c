/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 16:30:27 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/20 12:19:10 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	Checks the PATH variable of the environment for the given commands
**	1.	Iterate through all the possible paths
**		=> Make the command path with strjoin of the path + first arg of cmd
**		=> Check with access X_OK flag if the command exists
**	2.	If the command is found: break out of the loop and no error
**	3.	Else: The command does not exist => EXIST
*/

void	check_path_variable(t_pipex *p, char **args, char *err)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (p->env_paths[++i])
	{
		cmd_path = ft_strjoin(p->env_paths[i], args[0]);
		if (access(cmd_path, X_OK) != -1)
		{
			ft_strdel(&cmd_path);
			break ;
		}
		ft_strdel(&cmd_path);
	}
	if (!p->env_paths[i])
	{
		usage_error(p, err, FALSE);
		ft_strdel(&err);
		ft_str_array_del(&args);
		usage_error(p, "EXIT", TRUE);
	}
}

/*
**	Check if the given commands are valid by looking for their executable
**	1.	Go through argv from index 1 (or 2 in HERE_DOC mode) to argc - 1
**	2.	Split the command and its flags by space + Make a cmd specific error
**	3.	Check all the paths for the executable
**	4.	If it is found: OK
**	5	Else: Return the specific error
*/

void	valid_command(t_pipex *p, int argc, char **argv)
{
	char	**cmd_args;
	int		i;
	char	*err;

	i = 1;
	if (p->mode == PIPE)
		i = 1;
	else if (p->mode == HERE_DOC)
		i = 2;
	while (++i < argc - 1)
	{
		cmd_args = ft_split(argv[i], ' ');
		err = ft_strjoin("COMMAND ", cmd_args[0]);
		check_path_variable(p, cmd_args, err);
		ft_strdel(&err);
		ft_str_array_del(&cmd_args);
	}
}

/*
**	Check the PATH variable in the env for all possible command paths
**	1.	Iterate through envp until we find the PATH variable
**	2.	Split the line in envp by ":" to get all paths
**	3.	The first path still includes "PATH=" so we remove it with substr
**	4.	Add "/" to the end of every path so that execve will work correctly
**	5.	Check if the given commands are valid
*/

void	find_command_paths(t_pipex *p, int argc, char **argv, char **envp)
{
	int		i;
	char	*joined;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
	}
	p->env_paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (p->env_paths[++i])
	{
		joined = ft_strjoin_char(p->env_paths[i], '/');
		ft_strdel(&p->env_paths[i]);
		p->env_paths[i] = joined;
	}
	i = -1;
	valid_command(p, argc, argv);
}

/*
**	Add the right path to each command for the 'execve' function
**	=> Like in check_path_variable
**		=> iterate through all possible paths and add them to the cmd
**		=> check with access if it exists
**			=> if it exists => found the right path
**		=> This way execve will always find the command
*/

void	update_command_path(t_pipex *p, char **cmd_arg)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (p->env_paths[++i])
	{
		cmd_path = ft_strjoin(p->env_paths[i], *cmd_arg);
		if (access(cmd_path, X_OK) != -1)
		{
			ft_strdel(cmd_arg);
			*cmd_arg = cmd_path;
			break ;
		}
		ft_strdel(&cmd_path);
	}
}

/*
**	Index 1 (or 2 in HERE_DOC mode) to < (argc - 1) are commands to be piped
**		The array of commands is of size (argc - 2) AKA the last one is NULL
**		=> Array of "argv arrays" of the commands
**			=> Every command has a "char **argv"
**			=> Index 0 is the path to the command
**			=> Other indices are the flags for the command
**		=> Update the argv[0] (the path) of each command to have the right path
**			=> Split the argv of the program by space to get cmd_args
**			=> Update the paths so execve finds it everytime
**			=> Add it to the command array and increment the counter
**		=> After: env_paths is no longer needed
*/

void	get_commands(t_pipex *p)
{
	int		i;
	int		j;
	char	**cmd_args;

	p->commands = (char ***)ft_calloc(p->argc - 2, sizeof(char **));
	if (!p->commands)
		program_errors(p, "MALLOC", TRUE);
	i = 1;
	if (p->mode == PIPE)
		i = 1;
	else if (p->mode == HERE_DOC)
		i = 2;
	j = -1;
	while (++i < p->argc - 1)
	{
		cmd_args = ft_split(p->argv[i], ' ');
		if (!cmd_args)
			program_errors(p, "COMMAND SPLIT MALLOC", TRUE);
		update_command_path(p, &cmd_args[0]);
		p->commands[++j] = cmd_args;
		p->n_cmds++;
	}
	ft_str_array_del(&p->env_paths);
}
