.DEFAULT_GOAL := generate
SHELL := /bin/bash

clean:
	@sudo rm -rf build/
	@sudo rm -rf install/
	@sudo rm -rf log/
	@sudo rm -f algo_trading/.clang-format

check_code:
	@printf "=================== START cppcheck ==================="
	@printf "\n\n"
	@cppcheck --platform=unix64 --quiet -ialgo_trading/ext algo_trading
	@printf "==================== END cppcheck ===================="

compile_code:
	@printf "==================== START  build ===================="
	@printf "\n\n"
	@mkdir -p build
	@cd build; cmake -L -DCMAKE_CXX_COMPILER=/usr/bin/g++-10 -DCMAKE_BUILD_TYPE=Debug ../algo_trading
	@$(MAKE) -C build all
	@printf "\n"
	@printf "===================== END  build ====================="

test_code:
	@printf "===================== START test ====================="
	@printf "\n\n"
#	@colcon test
	@printf "\n"
	@printf "====================== END test ======================"

generate:
	@cp -u essentials/.clang-format algo_trading/.clang-format
	@printf "\n"
	@$(MAKE) -s check_code
	@printf "\n"
	@$(MAKE) -s compile_code
	@printf "\n"
	@$(MAKE) -s test_code
	@printf "\n\n"

rebuild:
	@$(MAKE) -s clean
	@$(MAKE) -s generate
