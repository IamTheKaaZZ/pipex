/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_widestrjoin_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 12:18:06 by bcosters          #+#    #+#             */
/*   Updated: 2021/04/08 17:04:16 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_bonus.h"

wchar_t	*ft_widestrjoin(const wchar_t *s1, const wchar_t *s2)
{
	wchar_t	*join;
	size_t	i;
	size_t	size;

	if (!s1)
		size = ft_wstrlen(s2) + 1;
	else if (!s2)
		size = ft_wstrlen(s1) + 1;
	else
		size = ft_wstrlen(s1) + ft_wstrlen(s2) + 1;
	join = (wchar_t *)malloc(size * sizeof(wchar_t));
	if (!(join))
		return (NULL);
	i = 0;
	while (s1 && *s1)
		join[i++] = *s1++;
	while (s2 && *s2)
		join[i++] = *s2++;
	join[i] = '\0';
	return (join);
}
