/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprotoce <aprotoce@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 11:10:20 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/26 16:43:49 by aprotoce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	key_null(t_node **dict, char *key, char *str)
{
	if (key == NULL)
		temp_str('$');
	else
		add_str_to_temp(find_var(dict, key));
	while (*str != '\0')
	{
		temp_str(*str);
		str++;
	}
}

char	*replace_vars_in_str(t_node **dict, char *str)
{
	char	*key;

	temp_str(-1);
	temp_str2(-1);
	key = NULL;
	if (is_char_in_set('$', str))
	{
		while (*str != '$')
		{
			temp_str(*str);
			str++;
		}
		str++;
		while (*str != '\0' && !is_char_in_set(*str, " 	"))
		{
			key = temp_str2(*str);
			str++;
		}
		key_null(dict, key, str);
		return (ft_strdup(temp_str(-2)));
	}
	return (NULL);
}

void	replace_vars_in_token_list(t_node **dict, t_node **list_addr)
{
	char	*temp;
	int		has_erased;
	t_node	*list;

	list = *list_addr;
	while (list != NULL)
	{
		has_erased = 0;
		if (list->operator == 0)
		{
			temp = replace_vars_in_str(dict, list->data);
			if (temp != NULL)
			{	
				free(list->data);
				list->data = temp;
				if (ft_strlen(temp) == 0)
				{
					free(list->data);
					*list_addr = remove_node_from_list(*list_addr, list);
					list = *list_addr;
					has_erased = 1;
				}
			}
		}
		if (!has_erased)
			list = list->next;
	}
}

void	assign_var(t_parse_data data)
{
	change_or_add_value(data.dict, get_key_value(data.assigment->data, 'k'),
		get_key_value(data.assigment->data, 'v'));
}
