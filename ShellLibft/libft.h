/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:39:01 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/26 10:15:22 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif
// list type struct

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// Struct || Nodes;
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new_thing);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new_thing);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
// Memmory ones
void				*ft_memset(void *s, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_bzero(void *s, size_t n);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
// Strings
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strndup(const char *s, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strrchr(const char *s, int c);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
size_t				ft_strlen(const char *s);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
size_t				ft_strlcat(char *dest, const char *src, size_t size);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(const char *s1, const char *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(const char *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
// Numbers
int					ft_atoi(const char *str);
int					ft_toupper(int c);
int					ft_tolower(int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
// Ft_Printf
// int					ft_printf(const char *format, ...);
// int					ft_printptr(unsigned long long ptr);
// int					ft_printunsigned(unsigned int n);
// int					ft_printhex(unsigned int num, const char format);
// size_t				intputnbr(int n);
// size_t				uintputnbr(unsigned int n);
// int					ft_printpercent(void);
// int					ft_printstr(char *s);
size_t				ft_putchar(int c);
// Get_next_line
char				*get_next_line(int fd);
char				*readfile(int fd, char *res);
char				*nextline(char *buf);
char				*makeline(char *buf);
void				*bytecheck(ssize_t bytes, char *buf, char *line);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_freejoin(char *buffer, char *new_thing);
char				*ft_strchr(const char *str, int car);
size_t				ft_strlen(const char *str);
void				*ft_calloc(size_t nmemb, size_t size);

#endif
