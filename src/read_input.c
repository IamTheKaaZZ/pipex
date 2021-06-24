/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:40:16 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/24 15:45:47 by bcosters         ###   ########.fr       */
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
