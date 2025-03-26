/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:56:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/26 11:13:25 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

// static char	*remove_quotes(char *start, char *end)
// {
// 	char	*result;
// 	char	*dst;
// 	char	quote_char;
// 	bool	in_quote;

// 	result = ft_calloc(end - start + 1, sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	dst = result;
// 	in_quote = false;
// 	while (start < end)
// 	{
// 		if (!in_quote && (*start == '\'' || *start == '\"'))
// 		{
// 			in_quote = true;
// 			quote_char = *start++;
// 		}
// 		else if (in_quote && *start == quote_char && start++)
// 			in_quote = false;
// 		else
// 			*dst++ = *start++;
// 	}
// 	*dst = '\0';
// 	return (result);
// }

// int	add_token(char **tokens, int *count, char *start, char *end)
// {
// 	char	*cleaned_token;

// 	if (*count >= MAX_TOKENS - 1)
// 		return (-1);
// 	cleaned_token = remove_quotes(start, end);
// 	if (!cleaned_token)
// 		return (-1);
// 	tokens[*count] = cleaned_token;
// 	(*count)++;
// 	return (0);
// }

// static int	process_special_char(char **tokens, int *count, char **input)
// {
// 	if (is_special_char(**input))
// 	{
// 		if (add_token(tokens, count, *input, *input + 1) < 0)
// 			return (-1);
// 		(*input)++;
// 		return (1);
// 	}
// 	return (0);
// }

// static int	handle_quotes(char **input, bool *in_quote, char *quote_char)
// {
// 	if (!*in_quote && (**input == '\'' || **input == '\"'))
// 	{
// 		*in_quote = true;
// 		*quote_char = **input;
// 		(*input)++;
// 		return (1);
// 	}
// 	else if (*in_quote && **input == *quote_char)
// 	{
// 		*in_quote = false;
// 		*quote_char = '\0';
// 		(*input)++;
// 		return (1);
// 	}
// 	return (0);
// }

// char	**lexer(char *input, t_shell *shell)
// {
// 	char	**tokens;
// 	t_lexer	lx;

// 	lx = (t_lexer){0};
// 	tokens = ft_calloc(MAX_TOKENS, sizeof(char *));
// 	if (!tokens || !input)
// 		return (free(tokens), NULL);
// 	lx.start = input;
// 	while (*input && lx.tokcount < MAX_TOKENS - 1)
// 	{
// 		if (handle_quotes(&input, &lx.in_quote, &lx.quote_char))
// 			continue ;
// 		if (!lx.in_quote && (ft_isspace(*input) || is_special_char(*input)))
// 		{
// 			if (lx.start != input && add_token(tokens, &lx.tokcount, lx.start,
// 					input) < 0)
// 				return (free_tokens(tokens), NULL);
// 			if (process_special_char(tokens, &lx.tokcount, &input) < 0)
// 				return (free_tokens(tokens), NULL);
// 			lx.start = input + 1;
// 		}
// 		input++;
// 	}
// 	return (end_check(lx, input, tokens, shell));
// }

static char	*remove_quotes(char *start, char *end, t_shell *shell)
{
    char	*result;
    char	*dst;
    char	quote_char;
    bool	in_quote;

    result = ft_calloc(end - start + 1, sizeof(char));
    if (!result)
        return (NULL);
    dst = result;
    in_quote = false;
    while (start < end)
    {
        if (!in_quote && (*start == '\'' || *start == '\"'))
        {
            in_quote = true;
            quote_char = *start++;
        }
        else if (in_quote && *start == quote_char)
        {
            in_quote = false;
            start++;
        }
        else if (in_quote && quote_char == '\"' && *start == '$') // Expand variables in double quotes
        {
            char *var_start = start + 1;
            while (var_start < end && (ft_isalnum(*var_start) || *var_start == '_'))
                var_start++;
            char *var_name = ft_substr(start + 1, 0, var_start - (start + 1));
            char *value = get_env_value(var_name, shell->env);
            free(var_name);
            if (value)
            {
                ft_strlcpy(dst, value, ft_strlen(value) + 1);
                dst += ft_strlen(value);
            }
            start = var_start;
        }
        else
            *dst++ = *start++;
    }
    //*dst = '\0';
    return (result);
}

int	add_token(char **tokens, int *count, char *start, char *end, t_shell *shell)
{
    char	*cleaned_token;

    if (*count >= MAX_TOKENS - 1)
        return (-1);
    cleaned_token = remove_quotes(start, end, shell);
    if (!cleaned_token)
        return (-1);
    tokens[*count] = cleaned_token;
    (*count)++;
    return (0);
}

static int	process_special_char(char **tokens, int *count, char **input, t_shell *shell)
{
    if (is_special_char(**input))
    {
        if (add_token(tokens, count, *input, *input + 1, shell) < 0)
            return (-1);
        (*input)++;
        return (1);
    }
    return (0);
}

static int	handle_quotes(char **input, bool *in_quote, char *quote_char)
{
    if (!*in_quote && (**input == '\'' || **input == '\"'))
    {
        *in_quote = true;
        *quote_char = **input;
        (*input)++;
        return (1);
    }
    else if (*in_quote && **input == *quote_char)
    {
        *in_quote = false;
        *quote_char = '\0';
        (*input)++;
        return (1);
    }
    return (0);
}

char	**lexer(char *input, t_shell *shell)
{
    char	**tokens;
    t_lexer	lx;

    lx = (t_lexer){0};
    tokens = ft_calloc(MAX_TOKENS, sizeof(char *));
    if (!tokens || !input)
        return (free(tokens), NULL);
    lx.start = input;
    while (*input && lx.tokcount < MAX_TOKENS - 1)
    {
        if (handle_quotes(&input, &lx.in_quote, &lx.quote_char))
            continue ;
        if (!lx.in_quote && (ft_isspace(*input) || is_special_char(*input)))
        {
            if (lx.start != input && add_token(tokens, &lx.tokcount, lx.start,
                    input, shell) < 0)
                return (free_tokens(tokens), NULL);
            if (process_special_char(tokens, &lx.tokcount, &input, shell) < 0)
                return (free_tokens(tokens), NULL);
            lx.start = input + 1;
        }
        input++;
    }
    return (end_check(lx, input, tokens, shell));
}
