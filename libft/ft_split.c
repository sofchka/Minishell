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

static size_t	word_count(char const *s, char c)
{
	size_t	count;
	int		i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static int	ft_malloc(char **result, int position, size_t len)
{
	result[position] = malloc(len + 1);
	if (!result[position])
	{
		while (position >= 0)
			free(result[position--]);
		free(result);
		return (0);
	}
	return (1);
}

static int	split(char **result, char const *s, char c, int i)
{
	size_t	len;
	char	*start;

	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			++s;
		start = (char *)s;
		while (*s != c && *s)
		{
			++len;
			++s;
		}
		if (len > 0)
		{
			if (ft_malloc(result, i, len) == 0)
				return (0);
			ft_strlcpy(result[i], start, len + 1);
			i++;
		}
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	len;

	if (!s)
		return (NULL);
	len = word_count(s, c);
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
/*
#include <stdio.h>
#include <string.h>

void print_result(int success)
{
    if (success)
        printf("ok\n");
    else
        printf("Qaq\n");
}

int compare_arrays(char **arr1, char **arr2)
{
    int i = 0;
    while (arr1[i] && arr2[i]) {
        if (strcmp(arr1[i], arr2[i]) != 0)
            return 0;
        i++;
    }
    return arr1[i] == NULL && arr2[i] == NULL;
}

int main()
{
	char **result;
	result = ft_split("lorem ipsum dolor sit amet, consectetur 
	!!!!!adapiscing elit. Sed non risus.", 'i');
	char *expected10[] = {"lorem ", "psum dolor s", "t amet, 
	!!!!!!consectetur adap", "sc", "ng el", "t. Sed non r", "sus.", NULL};
	print_result(compare_arrays(result, expected10));
	int i = 0;
	while (result[i])
		free(result[i++]);
	free(result);
}*/
