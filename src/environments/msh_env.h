
#ifndef MSH_ENV_H
# define MSH_ENV_H
# include "../minishell.h"

# define HASH_SIZE 128

typedef struct s_env
{
	char		*key;
	char		*value;
	struct s_env	*next;
}			t_env;

typedef struct s_table
{
	t_env	*bucket[HASH_SIZE];
	int		size;
}	t_table;

#endif
