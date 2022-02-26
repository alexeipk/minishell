/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:22:37 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/26 16:38:09 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	size_of_list(t_node *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		i++;
		list = list->next;
	}
	return (i);
}

t_node	*add_char_to_list(t_node *list, char *data)
{
	t_node	*node;

	if (data == NULL)
		return (list);
	node = malloc(sizeof(t_node));
	node->next = NULL;
	node->data = ft_strdup(data);
	if (list != NULL)
		get_last_node(list)->next = node;
	else
		list = node;
	return (list);
}

void	print_list(t_node *list)
{
	while (list != NULL)
	{
		printf("%s %d, ", (char *)list->data, list->operator);
		list = list->next;
	}
	printf("\n");
}

void	print_str_array(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("%s\n", args[i]);
		i++;
	}
}
