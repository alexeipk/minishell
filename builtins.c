/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 12:05:54 by aprotoce          #+#    #+#             */
/*   Updated: 2022/02/06 12:05:54 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_builtin(t_parse_data data)
{
	char	**envp;

	envp = make_envp_from_dict(data.dict);
	print_str_array(envp);
	free_str_array(envp);
}

void	unset_builtin(t_parse_data data)
{
	char	**args;
	t_node	*node;
	int		hash;

	args = data.args;
	args++;
	while (*args != NULL)
	{
		node = find_dict_node(data.dict, *args);
		if (node != NULL)
		{
			hash = get_hash(*args) % DICT_ARRAY_SIZE;
			data.dict[hash] = remove_node_from_list(data.dict[hash], node);
		}
		args++;
	}
}

void	export_builtin(t_parse_data data)
{
	char	**args;
	t_node	*node;

	args = data.args;
	args++;
	while (*args != NULL)
	{
		node = find_dict_node(data.dict, *args);
		if (node != NULL)
			node->operator = 1;
		args++;
	}
}

int	check_builtin(t_parse_data data)
{
	if (!(ft_strncmp(data.args[0], "echo", 4)))
		return (1);
	if (!(ft_strncmp(data.args[0], "unset", 4)))
		return (1);
	if (!(ft_strncmp(data.args[0], "export", 6)))
		return (1);
	if (!(ft_strncmp(data.args[0], "env", 3)))
		return (1);
	if (!(ft_strncmp(data.args[0], "exit", 4)))
		return (1);
	if (!(ft_strncmp(data.args[0], "pwd", 3)))
		return (1);
	if (!(ft_strncmp(data.args[0], "cd", 2)))
		return (1);
	return (0);
}

void	exec_builtin(t_parse_data data)
{
	if (!(ft_strncmp(data.args[0], "echo", 4)))
		echo_cmd(data);
	if (!(ft_strncmp(data.args[0], "export", 6)))
		export_builtin(data);
	if (!(ft_strncmp(data.args[0], "unset", 5)))
		unset_builtin(data);
	if (!(ft_strncmp(data.args[0], "env", 3)))
		env_builtin(data);
	if (!(ft_strncmp(data.args[0], "exit", 4)))
		exit_minishell(data);
	if (!(ft_strncmp(data.args[0], "pwd", 3)))
		printpwd();
	if (!(ft_strncmp(data.args[0], "cd", 2)))
		changeDir(data);
}
