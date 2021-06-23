/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 09:38:32 by bcosters          #+#    #+#             */
/*   Updated: 2021/02/28 16:37:47 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Swaps the contents of a and b
*/

void	ft_swap(int *a, int *b)
{
	int	swap;

	swap = *b;
	*b = *a;
	*a = swap;
}
