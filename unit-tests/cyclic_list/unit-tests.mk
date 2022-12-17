
include ../configs.mk

ALL_UNIT_TESTS_IN_CORRECT_ORDER := 	constructor			\
									operator_eq 		\
									iterator 			\
									assign 				\
									modifiers_adding 	\
									modifiers_deleting 	\
									modifiers_other 	\
									comparing
									


#------------------------------------------------
# "Functions"
#------------------------------------------------



define compile_test_section
echo "Compiling '$(TC_CONTAINER_NAME) $$TEST_SECTION_NAME' test section ..." ; \
$(COMPILER) $(COMPILE_FLAGS) $(T_SRC_DIR)/$(TC_CONTAINER_NAME)/$$TEST_SECTION_NAME.cpp -o $(T_EXE_DIR)/$(TC_CONTAINER_NAME)/$$TEST_SECTION_NAME.$(EXE_EXT) ; \
echo "      SUCCESSFULLY." ;
endef


define run_test_section
echo "Running '$(TC_CONTAINER_NAME) $$TEST_SECTION_NAME' test section ..." ; \
$(T_EXE_DIR)/$(TC_CONTAINER_NAME)/$$TEST_SECTION_NAME.$(EXE_EXT) $(T_LOG_DIR)/$(TC_CONTAINER_NAME)/$$TEST_SECTION_NAME.txt ; \
echo "      SUCCESSFULLY." ;
endef



#------------------------------------------------
# Goals
#------------------------------------------------



do_test_with_std: create_dirs
	@echo ""
	$(eval TC_CONTAINER_PREFIX 	:= std)
	$(eval TC_CONTAINER_NAME 	:= std_forward_list)
	@for TEST_SECTION_NAME in $(ALL_UNIT_TESTS_IN_CORRECT_ORDER) ; do \
		$(compile_test_section) \
		$(run_test_section) \
	done
	@echo ""

do_test_with_ctl: create_dirs
	@echo ""
	$(eval TC_CONTAINER_PREFIX 	:= ctl)
	$(eval TC_CONTAINER_NAME 	:= ctl_cyclic_list)
	@for TEST_SECTION_NAME in $(ALL_UNIT_TESTS_IN_CORRECT_ORDER) ; do \
		$(compile_test_section) \
		$(run_test_section) \
	done
	@echo ""

compare_logs:
	@echo ""
	@echo "Compiling '$(T_SRC_DIR)/compare_logs.cpp' ..."
	@g++ $(T_SRC_DIR)/compare_logs.cpp -o $(T_EXE_DIR)/compare_logs.$(EXE_EXT)
	@echo "      ... SUCCESSFULLY"
	@echo ""
	@echo "Comparing log files (results in '$(REPORT_RESULT_FILE)')..."
	@rm -f $(REPORT_RESULT_FILE)
	@for TEST_SECTION_NAME in $(ALL_UNIT_TESTS_IN_CORRECT_ORDER) ; do \
		echo "'$$TEST_SECTION_NAME': " >> $(REPORT_RESULT_FILE) ; \
		$(T_EXE_DIR)/compare_logs.$(EXE_EXT) $(T_LOG_DIR)/std_forward_list/$$TEST_SECTION_NAME.txt $(T_LOG_DIR)/ctl_cyclic_list/$$TEST_SECTION_NAME.txt >> $(REPORT_RESULT_FILE) ; \
		echo "" >> $(REPORT_RESULT_FILE) ; \
	done
	@echo "      ... SUCCESSFULLY"
	@echo ""
	@echo ""
	@echo "REPORT:"
	@echo ""
	@cat $(REPORT_RESULT_FILE)
	@echo ""
