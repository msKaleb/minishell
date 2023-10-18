# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 09:13:49 by aperez-m     +#+  #+#    #+#              #
#    Updated: 2023/10/18 09:05:13 by msoria-j         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS := $(shell uname -s)

NAME := minishell

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#
# LIBS        libraries to be used
# LIBS_TARGET libraries to be built
#
# INCS        header file locations
#
# SRC_DIR     source directory
# SRCS        source files
#
# BUILD_DIR   build directory
# OBJS        object files
# DEPS        dependency files
#
# CC          compiler
# CFLAGS      compiler flags
# CPPFLAGS    preprocessor flags
# LDFLAGS     linker flags
# LDLIBS      libraries name

LIBS_TARGET := lib/libft/libft.a 

INCS       		:= include \
				lib/libft/include
LIBS			:= ft readline
INCS			:= $(INCS) /usr/include
LIBS_TARGET 	:= $(LIBS_TARGET)

SRC_DIR     	:= src
SRCS			:= \
				utils.c \
				main.c \
				minishell.c \
				lexer/lexer.c \
				parser/parser.c \
				parser/parser_functions_2.c\

SRCS			:= $(SRCS:%=$(SRC_DIR)/%)

SRCS_BONUS		:= \
				utils_bonus.c \
				main_bonus.c \
				minishell_bonus.c\
				lexer/lexer_bonus.c \
				parser/parser_bonus.c \
				parser/parser_bonus_functions.c \
				parser/parser_bonus_functions_2.c \
				executor/exec_bonus.c \
				
SRCS_BONUS		:= $(SRCS_BONUS:%=$(SRC_DIR)/%)

SRCS_COMMON		:= \
				env_list.c \
				errors.c \
				array_indexation.c \
				executor/exec_process.c \
				executor/executor_functions.c \
				executor/here_doc.c \
				executor/children_waiting.c \
				lexer/read_line.c \
				lexer/spaces_inserter.c \
				lexer/quote_handling.c \
				lexer/quote_handling_utils.c \
				lexer/braces_handling.c \
				lexer/ft_split_msh.c \
				expander/expander.c \
				expander/sh_par_expansion.c \
				expander/sh_par_expansion_parsing.c \
				expander/fn_expander.c \
				parser/parser_functions.c \
				parser/ft_split_args.c \
				parser/parser_common_functions.c \
				signal_handling.c \
				built-ins/cd.c \
				built-ins/echo.c \
				built-ins/env.c \
				built-ins/exit.c \
				built-ins/export.c \
				built-ins/export_no_args.c \
				built-ins/pwd.c \
				built-ins/unset.c

SRCS_COMMON		:= $(SRCS_COMMON:%=$(SRC_DIR)/%)

BUILD_DIR  	 	:= .obj
OBJS			:= $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJS_BONUS		:= $(SRCS_BONUS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJS_COMMON		:= $(SRCS_COMMON:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS			:= $(OBJS:.o=.d)
DEPS_BONUS		:= $(OBJS_BONUS:.o=.d)
DEPS_COMMON		:= $(OBJS_COMMON:.o=.d)

ifeq ($(OS), Linux)
	CC          := cc
	CFLAGS      := -Wall -Wextra -Werror -ggdb -O0
	CPPFLAGS    := $(addprefix -I,$(INCS)) -MMD -MP
	LDFLAGS     := $(addprefix -L,$(dir $(LIBS_TARGET)))
	LDLIBS      := $(addprefix -l,$(LIBS))
else ifeq ($(OS), Darwin)
	CC          := cc
	CFLAGS      := -Wall -Wextra -Werror -ggdb -O0
	CPPFLAGS    := -I/Users/$(USER)/.brew/opt/readline/include
	CPPFLAGS	+= $(addprefix -I,$(INCS)) -MMD -MP
	LDFLAGS     := -L/Users/$(USER)/.brew/opt/readline/lib
	LDFLAGS		+= $(addprefix -L,$(dir $(LIBS_TARGET)))
	LDLIBS      := -lreadline $(addprefix -l,$(LIBS))
endif

#------------------------------------------------#
#   UTENSILS                                     #
#------------------------------------------------#
# RM        force remove
# MAKEFLAGS make flags
# DIR_DUP   duplicate directory tree

RM          := rm -rf
MAKEFLAGS   += --silent --no-print-directory
DIR_DUP     = mkdir -p $(@D)

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
# all       default goal
# $(NAME)   link .o -> archive
# $(LIBS)   build libraries
# %.o       compilation .c -> .o
# clean     remove .o
# fclean    remove .o + binary
# re        remake default goal
# run       run the program
# info      print the default goal recipe

# Define colors for output
COLOR='\033['
NONE=$(COLOR)0m
GREEN=$(COLOR)32m
GRAY=$(COLOR)37m
RED=$(COLOR)31m
CYAN=$(COLOR)36m
MAGENTA=$(COLOR)35m
BLUE=$(COLOR)34m
ITALIC=$(COLOR)3m
BOLD=$(COLOR)1m
BRIGHT_WHITE=$(COLOR)97m

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_COMMON) $(LIBS_TARGET)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(OBJS_COMMON) $(LDLIBS) -o $(NAME)
	$(info CREATED $(NAME))
	$(RM) .bonus

bonus: .bonus

.bonus:	$(OBJS_BONUS) $(OBJS_COMMON) $(LIBS_TARGET)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS_BONUS) $(OBJS_COMMON) $(LDLIBS) -o $(NAME)
	$(info CREATED $(NAME) (Bonus))
	@touch .bonus
	$(RM) $(OBJS)

$(LIBS_TARGET):
	$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -ggdb -c -o $@ $<
	$(info CREATED $@)

-include $(DEPS)

clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(BUILD_DIR) .bonus
	
fclean: clean
	$(MAKE) fclean -C lib/libft
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

rebonus:
	$(MAKE) fclean
	$(MAKE) bonus

info-%:
	$(MAKE) --dry-run --always-make $* | grep -v "info"

#------------------------------------------------#
#   SPEC                                         #
#------------------------------------------------#

.PHONY: clean fclean re
.SILENT:
