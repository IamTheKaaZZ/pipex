/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 15:51:01 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_path_variable(t_input *in, char **args, char *err)
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

void	valid_command(t_input *in, int argc, char **argv)
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

int	main(int argc, char **argv, char **envp)
{
	t_input input;

	init_data(&input);
	if (argc < 5)
		usage_error(&input, "USAGE", TRUE);
	if (access(argv[1], F_OK) == -1)
		usage_error(&input, "NO INPUT", TRUE);
	if (access(argv[1], R_OK) == -1)
		usage_error(&input, "NO PERMISSION", TRUE);
	find_command_paths(&input, argc, argv, envp);
	clear_data(&input);
	exit(EXIT_SUCCESS);
}
