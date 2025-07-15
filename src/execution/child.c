/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:49:29 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 23:47:37 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	safe_execve(t_pipedata *p, char *path, char **argv, char **env)
{
	t_stat	st;

	if (fstat(STDOUT_FILENO, &st) == -1 && errno == EBADF)
		ft_exit_child(p, NULL, 1);
	return (execve(path, argv, env));
}

static void	execute_child_builtin(t_cmd **tokens, t_pipedata *p)
{
	if (p->pipe_count > 0)
	{
		safe_close(&p->stdin_copy);
		safe_close(&p->stdout_copy);
		build_handler(p, tokens, p->cmd_index);
		ft_exit_child(p, NULL, ft_atoi(find_export("?")));
	}
	build_handler(p, tokens, 0);
}

static void	close_unused_child_fds(t_pipedata *p, int *child_stdin,
		int *child_stdout)
{
	int	j;

	j = 0;
	while (j < p->pipe_count)
	{
		if (p->pipefd[j][READ] != *child_stdin)
			safe_close(&p->pipefd[j][READ]);
		if (p->pipefd[j][WRITE] != *child_stdout)
			safe_close(&p->pipefd[j][WRITE]);
		j++;
	}
	if (dup2(*child_stdin, STDIN_FILENO) < 0 && p->is_child == true)
		ft_exit_child(p, "dup2", 1);
	if (dup2(*child_stdout, STDOUT_FILENO) < 0)
		ft_exit_child(p, "dup2", 1);
	if (*child_stdin != p->infile && *child_stdin != STDIN_FILENO)
		safe_close(&*child_stdin);
	if (*child_stdout != p->outfile && *child_stdout != STDOUT_FILENO)
		safe_close(&*child_stdout);
	if (p->infile != STDIN_FILENO)
		safe_close(&p->infile);
	if (p->outfile != STDOUT_FILENO)
		safe_close(&p->outfile);
}

static void	setup_read_and_write_ends(t_pipedata *p, int *child_stdin,
		int *child_stdout)
{
	*child_stdin = -1;
	*child_stdout = -1;
	if (p->pipe_count == 0)
	{
		*child_stdin = p->infile;
		*child_stdout = p->outfile;
	}
	else if (p->pipe_index == 0)
	{
		*child_stdin = p->infile;
		if (p->has_out_redirect)
			*child_stdout = p->outfile;
		else
			*child_stdout = p->pipefd[p->pipe_index][WRITE];
	}
	else if (p->pipe_index == p->pipe_count)
	{
		*child_stdin = p->pipefd[p->pipe_index - 1][READ];
		*child_stdout = p->outfile;
	}
	else
	{
		*child_stdin = p->pipefd[p->pipe_index - 1][READ];
		*child_stdout = p->pipefd[p->pipe_index][WRITE];
	}
}

void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;
	int		child_stdin;
	int		child_stdout;

	setup_read_and_write_ends(p, &child_stdin, &child_stdout);
	close_unused_child_fds(p, &child_stdin, &child_stdout);
	if (p->is_builtin == true)
	{
		execute_child_builtin(tokens, p);
		return ;
	}
	safe_close(&p->stdin_copy);
	safe_close(&p->stdout_copy);
	path = get_bin_path(mini_strndup(tokens[p->cmd_index]->str,
				ft_strlen(tokens[p->cmd_index]->str)), env, p);
	if (access(p->cmd_args[0], X_OK) >= 0
		&& ft_strncmp(p->cmd_args[0], "/", 1) == 0)
		if (safe_execve(p, p->cmd_args[0], p->cmd_args, env) < 0)
			ft_exit_child(p, NULL, 1);
	if (safe_execve(p, path, p->cmd_args, env) < 0 && path_exists() == 1)
		ft_exit_child(p, NULL, 1);
	else
		ft_exit_child(p, NULL, 0);
}
