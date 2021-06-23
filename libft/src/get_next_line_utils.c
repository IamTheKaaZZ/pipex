/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 16:36:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/06/18 17:44:25 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

/*
**Returns the amount of chars of a string before the NUL-term*
*/

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (*str++)
		i++;
	return (i);
}

/*
**copies n bytes from memory area src to memory area dest.*
**The memory areas must not overlap.*
**IF there is overlap -> memmove*
*/

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tdest;
	unsigned char	*tsrc;

	if (!dest && !src)
		return (0);
	tdest = (unsigned char *)dest;
	tsrc = (unsigned char *)src;
	while (n--)
	{
		*tdest++ = *tsrc++;
	}
	return (dest);
}

/*
**returns a pointer to a new string which is a duplicate of the string s.*
**Memory for the new string is obtained with malloc(3),*
**and can be freed with free(3)*
*/

char	*ft_strdup(const char *s)
{
	size_t	size;
	char	*dup;

	size = ft_strlen(s) + 1;
	dup = (char *)malloc(size * sizeof(char));
	if (!(dup))
		return (NULL);
	return ((char *)ft_memcpy(dup, s, size));
}

/*
**Allocates (with malloc(3)) and returns a new string,*
**which is the result of the concatenation of ’s1’ and ’s2’.*
*/

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*join;
	size_t	i;
	size_t	size;

	if (!s1)
		size = ft_strlen(s2) + 1;
	else if (!s2)
		size = ft_strlen(s1) + 1;
	else
		size = ft_strlen(s1) + ft_strlen(s2) + 1;
	join = (char *)malloc(size * sizeof(char));
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
