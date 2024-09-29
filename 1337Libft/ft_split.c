/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 18:03:41 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/24 05:05:22 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

static size_t	word_count(const char *s, const char *delimiters)
{
    size_t count = 0;
    int in_word = 0;
    while (*s)
    {
        if (strchr(delimiters, *s))
            in_word = 0;
        else if (!in_word)
        {
            in_word = 1;
            count++;
        }
        s++;
    }
    return (count);
}

static char *allocate_word(const char *s, size_t len)
{
    char *word = (char *)malloc(len + 1);
    if (word)
    {
        strncpy(word, s, len);
        word[len] = '\0';
    }
    return (word);
}

static void split_helper(const char *s, const char *delimiters, char **arr)
{
    int i;
	int j;
	size_t len;
	int k;

	i = 0;
	j = 0;
	len = 0;
	k = 0;
    while (s[i])
	{
		if (strchr(delimiters, s[i]))
		{
			if (len > 0)
				arr[j++] = allocate_word(s + k, len);
			len = 0;
			k = i + 1;
		}
		else
			len++;
		i++;
	}
    if (len > 0)
        arr[j++] = allocate_word(s + k, len);
    arr[j] = NULL;
}

char **ft_splitD(const char *s, const char *delimiters)
{
   size_t words ;
   words = word_count(s, delimiters);
    char **arr = (char **)malloc(sizeof(char *) * (words + 1));
    if (!arr)
        return NULL;
    split_helper(s, delimiters, arr);
    return arr;
}







