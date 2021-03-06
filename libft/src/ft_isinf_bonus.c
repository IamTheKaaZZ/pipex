/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinf_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 15:11:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/04/08 16:53:02 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_bonus.h"

t_bool	ft_isinf(double num)
{
	return (num == (1.0 / 0.0) || num == (-1.0 / 0.0));
}
