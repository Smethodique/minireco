#include"../minishell.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (*head == NULL)
	{
		*head = new_token;
	}
	else
	{
		current = *head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_token;
	}
}
void	free_tokens(t_token *head) // Explicitly declare return type as void
{
	t_token *tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->value); // Assuming ft_strdup allocates memory
		free(tmp);
	}
}
t_token	*new_token(int type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = type;
	token->value = ft_strdup(value);
	token->space = 0;
	token->next = NULL;
	return (token);
}