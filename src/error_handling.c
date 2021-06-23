/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 11:52:17 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/23 15:43:07 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	usage_error(t_input *in, char *usage_name, t_bool stop)
{
	if (ft_strequal(usage_name, "USAGE"))
	{
		ft_putendl_fd("Not enough arguments given.", 2);
		ft_putstr_fd(usage_name, 2);
		ft_putstr_fd(": ./pipex \"filename of file 1\"", 2);
		ft_putstr_fd(" \"n Commands (with flags or args)\"", 2);
		ft_putstr_fd(" \"filename of file 2\"\n", 2);
	}
	else if (ft_strequal("NO INPUT", usage_name)
		|| ft_strequal("NO PERMISSION", usage_name))
		perror("Input file");
	else if (!ft_strncmp("COMMAND ", usage_name, 8))
		perror(usage_name);
	if (stop == TRUE)
	{
		clear_data(in);
		exit(EXIT_FAILURE);
	}
}
