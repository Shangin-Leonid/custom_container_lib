#================================================
# NAMES, DIRS AND OTHER CONFIGURATION INFO
#================================================

include ../configs.mk

CONTAINER   := cyclic_list
T_SECTIONS	:= constructor iterator operator_eq


#================================================
# MAIN GOALS
#================================================

.PHONY: 


help:
	cat $(HELP_FILE)
	@echo ""
	cat $(MAKE_PARAMS_FILE)

create_dirs:
	@echo ""
	@echo "Creating auxiliary dirs:"
	@mkdir -p $(T_EXE_DIR)/std_forward_list $(T_EXE_DIR)/ctl_cyclic_list $(T_REPORT_DIR) $(T_LOG_DIR)/std_forward_list $(T_LOG_DIR)/ctl_cyclic_list
	@echo "   ... SUCCESSFULLY."
	@echo ""

clean_all:
	@echo ""
	@echo "Deleting all directories, which were created during testing:"
	@rm -r -f $(T_EXE_DIR) $(T_REPORT_DIR) $(T_LOG_DIR)
	@echo "   ... SUCCESSFULLY."
	@echo ""

do_all_test: do_compiling_test do_test


#================================================
# BUILD AND RUN COMPILING TEST
#================================================
# do_compiling_test			- prepare, run test, show result and clean temporary files
# clean_all_compiling_test	- clean all files which were created during 'do_compiling_test'

include compiling_test.mk

do_compiling_test: 	_create_dirs_for_compiling_test \
					_do_forward_list_compiling_test \
					_do_cyclic_list_compiling_test	\
					_show_compiling_test_reports	\

clean_compiling_test: _clean_compiling_test

clean_all_compiling_test: _clean_all_compiling_test


#================================================
# UNIT_TESTS
#================================================

include unit-tests.mk

do_test: do_test_with_std do_test_with_ctl compare_logs

