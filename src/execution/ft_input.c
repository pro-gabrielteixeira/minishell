/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gateixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 16:32:19 by gateixei          #+#    #+#             */
/*   Updated: 2023/07/19 20:43:28 by gateixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_input(void)
{
	int	tmp_cmd;

	tmp_cmd = ft_input_check((data()->curr_cmd + 1));
	if (data()->fd[data()->curr_fd][0] < 0)
	{
		while (data()->spc[tmp_cmd] != -1 \
		&& data()->tokens[data()->spc[data()->curr_cmd]][0] == '<' \
		&& data()->tokens[data()->spc[tmp_cmd - 1]][0] == '<')
			tmp_cmd++;
		data()->curr_cmd = tmp_cmd;
		return ;   
	}
	if (data()->spc && data()->spc[data()->curr_cmd] != -1 \
	&& data()->spc[tmp_cmd] != -1)
	{
		if (is_redirect(data()->tokens[data()->spc[tmp_cmd]]) == 1 \
		|| is_redirect(data()->tokens[data()->spc[tmp_cmd]]) == 2)
		{
			ft_red_input(tmp_cmd);
			return ;
		}
		else
		{
			ft_exec_pipe_md();
			tmp_cmd++;
		}
	}
	else
		ft_exec_pipe_end();
	if (data()->cmds[tmp_cmd] && is_builtins(data()->cmds[tmp_cmd][0]) \
	&& !(is_builtins(data()->cmds[data()->curr_cmd - 1][0])) && data()->error == 0)
		builtins_error(NULL, NULL, "Broken pipe", 0);
	data()->curr_cmd = tmp_cmd;
	unlink(TEMP_FILE);
}

int	handle_here(char *exit)
{
	int		fd;
	char	*str;
	char	*output;

	fd = 0;
	output = NULL;
	str = readline("> ");
	while (ft_strcpm(str, exit) == 0)
	{
		output = strjoin_here(output, str);
		str = readline("> ");
	}
	free(str);
	fd = open(TEMP_FILE, O_WRONLY | O_CREAT | O_ASYNC | O_TRUNC, 0644);
	ft_putstr_fd(output, fd);
	fd = open(TEMP_FILE, O_RDONLY | O_ASYNC);
	free(output);
	return (fd);
}

void	ft_red_input(int tmp_cmd)
{
	tmp_cmd++;
	tmp_cmd = ft_red_input_check(tmp_cmd);
	if (!(access(data()->cmds[tmp_cmd][0], W_OK) == 0))
	{
		if (errno == EACCES)
		{    
			builtins_error(NULL, data()->cmds[tmp_cmd][0], ": Permission denied", 1);
			data()->fd[data()->curr_fd][1] = -1;
			while (data()->spc[tmp_cmd] != -1 && data()->tokens[data()->spc[tmp_cmd]] 
			&& data()->tokens[data()->spc[data()->curr_cmd]][0] == '>' 
			&& data()->tokens[data()->spc[tmp_cmd - 1]][0] == '>')
				tmp_cmd++;
			data()->curr_cmd = tmp_cmd;
			return ;
		}
	}
	if (is_redirect(data()->tokens[data()->spc[tmp_cmd - 1]]) == 1)
		(data()->fd[data()->curr_fd + 1][1]) = open(data()->cmds[tmp_cmd][0], \
		O_RDWR | O_CREAT | O_TRUNC, 0664);
	else if (is_redirect(data()->tokens[data()->spc[tmp_cmd - 1]]) == 2)
		(data()->fd[data()->curr_fd + 1][1]) = open(data()->cmds[tmp_cmd][0], \
		O_RDWR | O_CREAT | O_APPEND, 0664);
	if (data()->fd[data()->curr_fd][1] < 0)
		printf("Error to create/read the redirected file named: %s\n", \
		data()->cmds[tmp_cmd][0]);
	ft_exec_pipe_md();
	data()->curr_cmd = tmp_cmd;
}
