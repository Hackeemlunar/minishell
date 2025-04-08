/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 18:32:15 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:14:03 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <strings.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdarg.h>
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

// ******* Data Structures *****************

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_context
{
	int		err;
	int		nl;
	size_t	buf_cap;
	size_t	buf_pos;
	size_t	buf_pos_prv;
	size_t	stash_len;
	size_t	stash_st;
	char	stash[BUFFER_SIZE];
	char	buffer[4096];
}			t_context;

typedef struct s_formated_data
{
	int		count;
	char	*fstring;
}				t_fdata;

typedef struct s_modifiers_info
{
	char	flags[5];
	int		width;
	int		precision;
	char	specifier;
	int		flags_count;
}				t_modinfo;

typedef struct s_arena
{
	size_t	size;
	size_t	used;
	char	*buffer;
}			t_arena;

// ************** Part 1 - Libc functions **************
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strcat(char *dst, const char *src);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
double	ft_atod(const char *str);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// ************** GNL functions **************
char	*get_next_line(int fd);
void	cleanup_context(t_context *ctx);
void	*ft_strncpy(char *dst, const char *src, size_t n);
char	*handle_eof_err(t_context *ctx, ssize_t byt_read);
void	handle_stash(t_context *ctx, char **line);

// ************** Ft_printf Functions **************
int		ft_printf(const char *format, ...);
void	parse_format(const char *fmt, t_modinfo *modinfo);
void	apply_precision(t_modinfo *modinfo, t_fdata *fdata);
void	apply_prefix(const char *str, t_modinfo *info, t_fdata *fdata);
void	ft_putstr_pf(t_fdata *data);
void	apply_minus_flag(t_modinfo *info, t_fdata *data);
t_fdata	*process_specifier(t_modinfo *modinfo, va_list args);
t_fdata	*handle_char(t_modinfo *modinfo, va_list args);
t_fdata	*handle_string(t_modinfo *modinfo, va_list args);
t_fdata	*handle_pointer(t_modinfo *modinfo, va_list args);
t_fdata	*handle_num_int(t_modinfo *modinfo, va_list args);
t_fdata	*handle_num_long(t_modinfo *modinfo, va_list args);
t_fdata	*handle_uns(t_modinfo *modinfo, va_list args);
t_fdata	*handle_hex(t_modinfo *modinfo, va_list args);
t_fdata	*handle_percent(t_modinfo *info);
t_fdata	*create_number_data(t_modinfo *modinfo, long nbr, int base);
char	*ft_itoa_base(long nbr, int base, int uppercase);
void	apply_left_justify(char *fstring, int width, t_fdata *fdata);
void	apply_r_justify(char *fstring, int width, char pad, t_fdata *fdata);
void	apply_r_justify(char *fstring, int width, char pad, t_fdata *fdata);
void	handle_string_size(char *fstring, int size, t_fdata *data);
void	handle_int_size(char *fstring, char pad, int size, t_fdata *data);
void	apply_plus_space_flag(t_modinfo *info, t_fdata *data);

// ************** Arena Functions **************
void	arena_destroy(t_arena *arena);
void	*arena_alloc(t_arena *arena, size_t size);
void	arena_reset(t_arena *arena);
t_arena	*arena_create(size_t size);
#endif
