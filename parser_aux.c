/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 15:17:31 by aprotoce          #+#    #+#             */
/*   Updated: 2022/02/26 15:17:31 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*iterate_list(t_node *list, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		list = list->next;
		i++;
	}
	return (list);
}

t_parse_data	parse_cmd_and_args(t_parse_data data)
{
	t_node	*token_list;
	t_node	*temp;
	int		i;

	i = 0;
	token_list = *data.token_list;
	data.args = malloc(sizeof(char *) * (size_of_list(token_list) + 1));
	while (token_list != NULL)
	{
		temp = token_list;
		data.args[i] = token_list->data;
		token_list = token_list->next;
		i++;
		free(temp);
	}
	data.args[i] = NULL;
	return (data);
}

void	print_parse(t_parse_data data)
{
	t_node	*as;

	as = data.assigment;
	printf("in: %d\n", data.fd_in);
	printf("out: %d\n", data.fd_out);
	if (as != NULL)
		printf("assigment: %s\n", (char *)as->data);
	printf("arguments: ");
	print_str_array(data.args);
	printf("\n");
}

void	assign_var(t_parse_data data)
{
	change_or_add_value(data.dict, get_key_value(data.assigment->data, 'k'),
		get_key_value(data.assigment->data, 'v'));
}
