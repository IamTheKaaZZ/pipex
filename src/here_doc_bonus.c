/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 11:41:19 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/20 13:52:42 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

/*
**	Check the return value of GNL and add the lines to readstr (WITH '\n')
**	1.	If there is an error or we have reached EOF before reading the LIMITER
**		=> Case: Error => Input error
**		=> Case: EOF before LIMITER
**	2.	No error
**		=> Add the line to readstr WITH a '\n' (GNL removes it)
**		=> free the line
*/

static void	check_retval(t_pipex *p, int retval, char **line, char **readstr)
{
	char	*buffer;
	char	*join;

	if (retval == ERROR || retval == 0)
	{
		if (*line)
			ft_strdel(line);
		if (*readstr)
			ft_strdel(readstr);
		if (retval == ERROR)
			program_errors(p, "INPUT READING", TRUE);
		else
			program_errors(p, "EOF BEFORE LIMITER", TRUE);
	}
	else if (retval == 1)
	{
		buffer = *readstr;
		join = ft_strjoin(*readstr, *line);
		*readstr = ft_strjoin_char(join, '\n');
		ft_strdel(&buffer);
		ft_strdel(&join);
		ft_strdel(line);
	}
}

/*
**	1.	Open the temporary file/inputstream in READ_ONLY mode
**		=>	Check for errors
**	2.	Call pipe_mode since it's the same situation now as MANDATORY
**	3.	We use unlink to destroy the temporary file/inputstream and close its fd
**		=>	Check for errors
*/

static void	open_pipe_unlink(t_pipex *p, char *inputstream)
{
	p->fd_input = open(inputstream, O_RDONLY);
	if (p->fd_input == ERROR)
		usage_error(p, "OPENING INPUTSTREAM", TRUE);
	pipe_mode(p);
	if (unlink(inputstream) == ERROR)
		usage_error(p, "UNLINKING INPUTSTREAM", TRUE);
}

/*
**	What is a here_doc? => https://en.wikipedia.org/wiki/Here_document
**	In this BONUS part we emulate the behaviour of a Here Document
**	=> In a shell script a heredoc can supply a command with the necessary data
**	=> That command keeps reading until it finds the LIMITER, then it stops
**	=> CLASSIC case: "<< cat EOF" => keep reading until you find ONLY "EOF"
**	=> Extra part of the bonus is ">>" AKA appending to an existing file
**
**	1.	We read lines from STDIN_FILENO (0) until we find the LIMITER
**	=>	 Janky hack incoming:
**		We create a temporary file to hold the data: "inputstream.txt"
**	2.	Read the first line and check it and retval before going into a loop
**		=> Add it to readstr
**	3.	WHILE (GNL != 0 or -1)
**		3.1	Read the next line
**		3.2	Check if it is the LIMITER => STOP if it is and don't save that line
**		3.3	Check the retval of GNL => add it to readstr if it's OK
**	4.	If no errors occurred and the LIMITER was found
**		=> write readstr into the temporary file
**		=> close its fd
**		=> free the allocated memory
**	5.	Call open_pipe_unlink
*/

void	here_doc_mode(t_pipex *p)
{
	char	*line;
	char	*readstr;
	int		size;
	int		retval;

	line = NULL;
	readstr = NULL;
	retval = get_next_line(STDIN_FILENO, &line);
	check_retval(p, retval, &line, &readstr);
	while (retval)
	{
		retval = get_next_line(STDIN_FILENO, &line);
		if (ft_strequal(line, p->limiter))
		{
			if (*line)
				ft_strdel(&line);
			break ;
		}
		check_retval(p, retval, &line, &readstr);
	}
	size = ft_strlen(readstr);
	write(p->fd_input, readstr, size);
	close(p->fd_input);
	ft_strdel(&readstr);
	open_pipe_unlink(p, "inputstream.txt");
}
