# include "../minishell.h"

t_result	create_success(void *value)
{
	t_result	result;

	result.is_error = false;
	result.data.error = NO_ERROR;
	result.data.value = value;
	return result;
}

t_result	create_error(t_error error_code)
{
	t_result	result;

	result.is_error = true;
	result.data.error = error_code;
	return result;
}
