#include "../minishell.h"

//copy env to a linked list


t_env *create_env_node(char *env_entry)
{
    t_env *new = malloc(sizeof(t_env));
    if (!new)
    {
        perror("malloc");
        return NULL;
    }
    char *equal = ft_strchr(env_entry, '=');
    if (equal)
    {
        *equal = '\0';
        new->key = ft_strdup(env_entry);
        new->value = ft_strdup(equal + 1);
        new->next = NULL;
    }
    return new;
}

t_env *env_to_list(char **env)
{
    t_env *head = NULL;
    t_env *current = NULL;
    int i = 0;
    while (env[i])
    {
        t_env *new = create_env_node(env[i]);
        if (!new)
        {
            return NULL;
        }
        if (!head)
        {
            head = new;
            current = new;
        }
        else
        {
            current->next = new;
            current = new;
        }
        i++;
    }
    return head;
}
void print_env(t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}