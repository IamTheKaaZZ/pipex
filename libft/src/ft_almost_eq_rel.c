/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_almost_eq_rel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 16:16:14 by bcosters          #+#    #+#             */
/*   Updated: 2021/04/08 16:44:42 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_bonus.h"

t_bool	ft_almost_eq_rel(float a, float b, float maxreldiff)
{
	float	diff;
	float	largest;

	diff = ft_abs_float(a - b);
	a = ft_abs_float(a);
	b = ft_abs_float(b);
	if (b > a)
		largest = b;
	else
		largest = a;
	if (diff <= (largest * maxreldiff))
		return (1);
	return (0);
}
