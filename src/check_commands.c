/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 16:30:27 by bcosters          #+#    #+#             */
/*   Updated: 2021/07/15 11:09:23 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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

void	valid_command(t_pipex *p, int argc, char **argv)
{
	char	**cmd_args;
	int		i;
	char	*err;

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

void	find_command_paths(t_pipex *p, int argc, char **argv, char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
	}
	p->env_paths = ft_split(envp[i], ':');
	p->env_paths[0] = ft_substr(p->env_paths[0], 5,
		ft_strlen(p->env_paths[0] - 5));
	i = -1;
	while (p->env_paths[++i])
		p->env_paths[i] = ft_strjoin_char(p->env_paths[i], '/');
	i = -1;
	valid_command(p, argc, argv);
}

/*
**	Add the right path to each command for the 'execve' function
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
			*cmd_arg = cmd_path;
			break ;
		}
		ft_strdel(&cmd_path);
	}
}

/*
**	Index 1 to (argc - 1) are commands to be piped
**		The array of commands is of size (argc - 2) AKA the last one is NULL
**		=> Array of "argv arrays" of the commands
**		=> Update the argv[0] of each command to have the right path
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
