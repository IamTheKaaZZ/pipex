/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa_base_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 09:49:02 by bcosters          #+#    #+#             */
/*   Updated: 2021/04/08 17:03:22 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_bonus.h"

/*
**	Function that returns an allocated string of the uint converted to basestr
**	-> temp is malloced with size 84 so it is definitely big enough
**	-> result gets malloced with the exact size needed
**		=> ft_str_realloc (free and NULLs temp)
**	RETURN: result string with exact good size!
*/

char	*ft_uitoa_base(t_ui n, char *basestr)
{
	char	*temp;
	char	*result;

	temp = malloc(84 * sizeof(char));
	if (!(temp))
		return (NULL);
	temp = ft_ui_to_base(temp, basestr, n);
	result = ft_str_realloc(temp, ft_strlen(temp));
	return (result);
}
