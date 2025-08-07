/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:17:26 by szakarya          #+#    #+#             */
/*   Updated: 2025/01/30 19:18:21 by szakarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(char const *s, char c, int i)
{
	size_t	count;

	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		count++;
		if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] == '\'')
				i++;
		}
		else
		{
			while (s[i] && s[i] != c && s[i] != '\'')
				i++;
		}
	}
	return (count);
}

static int	ft_malloc(char **result, int position, size_t len, char *start)
{
	result[position] = malloc(len + 1);
	if (!result[position])
	{
		while (position >= 0)
			free(result[position--]);
		free(result);
		return (0);
	}
	ft_strlcpy(result[position], start, len + 1);
	return (1);
}

static int	split_2(char **s, char c, int *qu)
{
	size_t	len;

	len = 0;
	while (**s && ((*qu && **s != '\'') || (!*qu && **s != c && **s != '\'')))
	{
		++len;
		(*s)++;
	}
	if (!*qu && len == 0)
		return (-1);
	if (*qu && **s == '\'')
	{
		*qu = 0;
		(*s)++;
	}
	return (len);
}

static int	split(char **result, char *s, char c, int i)
{
	char	*start;
	int		qu;
	long	len;

	qu = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s == '\'')
		{
			qu = 1;
			s++;
		}
		start = (char *)s;
		len = split_2(&s, c, &qu);
		if (len != -1)
		{
			if (ft_malloc(result, i, len, start) == 0)
				return (0);
			i++;
		}
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(char *s, char c)
{
	char	**res;
	size_t	len;

	if (!s)
		return (NULL);
	len = word_count(s, c, 0);
	res = (char **)malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	if (split(res, s, c, 0) == 0)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
