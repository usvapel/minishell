/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:04:47 by erantala          #+#    #+#             */
/*   Updated: 2025/07/12 23:55:12 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ambigous(char *s, size_t i)
{
	size_t	pos;
	t_data	*data;
	size_t	len;
	char	*exp_check;

	len = word_len(s + i, '|');
	if (check_all_redir((s + i), len) == 0)
		return (NULL);
	pos = i;
	while (pos < len + i)
	{
		if (s[pos] == '"' || s[pos] == '\'')
			return (NULL);
		pos++;
	}
	exp_check = mini_strndup(s + i, word_len(s + i, '|'));
	data = get_data();
	data->valid = 0;
	ft_fprintf(2, "%s %s\n", mini_join(MS, exp_check), AMBG);
	return ("");
}

int	check_all_redir(char *s, size_t	len)
{
	char	*exp;
	size_t	pos;
	size_t	explen;

	pos = 0;
	while (pos < len && s[pos])
	{
		explen = key_len(s + pos);
		exp = find_export(mini_strndup(s + pos, explen));
		if (ft_strcmp("", exp) != 0)
			return (0);
		pos += key_len(s + pos);
	}
	return (1);
}

char	*mini_append(char *s1, char *s2)
{
	char	*dup;

	dup = arena_malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	ft_strlcat(dup, s1, ft_strlen(s1) + 1);
	dup[ft_strlen(s1)] = ' ';
	ft_strlcat(dup, s2, ft_strlen(s1) + ft_strlen(s2) + 2);
	return (dup);
}

size_t	word_len(char *s, int quote)
{
	int		i;
	char	c;

	i = 0;
	while (s[i] && ft_isspace(s[i]) == 0 && !check_specials(s[i], quote))
		i++;
	if (s[i] && i == 0 && check_specials(s[i], quote))
	{
		if (s[i] == '|')
			return (1);
		c = s[i];
		if (!s[i + 1] || (s[i + 1] && s[i + 1] != c))
			return (1);
		else
			return (2);
	}
	return (i);
}

// Count the lenght of a word
int	check_specials(int c, int quote)
{
	if ((quote == 0 || quote == -1) && (c == '\'' || c == '"'))
		return (1);
	if (c == '<' || c == '|' || c == '>' || c == quote)
		return (1);
	if (quote != -1 && c == '$')
		return (1);
	return (0);
}
