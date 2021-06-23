/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:21:57 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 11:38:30 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		ft_putendl_fd("Not enough arguments given.", 2);
		ft_putstr_fd("Usage: ./pipex \"filename of file 1\"", 2);
		ft_putstr_fd(" \"Cmd 1 (with flags or args)\"", 2);
		ft_putstr_fd(" \"Cmd 2 (with flags or args)\"", 2);
		ft_putstr_fd(" \"filename of file 2\"\n", 2);
		exit(EXIT_FAILURE);
	}
	(void)argv;
	(void)envp;
	exit(EXIT_SUCCESS);
}
