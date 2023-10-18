/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 19:47:04 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/27 20:13:20 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_H
# define FT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "ft_fprintf.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
/**
 * @brief sets to 0 the first n positions of a pointer 
 */
void		ft_bzero(void *s, size_t n);
int			ft_isascii(int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isalpha(int c);
size_t		ft_strlen(const char *str);
/**
 * @brief sets to int the first n positions of a ptr
*/
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int			ft_isprint(int c);
int			ft_isdigit(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);
/**
 * @brief finds the first occurrence of 'c' in 's'
 * @return a pointer to the first 'c'. NULL if not found
*/
char		*ft_strchr(const char *s, int c);

/**
 * @brief finds the last occurrence of 'c' in 's'
 * @return a pointer to the last 'c'. NULL if not found
*/
char		*ft_strrchr(const char *s, int c);

/**
 * @brief up to size - 1 characters from the NUL-terminated string src
 * to dst, * NUL-terminating the result.
 * @param dstsize size of dst, or, you can use it to copy only dstsize - 1
 * chars from src, as long as dstsize <= length(dst)
*/
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);

/**
 * @brief appends the NUL-terminated string src to the end of dst.
 * It will append at most size - strlen(dst) - 1 bytes,
 * NUL-terminating the result.  
*/
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
/**
 * @brief locates the first occurrence of the string little in the	string big,
 * where not more than len characters are searched.
*/
char		*ft_strnstr(const char	*big, const char *little, size_t len);
char		*ft_strdup(const char *s);
/**
 * @brief Returns the substring of the given string at the given start 
 * position with the given length (or smaller if the length of the 
 * original string is less than start + length, 
 * or length is bigger than MAXSTRINGLEN).
*/
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
/**
 * @brief strtrim removes all whitespace characters from the beginning and 
 * 		the end of a string.
 * As whitespace is counted everything for which isspace(3) returns true.
*/
char		*ft_strtrim(char const *s1, char const *set);
/**
 * @brief splits a string every time it finds one or more
 * consecutive 'c'. Every resulting string is null terminated.
 * @return a char** of the chuncks extracted, the last string
 * is \0.
 */
char		**ft_split(char const *s, char c);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_memchr(const void *s, int c, size_t n);
char		*ft_itoa(int n);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list		*ft_lstnew(void *content);
int			ft_lstsize(t_list *lst);
/**
 * @return on success, char * of the line read.
 * On error and at EOF, NULL.
*/
char		*get_next_line(int fd);
char		*fill_temp_stash(int fd, char *temp_stash);
char		*ft_strjoin_gnl(char *temp_stash, char *buff);
char		*ft_strchr_gnl(char *s, int c);
char		*fill_ret(char *s);
char		*trim_temp_stash(char *temp_stash);

#endif