/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:23:37 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/30 17:25:13 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

/**
 * @brief Structure to hold formatted data during printf processing
 *
 * This structure stores the formatted string and character count
 * during the processing of format specifiers.
 */
typedef struct s_formated_data
{
	char	*fstring;
	int		count;
}				t_fdata;

/**
 * @brief Structure to hold format specifier information
 *
 * This structure contains all the parsed format specifier components
 * like flags, width, precision, etc.
 */
typedef struct s_modifiers_info
{
	char	flags[5];
	int		width;
	int		precision;
	int		flags_count;
	char	specifier;
}				t_modinfo;

/**
 * @brief Parse format string to extract modifier information
 *
 * @param fmt Format string to parse
 * @param modinfo Structure to store parsed information
 */
void	parse_format(const char *fmt, t_modinfo *modinfo);

/**
 * @brief Apply precision specifier to formatted data
 *
 * @param modinfo Format modifier information
 * @param fdata Formatted data structure to be modified
 */
void	apply_precision(t_modinfo *modinfo, t_fdata *fdata);

/**
 * @brief Apply prefix characters based on format specifiers
 *
 * @param str String representation
 * @param info Format modifier information
 * @param fdata Formatted data structure to be modified
 */
void	apply_prefix(const char *str, t_modinfo *info, t_fdata *fdata);

/**
 * @brief Write formatted string to output
 *
 * @param data Formatted data structure containing the string to write
 */
void	ft_putstr_pf(t_fdata *data);

/**
 * @brief Apply left-justification (minus flag)
 *
 * @param info Format modifier information
 * @param data Formatted data structure to be modified
 */
void	apply_minus_flag(t_modinfo *info, t_fdata *data);

/**
 * @brief Process specifier and return formatted data
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*process_specifier(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle character specifier (%c)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_char(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle string specifier (%s)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_string(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle pointer specifier (%p)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_pointer(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle integer specifiers (%d, %i)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_num_int(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle long integer conversion
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_num_long(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle unsigned integer specifier (%u)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_uns(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle hexadecimal specifiers (%x, %X)
 *
 * @param modinfo Format modifier information
 * @param args Variable arguments list
 * @return Formatted data structure
 */
t_fdata	*handle_hex(t_modinfo *modinfo, va_list args);

/**
 * @brief Handle percent sign specifier (%%)
 *
 * @param info Format modifier information
 * @return Formatted data structure
 */
t_fdata	*handle_percent(t_modinfo *info);

/**
 * @brief Create formatted data for numeric values
 *
 * @param modinfo Format modifier information
 * @param nbr Numeric value
 * @param base Base for conversion (10 for decimal, 16 for hex, etc.)
 * @return Formatted data structure
 */
t_fdata	*create_number_data(t_modinfo *modinfo, long nbr, int base);

/**
 * @brief Convert number to string in specified base
 *
 * @param nbr Number to convert
 * @param base Base for conversion
 * @param uppercase Whether to use uppercase letters for hex
 * @return String representation of the number
 */
char	*ft_itoa_base(long nbr, int base, int uppercase);

/**
 * @brief Apply left justification to formatted string
 *
 * @param fstring String to format
 * @param width Field width
 * @param fdata Formatted data structure to be modified
 */
void	apply_left_justify(char *fstring, int width, t_fdata *fdata);

/**
 * @brief Apply right justification to formatted string
 *
 * @param fstring String to format
 * @param width Field width
 * @param pad Padding character
 * @param fdata Formatted data structure to be modified
 */
void	apply_r_justify(char *fstring, int width, char pad, t_fdata *fdata);

/**
 * @brief Handle string size constraints
 *
 * @param fstring String to format
 * @param size Target size
 * @param data Formatted data structure to be modified
 */
void	handle_string_size(char *fstring, int size, t_fdata *data);

/**
 * @brief Handle integer size constraints
 *
 * @param fstring String to format
 * @param pad Padding character
 * @param size Target size
 * @param data Formatted data structure to be modified
 */
void	handle_int_size(char *fstring, char pad, int size, t_fdata *data);

/**
 * @brief Apply plus and space flags to formatted data
 *
 * @param info Format modifier information
 * @param data Formatted data structure to be modified
 */
void	apply_plus_space_flag(t_modinfo *info, t_fdata *data);

#endif