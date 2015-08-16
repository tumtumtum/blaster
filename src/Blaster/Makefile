UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	LIBS := -luv -lboost_regex-mt -lboost_filesystem-mt -lboost_container-mt -lboost_program_options-mt -lboost_system-mt
endif

ifneq ($(UNAME), Darwin)
	LIBS := -luv -L/opt/lib -lboost_regex -lboost_filesystem -lboost_container -lboost_program_options -lboost_system
endif

SRC_FILES := $(wildcard *.cpp) $(wildcard *.c)

OBJ_FILES := $(SRC_FILES:.cpp=.o)
OBJ_FILES := $(OBJ_FILES:.c=.o)

blaster: ${OBJ_FILES}
	c++ -I/opt/include -o blaster -std=c++11 ${LIBS} ${OBJ_FILES}
		
%.o: %.cpp
	c++ -I/opt/include -std=c++11 -c -o $@ $<

clean:
	rm -f *.o blaster
