/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:13:22 by stakhtou          #+#    #+#             */
/*   Updated: 2024/06/05 08:45:03 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100000
# endif
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

long double	ft_atoi(const char *str);
void		ft_bzero(void *s, size_t n);
int			ft_isalpha(int i);
int			ft_isascii(int c);
int			ft_isdigit(int i);
int			ft_isprint(int c);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memset(void *b, int c, size_t len);
char		*ft_strchr(const char *s, int c);
int ft_isalnum(int c);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *hayStack, const char *needle, size_t len);
char		*ft_strrchr(const char *s, int c);
int			ft_tolower(int c);
int			ft_toupper(int c);
char	*ft_strcpy(char *s1, const char *s2);
char		*ft_strdup(const char *s1);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
char		*ft_substr(const char *s, unsigned int start, size_t len);
char		*ft_strjoin(char *line, char *buff);
char		*ft_strtrim(char const *s1, char const *set);
char **ft_splitD(const char *s, const char *delimiters);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
size_t ft_strcat (char *dest, const char *src);
void		ft_putnbr_fd(int n, int fd);
char		*readex_line(int fd, char *line);
char		*zyada_mn_rass_l7me9(char *line);
char		*get_next_line(int fd);

int			ft_putchar(char c, int *len);
int			ft_putstr(char *str);
int			ft_putuphex(size_t c);
int			ft_puthex(size_t c);
int			ft_putdec(long num);
int			ft_putundec(unsigned int num);
int			ft_calclen(int n);
int			ft_printf(const char *printer, ...);
int			ft_putadress(void *add);
char		*ft_strncpy(char *dest, const char *src, unsigned int n);
int ft_strcmp(const char *s1, const char *s2);
#endif
