/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 01:00:50 by erantala          #+#    #+#             */
/*   Updated: 2025/07/18 00:38:02 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	empty_export(void)
{
	size_t	i;
	t_data	*data;
	char	*ln;

	init_export();
	data = get_data();
	i = 0;
	while (i < data->exp->count)
	{
		ln = data->exp->data[i];
		if (ft_strncmp(ln, "?=", 2) != 0 && ft_strncmp(ln, "_", 1) != 0)
		{
			ft_fprintf(1, "declare -x %s", mini_strndup(ln, key_len(ln) + 1));
			ln += key_len(ln);
			if (ln[0])
				ft_printf("\"%s\"\n", ln + 1);
			else
				ft_printf("\n");
		}
		i++;
	}
	replace_export("?=0");
}

void	sort_export(size_t count, t_data *data, char **exps)
{
	char	*line_1;
	char	*line_2;
	size_t	i;
	size_t	lowest;

	while (count > 0)
	{
		lowest = 0;
		i = 0;
		while (i < count)
		{
			line_1 = exps[lowest];
			line_2 = exps[i];
			if (ft_strcmp(line_1, line_2) > 0)
				lowest = i;
			i++;
		}
		line_1 = exps[lowest];
		add_elem(data->exp, line_1);
		strarm(exps, lowest);
		count--;
	}
}

void	init_export(void)
{
	char	**exports;
	t_data	*data;

	data = get_data();
	data->exp = new_vector(data->env_vec->count);
	exports = vec_to_array(data->env_vec);
	sort_export(data->env_vec->count, data, exports);
}

char	**strarm(char **s, size_t i)
{
	size_t	max;

	max = ft_stralen(s);
	if (i == max - 1)
		s[i] = NULL;
	while (i < max - 1)
	{
		s[i] = s[i + 1];
		i++;
	}
	s[i] = NULL;
	return (s);
}

void	empty_export_char(char **s)
{
	size_t	i;
	char	*line;

	i = 0;
	while (s[i])
	{
		line = s[i];
		if (ft_strncmp(line, "?=", 2) != 0 && ft_strncmp(line, "_", 1) != 0)
		{
			if (write(STDOUT_FILENO, "declare -x ", 11) < 0)
				perror("write");
			if (write(STDOUT_FILENO, line, key_len(line) + 1) < 0)
				perror("write");
			ft_putchar_fd('"', STDOUT_FILENO);
			line += key_len(line) + 1;
			ft_printf("%s\"\n", line);
		}
		i++;
	}
}
