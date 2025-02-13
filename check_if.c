/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamraouy <aamraouy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:24:51 by aamraouy          #+#    #+#             */
/*   Updated: 2025/02/13 10:35:04 by aamraouy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	file_or_cmd(char **cmd)
{
	if (ft_strchr(cmd[0], '/'))
	{
		if (access(cmd[0], X_OK) == 0)
			return (1);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_split(cmd);
		exit(127);
	}
	return (0);
}
