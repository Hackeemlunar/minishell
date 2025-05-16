# include "../minishell.h"

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(8192);
	allocs->sh_alloc = arena_create(8192);
	allocs->exec_alloc = arena_create(8192);
}

void	clean_mshell(t_allocs *allocs, t_table *table)
{
	clean_env(table);
	arena_destroy(allocs->parse_alloc);
	arena_destroy(allocs->sh_alloc);
	arena_destroy(allocs->exec_alloc);
}

t_result get_paths(t_table *table, char ***paths, t_allocs *allocs)
{
    t_result    result;
    char        *path;
    char        **path_split;
    int         i;

    result = get_env(table, "PATH");
    if (result.is_error)
        return (result);
    path = result.data.value;
    path_split = ft_split(path, ':');
    if (!path_split)
        return (create_error(NO_MEMORY));
    i = 0;
    while (path_split[i])
        i++;
    *paths = arena_alloc(allocs->sh_alloc, (i + 1) * sizeof(char*));
    i = -1;
    while (path_split[++i])
    {
        (*paths)[i] = arena_alloc(allocs->sh_alloc, ft_strlen(path_split[i]) + 3);
        ft_strlcpy((*paths)[i], path_split[i], ft_strlen(path_split[i]) + 1);
        ft_strlcat((*paths)[i], "/", ft_strlen(path_split[i]) + 2);
        free(path_split[i]);
    }
    (*paths)[i] = NULL;
    return (free(path_split), create_success(NULL));
}

int	check_all_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!whitespace(str[i]))
			return (0);
		i++;
	}
	return (1);
}
