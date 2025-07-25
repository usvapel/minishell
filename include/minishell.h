/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:12:49 by erantala          #+#    #+#             */
/*   Updated: 2025/07/20 00:08:18 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>

# include "structs.h"
# include "functions.h"

# define ARENA_SIZE 32768
# define ALIGNMENT 8

# define EOF1 "minishell: warning: here-document at line "
# define EOF2 " delimited by end-of-file (wanted `"

# define MS "minishell: "
# define QUIT "Quit (core dumped)"
# define INV "': not a valid identifier\n"
# define SHLVL "minishell: warning: shell level (%d) too high, resetting to 1\n"
# define TOKEN "minishell: syntax error near unexpected token `"
# define HRLIM "minishell: maximum here-document count exceeded\n"
# define PRMD "minishell: Permission denied"
# define NSFOD ": No such file or directory"
# define AMBG ": ambiguous redirect"
# define MARG "cd: too many arguments"
# define CMD ": command not found"
# define NMARG ": numeric argument required\n"

# define PROMPT	"\001\x1b[38;5;231m\002> " \
				"\001\x1b[38;5;156m\002minishell" \
				"\001\x1b[0m\002\001\x1b[38;5;231m\002$\001\x1b[0m\002 "

# define HD_PROMPT	"\001\x1b[38;5;231m\002>> \001\x1b[0m\002"

# define COLOR_WHITE	"\001\x1b[38;5;231m\002"
# define COLOR_PINK		"\001\x1b[38;5;219m\002"

extern sig_atomic_t		g_sig;

typedef struct stat		t_stat;

#endif // MINISHELL_H