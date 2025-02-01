/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamraouy <aamraouy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:40:35 by aamraouy          #+#    #+#             */
/*   Updated: 2025/02/01 17:29:08 by aamraouy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_abs_path(char *cmd1, char **env)
{
	char	**env_paths;
	char	*abs_path;
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	env_paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (env_paths[i])
	{
		abs_path = ft_strjoin(env_paths[i], "/");
		abs_path = ft_strjoin(env_paths[i], cmd1);
		if (access(abs_path, F_OK))
		{//signal request to the kernel to ask for the existance of the file 
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
		exit(127);//command not found
	if (execve(absolute_path, &v, env) == -1)
	{
		free_split(args);
		exit_error();
	}
}

void	child_process(int *pipe_fd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY, 0776);
	if (fd == -1)
		exit(1);
	dup2(fd, 0);
	close(pipe_fd[0]);
	dup2(pipe_fd[1], 1);
	run_command(argv[2], env);
}

void	parent_process(int *pipe_fd, char **argv, char **env)//here the std out of this process will be the file specified by the argv[3] // stdin will be the pipe exactly the read_end(0)
{
	int	fd;

	fd = open(argv[3], O_WRONLY, 0776);
	if (fd == -1)
		exit(1);
	dup2(fd, 1);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	run_command(argv[2], env);
}

int	main(int argc, char **argv, char **env)
{
	int	pipe_fd[2];
	pid_t	pr_id;

	if (argc != 5)
		exit_error();
	// parser(argv, env);
	pipe(pipe_fd);//here i created a buffer with two ends and this space is managed by the kernel;
	pr_id = fork();//forked two processes;
	if (pr_id == -1)
		exit(-1);
	if (!pr_id)//this child process will handle the first process in which i execute the first command and redirect its output to the write_end of pipe;
		child_process(pipe_fd, argv, env);
	parent_process(pipe_fd, argv, env);
}
