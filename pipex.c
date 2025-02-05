/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamraouy <aamraouy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:40:35 by aamraouy          #+#    #+#             */
/*   Updated: 2025/02/05 10:46:02 by aamraouy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_abs_path(char *cmd1, char **env)
{
	char	**env_paths;
	char	*abs_path;
	int	i;

	i = 0;
	while (env[i] && (ft_strncmp(env[i], "PATH=", 5) != 0))
		i++;
	env_paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (env_paths[i])
	{
		abs_path = ft_strjoin(env_paths[i], "/");
		abs_path = ft_strjoin(abs_path, cmd1);
		if (access(abs_path, F_OK | X_OK) == 0)
		{
			free_split(env_paths);
			return (abs_path);
		}
		free(abs_path);
		i++;
	}
	free_split(env_paths);
	return (NULL);
}

void	run_command(char *v, char **env)
{
	char	**args;
	char	*absolute_path;

	args = ft_split(v, ' ');
	if (!args)
		exit(EXIT_FAILURE);
	absolute_path = get_abs_path(args[0], env);
	if (!absolute_path)
		exit(127);
	if (execve(absolute_path, args, NULL) == -1)
	{
		free_split(args);
		exit_error();
	}
}

void	child_process(int *pipe_fd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		exit(1);
	dup2(fd, 0);
	close(fd);
	close(pipe_fd[0]);
	dup2(pipe_fd[1], 1);
	run_command(argv[2], env);
}

void	parent_process(int *pipe_fd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC , 0644);
	if (fd == -1)
		exit(1);
	dup2(fd, 1);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	run_command(argv[3], env);
}

// void f(){ system("losf -e pipex");}

int	main(int argc, char **argv, char **env)
{
	// atexit(f);
	int	pipe_fd[2];
	pid_t	pr_id;

	if (argc != 5)
		exit_error();
	pipe(pipe_fd);
	pr_id = fork();
	if (pr_id == -1)
		return(1);
	if (!pr_id)
		child_process(pipe_fd, argv, env);
	wait(NULL);
	parent_process(pipe_fd, argv, env);
}
