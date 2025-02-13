/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamraouy <aamraouy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:40:35 by aamraouy          #+#    #+#             */
/*   Updated: 2025/02/13 08:23:45 by aamraouy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_abs_path(char *cmd1, char **env)
{
	char	**env_paths;
	char	*abs_path;
	int		file_cmd;
	int		i;

	i = 0;
	file_cmd = file_or_cmd(cmd1);
	if (file_cmd)
		return (ft_strdup(cmd1));
	while (env[i] && (ft_strncmp(env[i], "PATH=", 5) != 0))
		i++;
	env_paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (env_paths[i])
	{
		abs_path = ft_strjoin(env_paths[i], "/");
		abs_path = ft_strjoin(abs_path, cmd1);
		if (access(abs_path, X_OK) == 0)
			return (abs_path);
		free(abs_path);
		i++;
	}
	free(env_paths);
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
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(absolute_path, args, env) == -1)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(" No such file or directory\n", 2);
		free_split(args);
		free(absolute_path);
		exit(1);
	}
}

void	child_process(int *pipe_fd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(1);
	}
	if (dup2(fd, 0) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], 1) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close (pipe_fd[1]);
	run_command(argv[2], env);
}

void	parent_process(int *pipe_fd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_RDWR | O_TRUNC , 0644);
	if (fd == -1)
	{
		ft_putstr_fd(argv[4], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(1);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], 0) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[0]);
	run_command(argv[3], env);
}

// void f(){ system("lsof  pipex");}

int	main(int argc, char **argv, char **env)
{
	int	pipe_fd[2];
	pid_t	pr_id;
	int		status;

	if (argc != 5)
		exit_error();
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pr_id = fork();
	if (pr_id == -1)
	{
		perror("fork");
		exit(1);
	}
	if (!pr_id)
		child_process(pipe_fd, argv, env);
	if (waitpid(pr_id, &status, 0) == -1)
	{
		perror("waitpid");
		exit(1);
	}
	parent_process(pipe_fd, argv, env);
}
