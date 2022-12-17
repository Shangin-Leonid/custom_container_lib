#================================================
# NAMING FORMAT
#================================================

T_PREFIX	:=
#LOG_PREFIX	:= $(T_PREFIX)_log
EXE_EXT		:= out

#================================================
# DIRECTORIES
#================================================

CONTAINER_DIR         := ../../containers
T_HEADER_DIR          := ./header
T_SRC_DIR             := ./src
T_OBJ_DIR             := ./obj
T_EXE_DIR             := ./exe
T_REPORT_DIR		  := ./report
T_LOG_DIR             := $(T_REPORT_DIR)/$(T_PREFIX)logs
#T_COVERAGE_DIR 	  := $(T_REPORT_DIR)/coverage

#================================================
# FILES
#================================================

HELP_FILE   		:= ../make_help.txt
MAKE_PARAMS_FILE    := ../make_params.txt
CONTAINER_FILE       = $(CONTAINER_DIR)/$(CONTAINER).hpp
#T_OBJ_FILES          = $(patsubst %,$(T_OBJ_DIR)/$(T_PREFIX)%.o,$(strip $(T_SECTIONS)))
REPORT_RESULT_FILE  := $(T_REPORT_DIR)/test_result.txt
#VALGRIND_LOG_FILE    = $(T_LOG_DIR)/vg_log_$(CONTAINER).txt
#VALGRIND_FILE  		 = $(T_REPORT_DIR)/vg_$(TC_CONTAINER_NAME).txt

#================================================
# FLAGS FOR BUILDING UNIT TESTS
#================================================

COMPILER        ?= g++
MAX_ERRORS      ?= 5
MAX_ERRORS_FLAG ?= -fmax-errors=$(MAX_ERRORS)
COMPILE_FLAGS   ?= -g -Wall -Wextra -Werror -pedantic-errors -std=c++11 $(MAX_ERRORS_FLAG) $(OPTIMIZATION)
OPTIMIZATION    ?= -O0
#VALGRIND_FLAG   ?= valgrind --leak-check=yes --log-file="$(VALGRIND_LOG_FILE)"
