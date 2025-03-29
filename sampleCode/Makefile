# Adapted from Paulo Pedreira's SETREXAMPLE_class
# Paulo Pedreiras, Mar/2022
# Adapted from the ThrowTheSwitch.org   
# 
# This makefile assumes that test source and test files are in folders 
#   "src" and "test", respectively
# Moreover, this makefile, "src" and "test" are within the root Unity 
#	folder.
# Finaly, the SW module name has a MODULE_NAME.c file and MODULE_NAME.h
# 	file, and the test file is testMODULENAME.c
# If the rules above are obeyed only MODULE_NAME needs to be adjusted

MODULE_CMDPROC = cmdproc
MODULE_SENSORES = sensores


# Paths
UNITY_ROOT = ../Unity
SRC_FOLDER = src
TEST_FOLDER = test

# Commands
CLEANUP = rm -f
MKDIR = mkdir -p
	
#Compiler
C_COMPILER = gcc
CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
#CFLAGS += -Wno-misleading-indentation

#includes
INC_DIRS=-I$(SRC_FOLDER) -I$(UNITY_ROOT)/src

#Unity framewoork
UNITY_SRC = $(UNITY_ROOT)/src/unity.c

#source files
SRC_CMDPROC = $(SRC_FOLDER)/$(MODULE_CMDPROC).c $(SRC_FOLDER)/$(MODULE_SENSORES).c
SRC_SENSORES = $(SRC_FOLDER)/$(MODULE_SENSORES).c

#test files
TEST_CMDPROC = $(TEST_FOLDER)/test_$(MODULE_CMDPROC).c
TEST_SENSORES = $(TEST_FOLDER)/test_$(MODULE_SENSORES).c

#targets
TARGET_CMDPROC = TestCmdProc
TARGET_SENSORES = TestSensores


#build all
SYMBOLS=

all: clean $(TARGET_SENSORES) $(TARGET_CMDPROC)
	./$(TARGET_SENSORES)
	./$(TARGET_CMDPROC)
	
# sensors test
$(TARGET_SENSORES): $(SRC_SENSORES) $(TEST_SENSORES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(UNITY_SRC) $(SRC_SENSORES) $(TEST_SENSORES) -o $(TARGET_SENSORES)

# cmdproc test (dependes on sensores.c)
$(TARGET_CMDPROC): $(SRC_CMDPROC) $(TEST_CMDPROC)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(UNITY_SRC) $(SRC_CMDPROC) $(TEST_CMDPROC) -o $(TARGET_CMDPROC)
	
#clean
.PHONY: clean

clean:
	$(CLEANUP) $(TARGET_CMDPROC) $(TARGET_SENSORES)
	


