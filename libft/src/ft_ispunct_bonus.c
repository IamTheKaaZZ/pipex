/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ispunct_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 15:13:07 by bcosters          #+#    #+#             */
/*   Updated: 2021/04/08 16:53:11 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_bonus.h"

t_bool	ft_ispunct(int c)
{
	if (ft_isprint(c) && !ft_isalnum(c) && c != ' ')
		return (1);
	else
		return (0);
}
