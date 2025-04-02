CC=g++
CC_FLAGS=-c -std=c++17 -Wall -Werror -Wextra
TEST_FLAGS=-lgtest -lsubunit -lrt -lm -lpthread

test:
	${CC} tests/*.cpp ${TEST_FLAGS} -o tests/tests
	valgrind ./tests/tests

gcov_report:
	${CC} -std=c++17 --coverage tests/*.cpp ${TEST_FLAGS} -o tests/test_report
	./tests/test_report
	lcov --directory . --capture -o coverage.info --ignore-errors mismatch
# lcov --directory . --capture -o coverage.info
	genhtml --output-directory report --legend coverage.info 
	-rm ./tests/*.gcda
	-rm ./tests/*.gcno
	-rm coverage.info

clean:
	-rm tests/tests
	-rm tests/test_report
	-rm -r report

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i AVL/*.tpp AVL/*.h
	clang-format -i map/*.tpp map/*.h
	clang-format -i set/*.tpp set/*.h
	clang-format -i list/*.tpp list/*.h
	clang-format -i queue/*.tpp queue/*.h
	clang-format -i stack/*.tpp stack/*.h
	clang-format -i vector/*.tpp vector/*.h
	clang-format -i array/*.tpp array/*.h
	clang-format -i multiset/*.tpp multiset/*.h
	clang-format -i tests/*.cpp
	clang-format -n AVL/*.tpp AVL/*.h
	clang-format -n map/*.tpp map/*.h
	clang-format -n set/*.tpp set/*.h
	clang-format -n multiset/*.tpp multiset/*.h
	clang-format -n list/*.tpp list/*.h
	clang-format -n queue/*.tpp queue/*.h
	clang-format -n stack/*.tpp stack/*.h
	clang-format -n vector/*.tpp vector/*.h
	clang-format -n array/*.tpp array/*.h
	clang-format -n tests/*.cpp
	rm .clang-format