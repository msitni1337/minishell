/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msitni <msitni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:46:13 by msitni            #+#    #+#             */
/*   Updated: 2024/06/04 14:12:19 by msitni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int write_next_line_here_doc(t_node *node, int write)
{
	char *line;
	char *delim;

	g_shell.collecting_here_doc = TRUE;
	delim = expand_string(node->token_str, FALSE);
	if (delim == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	line = readline("> ");
	while (line)
	{
		if (ft_strcmp(line, delim) == 0)
			break;
		ft_putendl_fd(line, write);
		free(line);
		line = readline("> ");
	}
	close(write);
	free(delim);
	if (line)
	{
		free(line);
		return (0);
	}
	return (1);
}

void get_here_doc_filename(char *filename)
{
	int rand_fd;
	int tries;

	rand_fd = open("/dev/random", O_RDONLY);
	if (rand_fd == -1)
	{
		perror("open");
		free(filename);
		exit_with_code(NULL, EXIT_FAILURE);
	}
	tries = randomize_str(filename + 25, rand_fd);
	while (tries < 100 && access(filename, F_OK) == 0)
		tries += randomize_str(filename + 25, rand_fd);
	if (access(filename, F_OK) == 0)
	{
		ft_putstr_fd(HERE_DOC_FILENAME_ERROR, STDERR_FILENO);
		free(filename);
		exit_with_code(NULL, EXIT_FAILURE);
	}
	close(rand_fd);
}

void init_here_doc(t_node *node, int *fd, int *stdin_dup)
{
	char *filename;

	filename = ft_strdup("/tmp/" PROG_NAME "_here_doc_XXXXXXXX");
	if (filename == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
	get_here_doc_filename(filename);
	*stdin_dup = dup(0);
	if (*stdin_dup == -1)
	{
		perror("dup");
		free(filename);
		exit_with_code(NULL, EXIT_FAILURE);
	}
	*fd = open(filename, O_RDWR | O_CREAT,
			   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	node->here_doc_fd = open(filename, O_RDWR);
	if (*fd == -1 || node->here_doc_fd == -1)
	{
		perror("open");
		free(filename);
		exit_with_code(NULL, EXIT_FAILURE);
	}
	free(filename);
}

void add_here_doc(t_node *node)
{
	if (g_shell.here_docs.count > 15)
	{
		print_error(NULL, "maximum here-document count exceeded");
		exit_with_code(NULL, 2);
	}
	if (add_to_arr(&(g_shell.here_docs), &node) == NULL)
		malloc_error(NULL, NULL, NULL, NULL);
}

int get_here_doc(t_node *node)
{
	int here_doc_fd;
	int stdin_dup;

	init_here_doc(node, &here_doc_fd, &stdin_dup);
	if (write_next_line_here_doc(node, here_doc_fd))
	{
		if (g_shell.interrupt == TRUE)
			return (handle_here_doc_interrupt(stdin_dup));
		syntax_error("expecting delim for here_doc.");
	}
	close(stdin_dup);
	g_shell.collecting_here_doc = FALSE;
	return (0);
}

t_node** get_here_docs(t_node**root)
{
	t_node** here_docs;
	size_t i;

	here_docs = g_shell.here_docs.data;
	i = 0;
	while (i < g_shell.here_docs.count)
	{
		if (get_here_doc(here_docs[i]))
			return NULL;
		i++;
	}
	g_shell.here_docs.count = 0;
	return root;
}