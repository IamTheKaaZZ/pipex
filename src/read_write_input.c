/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 16:08:20 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 16:09:34 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*read_input_file(t_pipex *p)
{
	int	ret_read;
	char	buff;
	char	*str;

	str = NULL;
	ret_read = read(p->fd_input, &buff, 1);
	if (ret_read == -1)
		program_errors(p, "READING FILE", TRUE);
	while(ret_read)
	{
		str = ft_strjoin_char(str, buff);
		ret_read = read(p->fd_input, &buff, 1);
		if (ret_read == -1)
		{
			if (str)
				ft_strdel(&str);
			program_errors(p, "READING FILE", TRUE);
		}
	}
	close(p->fd_input);
	return (str);
}

void	write_input(t_pipex *p)
{
	char	*str;
	int		size;

	str = NULL;
	str = read_input_file(p);
	size = ft_strlen(str) + 1;
	write(p->pipe[1], str, size);
	ft_strdel(&str);
}
