/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 14:52:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/15 01:58:42 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_here_lines(int fd, t_vector	*lines)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return ;
		add_elem(lines, mini_strdup(line));
		free(line);
	}
}

void	here_check(int fd, char *name, t_data *data, size_t index)
{
	char		**file;
	t_vector	*lines;
	size_t		i;
	size_t		j;

	i = 0;
	lines = new_vector(4);
	get_here_lines(fd, lines);
	file = vec_to_array(lines);
	while (file[i])
	{
		j = 0;
		while (file[i][j] && j < ft_strlen(file[i]))
		{
			if (file[i][j] == '$')
				file[i] = here_expansion(file[i], &j);
			else
				j++;
		}
		i++;
	}
	fix_lines(file, index, name, data);
}

// write new heredoc file.
char	*here_expansion(char *ln, size_t *i)
{
	char	*expansion;

	expansion = (find_export(mini_strndup((ln + (*i)), key_len(ln + *i))));
	if (ft_strcmp(expansion, "") == 0)
	{
		expansion = mini_strndup(ln, *i);
		*i += key_len(ln + *i);
		ln = mini_join(expansion, ln + *i);
		return (ln);
	}
	expansion = mini_join(mini_strndup(ln, (*i)), expansion);
	if ((*i + key_len(ln + *i)) < ft_strlen(ln))
		expansion = mini_join(expansion, ln + *i + key_len(ln + *i));
	*i = 0;
	return (expansion);
}

void	fix_lines(char **file, size_t index, char *name, t_data *data)
{
	int		fd;
	size_t	i;

	fd = data->hdfd[index];
	i = 0;
	safe_close(&fd);
	fd = open(name, O_WRONLY | O_TRUNC);
	if (fd == -1)
		soft_exit("heredoc:", 1, 1);
	while (file[i])
	{
		if (write(fd, file[i], ft_strlen(file[i])) < 0)
			soft_exit("heredoc", 1, 1);
		i++;
	}
	safe_close(&fd);
}

char	*mini_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (mini_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	res = arena_malloc(len + 1);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
