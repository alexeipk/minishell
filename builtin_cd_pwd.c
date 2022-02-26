/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 12:50:48 by aprotoce          #+#    #+#             */
/*   Updated: 2022/02/09 12:50:48 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printpwd(void)
{
	char	cwd[4096];

	printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void	changeDir(t_parse_data data)
{
	char	cwd[4096];
	int		status;

	status = chdir(data.args[1]);
	if (ft_strncmp(data.args[1], " ", 1) || !data.args[1])
	{
		getcwd(cwd, 4096);
		return ;
	}	
	if (status == 0)
	{
		getcwd(cwd, 4096);
		chdir(cwd);
	}
	else
	{
		printf("%s\n", "Error, you need a correct parameter");
		return ;
	}
	change_or_add_value(data.dict, "PWD", getcwd(cwd, sizeof(cwd)));
}
