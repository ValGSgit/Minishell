/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/02 15:55:39 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Select a random HERE_TEMP name from the list.
 */
// static const char *get_random_temp_name(void)
// {
// 	static unsigned int counter;
// 	unsigned int num;
// 	static char temp_name[32];
// 	char *ptr;
	
// 	ptr = temp_name;
// 	counter = 0;
// 	counter = (counter + 1) % HERE_DOC_MAX;
// 	*ptr++ = 'H';
// 	*ptr++ = 'T';
// 	num = counter + 1;
// 	if (num >= 10)
// 		*ptr++ = '0' + (num / 10);
// 	*ptr++ = '0' + (num % 10);
// 	*ptr = '\0';
// 	return (temp_name);
// }
static const char *get_random_temp_name(void)
{
    static const char *here_temp_list[HERE_DOC_MAX] = {
        HT1, HT2, HT3, HT4, HT5, HT6, HT7, HT8, HT9, HT10,
        HT11, HT12, HT13, HT14, HT15, HT16, HT17, HT18, HT19, HT20,
        HT21, HT22, HT23, HT24, HT25, HT26, HT27, HT28, HT29, HT30,
        HT31, HT32, HT33, HT34, HT35, HT36, HT37, HT38, HT39, HT40,
        HT41, HT42, HT43, HT44, HT45, HT46, HT47, HT48, HT49, HT50,
        HT51, HT52, HT53, HT54, HT55, HT56, HT57, HT58, HT59, HT60,
        HT61, HT62, HT63, HT64, HT65, HT66, HT67, HT68, HT69, HT70,
        HT71, HT72, HT73, HT74, HT75, HT76, HT77, HT78, HT79, HT80,
        HT81, HT82, HT83, HT84, HT85, HT86, HT87, HT88, HT89, HT90,
        HT91, HT92, HT93, HT94, HT95, HT96, HT97, HT98, HT99, HT100,
        HT101, HT102, HT103, HT104, HT105, HT106, HT107, HT108, HT109, HT110,
        HT111, HT112, HT113, HT114, HT115, HT116, HT117, HT118, HT119, HT120,
        HT121, HT122, HT123, HT124, HT125, HT126, HT127, HT128, HT129, HT130,
        HT131, HT132, HT133, HT134, HT135, HT136, HT137, HT138, HT139, HT140,
        HT141, HT142, HT143, HT144, HT145, HT146, HT147, HT148, HT149, HT150,
        HT151, HT152, HT153, HT154, HT155, HT156, HT157, HT158, HT159, HT160,
        HT161, HT162, HT163, HT164, HT165, HT166, HT167, HT168, HT169, HT170,
        HT171, HT172, HT173, HT174, HT175, HT176, HT177, HT178, HT179, HT180,
        HT181, HT182, HT183, HT184, HT185, HT186, HT187, HT188, HT189, HT190,
        HT191, HT192, HT193, HT194, HT195, HT196, HT197, HT198, HT199, HT200,
        HT201, HT202, HT203, HT204, HT205, HT206, HT207, HT208, HT209, HT210,
        HT211, HT212, HT213, HT214, HT215, HT216, HT217, HT218, HT219, HT220,
        HT221, HT222, HT223, HT224, HT225, HT226, HT227, HT228, HT229, HT230,
        HT231, HT232, HT233, HT234, HT235, HT236, HT237, HT238, HT239, HT240,
        HT241, HT242, HT243, HT244, HT245, HT246, HT247, HT248, HT249, HT250,
        HT251, HT252, HT253, HT254, HT255
    };

	static unsigned int counter = 0;
	unsigned int random_index = counter % HERE_DOC_MAX;
	counter++;
	return here_temp_list[random_index];
}

static char *process_line(char *arg, t_cmd *cmd)
{
	char *expanded_line = process_argument(arg, cmd->shell);
	if (expanded_line && ft_strcmp(expanded_line, arg) != 0)
	{
		free(arg);
		return (expanded_line);
	}
	return arg;
}

/**
 * Read heredoc input and write it to the temporary file.
 */
static void read_heredoc_input(const char *delimiter, int fd, t_cmd *shell)
{
    char *line;
    char *expanded_line;

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        expanded_line = process_line(line, shell);
        if (expanded_line)
        {
            write(fd, expanded_line, ft_strlen(expanded_line));
            write(fd, "\n", 1);
            free(expanded_line);
        }
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
			free(line);
        }
    }
}

/**
 * Handle heredoc redirection.
 */
void handle_heredoc(t_cmd *cmd, char *eof)
{
    const char *temp_name = get_random_temp_name();
	printf("Temporary file name: %s\n", temp_name);
    int fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("heredoc");
        return;
    }
    read_heredoc_input(eof, fd, cmd);
    close(fd);
    // Create the redirection node after handling the heredoc
	create_redir_node(cmd, REDIR_HEREDOC, (char *)temp_name);
}