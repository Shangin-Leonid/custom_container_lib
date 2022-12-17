#================================================
# BUILD AND RUN COMPILING TEST
#================================================


#------------------------------------------------
# Names
#------------------------------------------------

TC_NAME          	= $(T_PREFIX)$(TC_LIB)_$(TC_CONTAINER_NAME)/compiling
TC_FILE     		= $(T_SRC_DIR)/$(TC_NAME).cpp
TC_EXE_FILE 		= $(T_EXE_DIR)/$(TC_NAME).$(EXE_EXT)
TC_LOG_FILE 		= $(T_LOG_DIR)/$(TC_LIB)_compiling_report.txt
TC_REPORT_FILE 		= $(T_REPORT_DIR)/$(TC_LIB)_compiling_report.txt

TC_COMPILE_FLAGS   := -ftime-report -Wall -Wextra -Werror -pedantic-errors -std=c++11 -O0 $(MAX_ERRORS_FLAG)
TC_RUN      		= $(TC_EXE_FILE)


#------------------------------------------------
# "Functions"
#------------------------------------------------

define _formating_compiling_results
@echo ""
@echo "Formating results:"
@echo "" >> $(TC_REPORT_FILE)
@echo "                         '$(TC_LIB)::$(TC_CONTAINER_NAME)' :" >> $(TC_REPORT_FILE)
@echo "" >> $(TC_REPORT_FILE)
@egrep -s -w 'Time variable|TOTAL' $(TC_LOG_FILE) |	\
        sed 's/Time variable/Category/g' |              	\
        sed 's/ TOTAL/Time/g' |                         	\
        sed 's/ *:/    /g' |                            	\
        sed 's/ *usr/    usr/g' >> $(TC_REPORT_FILE)
@echo "   ... SUCCESSFULLY."
endef

define _do_compiling_test
@echo ""
@echo ""
@echo "Compile testing programm for '$(TC_LIB)::$(TC_CONTAINER_NAME)' (messages are hiden/removed in '$(TC_LOG_FILE)' ):"
$(COMPILER) $(TC_COMPILE_FLAGS) $(TC_FILE) -o $(TC_EXE_FILE) 2> $(TC_LOG_FILE) 1>&2
@echo "Here"
$(_formating_compiling_results)
@echo ""
@echo "Run testing programm:"
$(TC_RUN) >> $(TC_REPORT_FILE)
@echo "   ... SUCCESSFULLY."
endef

define _show_compiling_test_report
@cat $(TC_REPORT_FILE)
endef



#------------------------------------------------
# Goals
#------------------------------------------------


_create_dirs_for_compiling_test:
	@mkdir -p $(T_EXE_DIR)/std_forward_list $(T_EXE_DIR)/ctl_cyclic_list $(T_REPORT_DIR) $(T_LOG_DIR)


_do_forward_list_compiling_test:
	$(eval TC_LIB 	:= std)
	$(eval TC_CONTAINER_NAME 	:= forward_list)
	$(_do_compiling_test)


_do_cyclic_list_compiling_test:
	$(eval TC_LIB 	:= ctl)
	$(eval TC_CONTAINER_NAME 	:= cyclic_list)
	$(_do_compiling_test)


_show_compiling_test_reports:
	@echo ""
	@echo ""
	@echo "==================== RESULTS OF COMPILING TEST ===================="
	$(eval TC_LIB 	:= std)
	$(eval TC_CONTAINER_NAME 	:= forward_list)
	$(_show_compiling_test_report)
	$(eval TC_LIB 	:= ctl)
	$(eval TC_CONTAINER_NAME 	:= cyclic_list)
	$(_show_compiling_test_report)
	@echo "==================================================================="
	@echo ""
	@echo ""


_clean_compiling_test:
	$(eval TC_LIB 	:= std)
	$(eval TC_CONTAINER_NAME 	:= forward_list)
	@rm -f $(TC_LOG_FILE) $(TC_EXE_FILE)
	$(eval TC_LIB 	:= ctl)
	$(eval TC_CONTAINER_NAME 	:= cyclic_list)
	@rm -f $(TC_LOG_FILE) $(TC_EXE_FILE)
	@echo ""
	@echo "Temporary files were successfully deleted."
	@echo ""


_clean_all_compiling_test:
	@echo ""
	$(eval TC_LIB 	:= std)
	$(eval TC_CONTAINER_NAME 	:= forward_list)
	@rm -f $(TC_REPORT_FILE) $(TC_LOG_FILE) $(TC_EXE_FILE)
	$(eval TC_LIB 	:= ctl)
	$(eval TC_CONTAINER_NAME 	:= cyclic_list)
	@rm -f $(TC_REPORT_FILE) $(TC_LOG_FILE) $(TC_EXE_FILE)
	@echo "All files created by compiling test were successfully deleted."
	@echo ""
