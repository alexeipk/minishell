/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:23:55 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/26 21:42:33 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_data	parse_in(t_parse_data data)
{
	int		index;
	t_node	*temp;

	if (errno == 502)
		return (data);
	index = find_node_index(*data.token_list, "<", "<<");
	if (index < 0)
	{
		if (data.last_was_pipe)
			data.fd_in = data.pipe2[0];
		else
			data.fd_in = 0;
		return (data);
	}
	while (index >= 0)
	{
		temp = iterate_list(*data.token_list, index);
		if (temp->next == NULL || temp->next->operator > 0)
		{
			errno = 502;
			free_list(*data.token_list);
			return (data);
		}
		if (strncmp(temp->data, "<<", 2) == 0)
			data.fd_in = here_doc(temp->next->data);
		else
			data.fd_in = get_fd_for_file(temp->next->data, 1, data.dict);
		*data.token_list = remove_node_from_list(*data.token_list, temp->next);
		free(temp->data);
		*data.token_list = remove_node_from_list(*data.token_list, temp);
		index = find_node_index(*data.token_list, "<", "<<");
	}
	return (data);
}

t_parse_data	parse_out(t_parse_data data, t_node *other_pipes)
{
	int		index;
	t_node	*temp;

	if (errno == 502)
		return (data);
	index = find_node_index(*data.token_list, ">", ">>");
	if (index < 0)
	{
		if (other_pipes != NULL)
			data.fd_out = data.pipe1[1];
		else
			data.fd_out = 1;
		return (data);
	}
	while (index >= 0)
	{
		temp = iterate_list(*data.token_list, index);
		if (temp->next == NULL || temp->next->operator > 0)
		{
			errno = 502;
			free_list(*data.token_list);
			return (data);
		}
		if (strncmp(temp->data, ">>", 2) == 0)
			data.fd_out = get_fd_for_file(temp->next->data, 1, data.dict);
		else
			data.fd_out = get_fd_for_file(temp->next->data, 0, data.dict);
		*data.token_list = remove_node_from_list(*data.token_list, temp->next);
		free(temp->data);
		*data.token_list = remove_node_from_list(*data.token_list, temp);
		index = find_node_index(*data.token_list, ">", ">>");
	}
	return (data);
}

t_parse_data	parse_assigment(t_parse_data data)
{
	t_node	*node;

	node = *data.token_list;
	if (is_char_in_set('=', node->data))
	{
		if (data.assigment != NULL)
		{
			free(data.assigment->data);
			free(data.assigment);
		}
		data.assigment = node;
		*data.token_list = node->next;
	}
	return (data);
}

void	process_cmd(t_parse_data data)
{
	data.envp = make_envp_from_dict(data.dict);
	if (check_builtin(data) == 1)
		exec_builtin(data);
	else
	{
		data.bin_path = check_command_path(data);
		if (data.bin_path != NULL)
			exec_command(data);
		free(data.bin_path);
	}
	free_str_array(data.args);
	free_str_array(data.envp);
}

void	parse(t_node *token_list, t_node **dict)
{
	t_node			*sub_token_list;
	t_parse_data	data;

	data.last_was_pipe = 0;
	data.assigment = NULL;
	data.dict = dict;
	while (token_list != NULL)
	{
		data.pipe2[0] = data.pipe1[0];
		data.pipe2[1] = data.pipe1[1];
		pipe(data.pipe1);
		sub_token_list = split_by_pipe(&token_list);
		data.token_list = &sub_token_list;
		data = parse_in(data);
		data = parse_out(data, token_list);
		verify_errno(errno, 502, "Sintax error\n");
		if (verify_errno(errno, 502, "Sintax error\n"))
			return ;
		if (*data.token_list == NULL)
			return ;
		data = parse_assigment(data);
		data = parse_cmd_and_args(data);
		if (token_list == NULL && data.assigment != NULL && !data.last_was_pipe)
			assign_var(data);
		else if (*data.args != NULL)
			process_cmd(data);
		data.last_was_pipe = 1;
	}
}
