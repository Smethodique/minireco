#include "../minishell.h"

int myrand(void)
{
    static unsigned int seed = 0;
    seed = seed * 1103515245 + 12345;
    return ((unsigned int)(seed / 65536) % 32768);
}

int create_temp_file(char *template) {
    int fd;
    while (1) {
        int i = 0;
        while (i < 6) {
            
            template[strlen(template) - 6 + i] = 'a' + myrand() % 26;
            i++;
        }
        fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0)
            break;
    }
    return fd;
}
int my_mkstemp(char *template) {
    int fd;
    char *p ;
    int len ;

    len = strlen(template);
    p = NULL;
    if (len < 6)
        return -1;
    p = template + len - 6;

    int i = 0;
    while (i < 6) {
        if (p[i] != 'X') {
            return -1;
        }
        i++;
    }

    fd = create_temp_file(template);
    if (fd < 0) {
        perror("mkstemp");
        return -1;
    }

    return fd;
}

void	*ft_calloc(size_t count, size_t size)
{
	void *ptr;

	ptr = malloc(count * size);
	if (ptr == NULL)
		return (ptr);
	ft_bzero(ptr, size * count);
	return (ptr);
}

	void *ft_realloc(void *ptr, size_t size)
{
    void *new_ptr;

    if (!ptr)
        return (malloc(size));
    if (!size)
    {
        free(ptr);
        return (NULL);
    }
    new_ptr = malloc(size);
    if (!new_ptr)
        return (NULL);
    ft_memcpy(new_ptr, ptr, size);
    free(ptr);
    return (new_ptr);
}

