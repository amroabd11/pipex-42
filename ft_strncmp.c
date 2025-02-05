/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamraouy <aamraouy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:48:30 by aamraouy          #+#    #+#             */
/*   Updated: 2025/02/05 21:19:13 by aamraouy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if ((n < 1) || (s1 == s2))
		return (0);
	if (*s1 && *s1 == *s2)
		return (ft_strncmp(s1 + 1, s2 + 1, n - 1));
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
