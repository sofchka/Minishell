/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:29:17 by szakarya          #+#    #+#             */
/*   Updated: 2025/01/27 17:26:29 by szakarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2, int should_free)
{
	unsigned int	s1_len;
	unsigned int	s2_len;
	unsigned int	i;
	char			*ptr;
	char			*tmp_s1;

	if (!s1)
		return (0);
	tmp_s1 = s1;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ptr = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s1_len-- != 0)
		ptr[i++] = *s1++;
	while (s2_len-- != 0)
		ptr[i++] = *s2++;
	ptr[i] = '\0';
	if (should_free == 1 && ptr[0] != '\0')
		free(tmp_s1);
	return (ptr);
}
