/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 06:56:35 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 12:31:31 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static int	countwords(char const *s, char c)
{
	int	find;
	int	count;

	find = 0;
	count = 0;
	while (*s)
	{
		if ((*s != c) && (find == 0))
		{
			find = 1;
			count++;
		}
		else if (*s == c)
			find = 0;
		s++;
	}
	return (count);
}

static int	wordlen(char const *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static void	copyword(char *dest, char const *s, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = s[i];
		i++;
	}
	dest[len] = '\0';
}

static char	**sort(char const *s, char **str, char c, int words)
{
	int		len;
	int		k;

	k = 0;
	while (k < words)
	{
		while (*s == c)
			s++;
		len = wordlen(s, c);
		str[k] = (char *) malloc(sizeof(char) * (len + 1));
		if (str[k] == NULL)
		{
			while (k >= 0)
				free (str[k--]);
			free (str);
			return (NULL);
		}
		copyword(str[k], s, len);
		k++;
		s += len;
	}
	str[words] = NULL;
	return (str);
}

char	**split(char const *s, char c)
{
	int		words;
	char	**str;

	if (!s)
		return (NULL);
	words = countwords(s, c);
	str = (char **) malloc(sizeof(char *) * (words + 1));
	if (str == NULL)
		return (NULL);
	str = sort(s, str, c, words);
	return (str);
}
