#include "minishell.h"
#include <stdlib.h>

sig_atomic_t	g_error;

void	glob_engine_test(void)
{
	char *pattern = "casa*";
	char *match1 = "una casa";
	char *match2 = "casa";
	char *match3 = "casa bonita";
	char *match4 = "una casa bonita";
	char *match5 = "mierda";
	char *match6 = "una casa y una chica fea";

	printf(ANSI_RED"\n\nglob_engine_test\n"ANSI_RESET);
	printf("%s y %s -> %d\n", pattern, match1, glob_engine(pattern, match1));
	printf("%s y %s -> %d\n", pattern, match2, glob_engine(pattern, match2));
	printf("%s y %s -> %d\n", pattern, match3, glob_engine(pattern, match3));
	printf("%s y %s -> %d\n", pattern, match4, glob_engine(pattern, match4));
	printf("%s y %s -> %d\n", pattern, match5, glob_engine(pattern, match5));
	printf("%s y %s -> %d\n", pattern, match6, glob_engine(pattern, match6));
}

static char **char_pp_dup(char **src);
void	get_brace_state_i_test(void)
{
	char	*src[] = {"${HOME}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "{HOME}", \
					 "{${HOME ${NOME}",
					  NULL};
	int		i = -1;

	printf(ANSI_RED"\n\nget_brace_state_i\n"ANSI_RESET);
	while (src[++i])
		printf("source: %s -> %s\n", src[i], get_brace_state_i(src[i]));
}

void	free_list(void *list)
{
	free((t_list *)list);
}

void	get_env_var_value_test(char **envp)
{
	char	*src[] = {"${HOME\n}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "\"${HOME}\"", \
					 NULL};
	t_list	**lenv;
	size_t	pos = 0;
	int		i = -1;

	printf(ANSI_RED"\n\nget_env_var_value\n"ANSI_RESET);
	lenv = fill_penv_list(envp);
	while (src[++i])
	{
		pos = ft_strchr(src[i], '$') - src[i];
		printf("source: %s -> %s\n", src[i], get_env_var_value(src[i], pos, *lenv));
	}
	ft_lstiter(*lenv, &free_list);
}

void	get_env_var_name_test()
{
	char	*src[] = {"${HOME\n}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "\"${HOME}\"", \
					 "${a}", \
					 "$a", \
					 NULL};
	size_t	pos = 0;
	int		i = -1;

	printf(ANSI_RED"\n\nget_env_var_name\n"ANSI_RESET);
	while (src[++i])
	{
		pos = ft_strchr(src[i], '$') - src[i];
		printf("source: %s -> %s·\n", src[i], get_env_var_name(src[i], pos));
	}
}

void	get_var_end_test()
{
	char	*src[] = {"${HOME}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "\"${HOME}\"", \
					 "$a" ,\
					 "${a}", \
					 NULL};
	size_t	pos = 0;
	int		i = -1;

	printf(ANSI_RED"\n\nget_env_var_end\n"ANSI_RESET);
	while (src[++i])
	{
		pos = ft_strchr(src[i], '$') - src[i];
		printf("source: %s -> %c\n", src[i], *get_var_end(src[i], pos));
	}
}

void	get_var_start_test()
{
	char	*src[] = {"${HOME}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "\"${HOME}\"", \
					 NULL};
	size_t	pos = 0;
	int		i = -1;

	printf(ANSI_RED"\n\nget_env_var_start\n"ANSI_RESET);
	while (src[++i])
	{
		pos = ft_strchr(src[i], '$') - src[i];
		printf("source: %s -> %c\n", src[i], *get_var_start(src[i], pos));
	}

}

void	get_parameter_end_test()
{
	char	*src[] = {"${HOME}",\
					 "${HOME", \
					 "$HOME}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "\"${HOME}\"", \
					 NULL};
	size_t	pos = 0;
	int		i = -1;

	printf(ANSI_RED"\n\nget_parameter_end\n"ANSI_RESET);
	while (src[++i])
	{
		pos = ft_strchr(src[i], '$') - src[i];
		if (get_parameter_end(src[i], pos))	
			printf("source: %s -> %c\n", src[i], *get_parameter_end(src[i], pos));
		else
			printf("source: %s -> %s\n", src[i], get_parameter_end(src[i], pos));
	}

}

void	replace_string_chunk_test()
{
	char	*host;
	char	*insert[] = {NULL, \
						NULL,\
					 "\\home\\al"};
	size_t	pos[] = {3, 7, 14};
	int		i = 3;
	host = ft_strdup("uno$dos$\"HOME\"$HOME");

	printf(ANSI_RED"\n\nreplace_string_chunk\n"ANSI_RESET);
	while (--i >= 0)
	{
			printf("source: %s", host);
			host = replace_string_chunk(&host, &insert[i], pos[i]);
			printf(" -> %s\n", host);
	}
	free(host);
}


void	lexer_test(void)
{
	char	*src[] = {/*"${HOME}",\
					 "${HOME", \
					 "$HOME>}", \
					 "$\"{\"HOME", \
					 "${{HOME}", \
					 "echo ${|}", /
					 ">>>echo<<<<${|}|", \*/
					 "env | grep SHLVL", \
					 /*"<<a <<s <<d", \
					 "<<d <<ad <<af", \*/
					 NULL};
	char	*input;
	int		i = -1;
	//char	*temp;

	printf(ANSI_RED"\n\nlexer\n"ANSI_RESET);
	while (src[++i])
	{
		input = ft_strdup(src[i]);
		printf("|%s|", input);
		lexer(&input);
		printf(" -> |%s|\n", input);
		free(input);
	}
}

void	quote_remove_token_test()
{
	char	*src[] = {"\"\"", \
					"\"casa' \"", \
					"\"casa\"casa", \
					"'casa'casa", \
					"'ca\"sa'casa", \
					"\"ca'sa\"casa", \
					"\"ca'sacasa\"", \
					NULL};
	char	*input;
	int		i = -1;

	printf(ANSI_RED"\n\nquote_remove\n"ANSI_RESET);
	while (src[++i])
	{
		input = ft_strdup(src[i]);
		printf("%s ", input);
		quote_remove(input);
		printf(" -> %s\n", input);
		free(input);
	}
}

static void print_blue(void *msg)
{
	ft_fprintf(1, ANSI_BLUE"%s\n"ANSI_RESET, (char*)msg);
}

static void print_red(void *msg)
{
	ft_fprintf(1, ANSI_RED"%s\n"ANSI_RED, (char*)msg);
}

void	unset_test(char **penv)
{
	t_list	**lenv;
	char	*args[] = {"DUMMY", \
					"SHELL", \
					"LANGUAGE", \
					"CASA", \
					"COLORTERM", \
					NULL};
	char	**args_cpy;
	int		i = -1;
	
	printf(ANSI_RED"\n\nunset_test\n"ANSI_RESET);
	while (args[++i])
		;
	args_cpy= ft_calloc(i + 1, sizeof(char *));
	i = -1;
	while (args[++i])
		args_cpy[i] = ft_strdup(args[i]);
	lenv = fill_penv_list(penv);
	unset(args_cpy, lenv, &penv);
	i = -1;
	while(penv[++i])
		printf(ANSI_GREEN"penv: %s"ANSI_RESET"\n", penv[i]);
	ft_lstiter(*lenv, free_list);
	dbl_free(args_cpy);
}

void	remove_matched_nodes_test(char **penv)
{
	t_list **lenv;
	char	*arg;

	printf(ANSI_RED"\n\nremove_matched_nodes_test\n"ANSI_RESET);
	arg = ft_strdup("CASA");
	lenv = fill_penv_list(penv);
	ft_lstiter(*lenv, &print_blue);
	remove_matched_nodes(lenv, arg);
	ft_lstiter(*lenv, &print_red);
}
void ft_split_msh_test()
{
	char *src;
	char **ret;
	int	i = -1;

	printf(ANSI_RED"\n\nft_split_msh\n"ANSI_RESET);
	src = "echo ${|} ba";
	ret = ft_split_msh(src, ' ');
	while (ret[++i])
		ft_fprintf(1,"%s\n", ret[i]);
	dbl_free(ret);
}

void	export_test(char **penv)
{

	t_list	**lenv;
	char	*args[] = {"DUMMY", \
					"SHELL=concha", \
					"LANGUAGE=indio", \
					"CASA=notengo", \
					"COLORTERM=negro", \
					NULL};
	char	**args_cpy;
	char	*empty[] = {"\0", NULL};
	//int	i;
	
	printf(ANSI_RED"\n\nexport_test\n"ANSI_RESET);
	args_cpy = char_pp_dup(args);
	lenv = fill_penv_list(penv);
	export(args_cpy, *lenv, &penv, 0);
	export(empty, *lenv, &penv,0);
	//i = -1;
	//while(penv[++i])
		//printf(ANSI_GREEN"envp: %s"ANSI_RESET"\n", penv[i]);
	dbl_free(args_cpy);
	ft_lstiter(*lenv, free_list);
}

void	sort_list_test(char **envp)
{
	t_list **lenv;

	lenv = fill_penv_list(envp);
	sort_list(*lenv);
	ft_lstiter(*lenv, &print_blue);
}

void	get_env_test(char **envp)
{
	t_list	**lenv;
	char	*var_name = "HOME\n";

	lenv = fill_penv_list(envp);
	printf(ANSI_RED"\n\nget_env_test"ANSI_RESET"\n");
	ft_fprintf(2, "%s\n", get_env(*lenv, var_name));
}

void	valid_var_name_test(void)
{
	char	*var_name = "HOME ";

	printf(ANSI_RED"\n\nvalid_var_name\n"ANSI_RESET);
	printf("%d\n", valid_var_name(var_name));
}

static char **char_pp_dup(char **src)
{
	int		i;
	char	**ret;

	i = -1;
	while (src[++i])
		;
	ret= ft_calloc(i + 1, sizeof(char *));
	i = -1;
	while (src[++i])
		ret[i] = ft_strdup(src[i]);
	return (ret);
}

void	unpack_char_pp_test()
{
	char	**src;
	int		i = -1;

	src = ft_split("echo \"\"", ' ');
	src = unpack_char_pp(&src);
	while (src[++i])
		ft_fprintf(1, "%s\n", src[i]);
	dbl_free(src);
}

void 	ft_wordcount_test()
{
	printf("%d\n", ft_wordcount("echo casa'"));
}

void ft_split_args_test()
{
	char	*src;
	char	**ret;
	int		i;

	i = -1;
	src = "echo casa \"";
	ret = ft_split_args(src);
	while(ret[++i])
		printf("%d - %s\n", i, ret[i]);
	dbl_free(ret);
}

void	get_w_nbr_msh_test()
{
	char	*src;
	int		ret;
	char	*qsi;
	char	*bsi;

	qsi = NULL;
	src = "<<a <<s <<d";
	get_quote_state_i(src, &qsi);
	bsi = get_brace_state_i(src);
	ret = get_w_nbr_msh(src, ' ', qsi, bsi);
	printf(ANSI_RED"get_w_nbr_msh_test: %s, -> %d"ANSI_RESET"\n", src, ret);
	free(qsi);
	free(bsi);
}
void	get_spaces_to_insert_test(void)
{
	char	*src[] = {"<<a", \
					"<<", \
					"a<<", \
					"<<a<<", \
					"<<a<<", \
					NULL};
	int		pos[] = {0, 0, 1, 0, 3};
	
	char	*input;
	int		i = -1;

	printf(ANSI_RED"\n\nget_spaces_to_insert\n"ANSI_RESET);
	while (src[++i])
	{
		input = ft_strdup(src[i]);
		printf("%s -> %d\n", input, get_spaces_to_insert(input, pos[i], 2));
		free(input);
	}
}

void	insert_spaces_test()
{
	char	*src[] = {/*"<<a", \
					"<<", \
					"a<<", \
					"<<a<<",*/ \
					"<<a<<", \
					/* "|",*/ \
					NULL};
	int		pos[] = {0, \
					0, \
					1, \
					0, \
					3};
	
	char	*input;
	int		i = -1;

	printf(ANSI_RED"\n\ninsert_spaces_\n"ANSI_RESET);
	while (src[++i])
	{
		input = ft_strdup(src[i]);
		printf("%s -> ", input);
		insert_spaces(&input, pos[i], is_special(&input, pos[i]));
		printf("%s\n", input);
		free(input);
	}
}

int main (int argc, char **argv, char **penv)
{
	(void)argc;
	(void)argv;
	char	**penv_cpy;
	int		i = -1;

	while(penv[++i])
		;
	penv_cpy = ft_calloc(i + 1, sizeof(char *));
	i = -1;
	while(penv[++i])
		penv_cpy[i] = ft_strdup(penv[i]);
	//get_brace_state_i_test();
	//get_parameter_end_test();	
	//get_var_end_test();
	//get_var_start_test();
	//get_env_var_name_test();
	//get_env_var_value_test(penv);
	//get_env_test(penv);
	//valid_var_name_test();
	//replace_string_chunk_test();
	//glob_engine_test();
	lexer_test();
	//ft_split_msh_test();
	//quote_remove_token_test();
	//unset_test(penv_cpy);
	//export_test(penv_cpy);
	//remove_matched_nodes_test(penv);
	//sort_list_test(penv);
	//unpack_char_pp_test();
	//ft_wordcount_test();
	//ft_split_args_test();
	//if (argv[1])
	//	while (1)
	//		minishell(penv);
	//get_w_nbr_msh_test();
	//get_spaces_to_insert_test();
	//insert_spaces_test();
	dbl_free(penv_cpy);
}
