/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 13:10:47 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	valid_command(char **argv)
{
	char	**cmd_args;
	char	*cmd_path;
	int		i;
	char	*err;

	i = 1;
	while (++i < 4)
	{
		cmd_args = ft_split(argv[i], ' ');
		err = ft_strjoin("COMMAND ", cmd_args[0]);
		cmd_path = ft_strjoin("/bin/", cmd_args[0]);
		if (access(cmd_path, X_OK) == -1)
		{
			usage_error(err, FALSE);
			ft_strdel(&cmd_path);
			ft_strdel(&err);
			ft_str_array_del(&cmd_args);
			exit(EXIT_FAILURE);
		}
		ft_strdel(&cmd_path);
		ft_strdel(&err);
		ft_str_array_del(&cmd_args);
	}
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		usage_error("USAGE", TRUE);
	if (access(argv[1], F_OK) == -1)
		usage_error("NO INPUT", TRUE);
	if (access(argv[1], R_OK) == -1)
		usage_error("NO PERMISSION", TRUE);
	valid_command(argv);
	(void)envp;
	exit(EXIT_SUCCESS);
}
