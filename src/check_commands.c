/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 16:30:27 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 17:14:48 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	check_path_variable(t_input *in, char **args, char *err)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (in->env_paths[++i])
	{
			cmd_path = ft_strjoin(in->env_paths[i], args[0]);
		if (access(cmd_path, X_OK) != -1)
		{
			ft_strdel(&cmd_path);
			break ;
		}
		ft_strdel(&cmd_path);
	}
	if (!in->env_paths[i])
	{
		usage_error(in, err, FALSE);
		ft_strdel(&err);
		ft_str_array_del(&args);
		usage_error(in, "EXIT", TRUE);
	}
}

static void	valid_command(t_input *in, int argc, char **argv)
{
	char	**cmd_args;
	int		i;
	char	*err;

	i = 1;
	while (++i < argc - 1)
	{
		cmd_args = ft_split(argv[i], ' ');
		err = ft_strjoin("COMMAND ", cmd_args[0]);
		check_path_variable(in, cmd_args, err);
		ft_strdel(&err);
		ft_str_array_del(&cmd_args);
	}
}

void	find_command_paths(t_input *in, int argc, char **argv, char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
	}
	in->env_paths = ft_split(envp[i], ':');
	in->env_paths[0] = ft_substr(in->env_paths[0], 5,
		ft_strlen(in->env_paths[0] - 5));
	i = -1;
	while (in->env_paths[++i])
		in->env_paths[i] = ft_strjoin_char(in->env_paths[i], '/');
	valid_command(in, argc, argv);
}

/*
**	Index 1 to (argc - 1) are commands to be piped
**		The array of commands is of size (argc - 2) AKA the last one is NULL
*/

void	get_commands(t_input *in)
{
	int		i;
	int		j;

	in->commands = (char **)ft_calloc(in->argc - 2, sizeof(char *));
	if (!in->commands)
		program_errors(in, "MALLOC", TRUE);
	i = 1;
	j = -1;
	while (++i < in->argc - 1)
		in->commands[++j] = in->argv[i];
}
