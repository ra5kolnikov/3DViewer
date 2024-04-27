CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVR_FLAGS = --coverage -fprofile-arcs -ftest-coverage -O0 -fPIC
CHECK_FLAGS = -lcheck
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

OS = $(shell uname -s)
OPEN = open
ifeq ($(OS), Linux)
	CHECK_FLAGS += -lm -lpthread -lrt -lsubunit
	OPEN = xdg-open
endif


PROJECT_NAME = 3DViewerSurvivor
APP_NAME = $(PROJECT_NAME)

ifeq ($(OS), Darwin)
	APP_NAME = $(PROJECT_NAME).app
endif
QMAKE = qmake
QT_INSTALL_PREFIX = $(shell $(QMAKE) -query QT_INSTALL_PREFIX)

SRC_DIR = ./$(PROJECT_NAME)

OBJ_DIR = ./objs

TEST_DIR = ./tests
REPORT_DIR = ./report
BUILD_DIR = ../build

SOURCE_C = $(wildcard $(SRC_DIR)/s21_*.c)
TESTS_SOURCE_CHECK = $(wildcard $(TEST_DIR)/test_s21_*.check)
TESTS_SOURCE_C = $(TEST_DIR)/tests.c

TEST_OBJECTS = $(addprefix $(TEST_DIR)/, $(notdir $(SOURCE_C:%.c=%.o)))
TEST_OBJ = $(TEST_DIR)/tests



APP_DIR = ../app
DIST_DIR = ../dist
LIBRARY = $(PROJECT_NAME).a


RUN = $(APP_DIR)/$(APP_NAME)
ifeq ($(OS), Darwin)
	RUN = open $(APP_DIR)/$(APP_NAME)
endif

OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE_C:%.c=%.o)))




default: all

all: build

build: clean 
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_PREFIX_PATH=$(QT_INSTALL_PREFIX) -S $(SRC_DIR) -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

install: build
	mkdir -p $(APP_DIR)
	cp -r $(BUILD_DIR)/$(APP_NAME) $(APP_DIR)/$(APP_NAME)
	chmod +x $(APP_DIR)/$(APP_NAME)

uninstall:
	rm -rf $(APP_DIR)

rebuild: clean all

run: 
	$(RUN)

dvi:
	mkdir -p $(APP_DIR)
	#sudo apt install -y texinfo texlive xpdf
	texi2dvi $(SRC_DIR)/manual.tex -c -o $(APP_DIR)/manual.dvi
	#xdvi $(APP_DIR)/manual.dvi
	texi2pdf $(SRC_DIR)/manual.tex -c -o $(APP_DIR)/manual.pdf
	#xpdf $(APP_DIR)/manual.pdf

dist:
	mkdir -p $(DIST_DIR)
	tar -cvf $(DIST_DIR)/$(PROJECT_NAME).tar.gz $(SRC_DIR)/*


format_style:
	clang-format -i --style=google $(SRC_DIR)/*.c
	clang-format -i --style=google $(SRC_DIR)/*.cpp
	clang-format -i --style=google $(SRC_DIR)/*.h
	clang-format -i --style=google $(SRC_DIR)/QtGifImage/giflib/*.c
	clang-format -i --style=google $(SRC_DIR)/QtGifImage/giflib/*.h
	clang-format -i --style=google $(SRC_DIR)/QtGifImage/gifimage/*.cpp
	clang-format -i --style=google $(SRC_DIR)/QtGifImage/gifimage/*.h

check_style:
	clang-format -n --style=google $(SRC_DIR)/*.c
	clang-format -n --style=google $(SRC_DIR)/*.cpp
	clang-format -n --style=google $(SRC_DIR)/*.h
	clang-format -n --style=google $(SRC_DIR)/QtGifImage/giflib/*.c
	clang-format -n --style=google $(SRC_DIR)/QtGifImage/giflib/*.h
	clang-format -n --style=google $(SRC_DIR)/QtGifImage/gifimage/*.cpp
	clang-format -n --style=google $(SRC_DIR)/QtGifImage/gifimage/*.h

clean:
	rm -rf $(BUILD_DIR)
	rm -rf ./build*
	rm -rf $(SRC_DIR)/CMakeLists.txt.user
	rm -rf $(LIBRARY) $(OBJ_DIR) *.txt
	rm -rf $(TEST_DIR)/$(LIBRARY) $(TEST_DIR)/*.o
	rm -rf $(TESTS_SOURCE_C) $(TEST_OBJ)
	rm -rf $(TEST_DIR)/*.gcno $(TEST_DIR)/*.info  $(TEST_DIR)/*.gcda
	rm -rf $(TEST_DIR)/test_cov
	rm -rf $(REPORT_DIR)
	rm -rf $(APP_DIR)
	rm -rf $(DIST_DIR)


test: $(TEST_OBJ)
	$(TEST_OBJ)

valgrind: test
	valgrind $(VALGRIND_FLAGS) --log-file=valgrind_report.txt $(TEST_OBJ)

leaks: test
	leaks -atExit -- $(TEST_OBJ) > leaks_report.txt

gcov_report: $(TEST_OBJECTS) $(TESTS_SOURCE_C:%.c=%.o)
	mkdir -p $(REPORT_DIR)
	$(CC) $(TEST_OBJECTS) $(TESTS_SOURCE_C:%.c=%.o) $(GCOVR_FLAGS) $(CHECK_FLAGS) -o $(TEST_DIR)/test_cov
	$(TEST_DIR)/test_cov
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage-report.html
	$(OPEN) $(REPORT_DIR)/coverage-report.html  


$(TESTS_SOURCE_C): $(TESTS_SOURCE_CHECK) 
	checkmk clean_mode=1 $(TEST_DIR)/includes.check $^ $(TEST_DIR)/pre_main.check > $@

$(TESTS_SOURCE_C:%.c=%.o): $(TESTS_SOURCE_C) 
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(LIBRARY) $(TESTS_SOURCE_C:%.c=%.o)
	cp $(LIBRARY) $(TEST_DIR)
	$(CC) $(TESTS_SOURCE_C:%.c=%.o) $(TEST_DIR)/$(LIBRARY) $(CHECK_FLAGS) -o $@

$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARY) $(OBJECTS)
	ranlib $(LIBRARY)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOVR_FLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOVR_FLAGS) -c $< -o $@


.PHONY: all clean test gcov_report default check_style format_style valgrind leaks build install uninstall rebuild run dvi dist 
