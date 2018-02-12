#IS_ICPC:= $(shell command -v icpc 2> /dev/null)

ifdef IS_ICPC
	CXX:=icpc
	CXXFLAGS:= -O3 -std=c++11 -xAVX -qopenmp -funroll-loops -ggdb -pg
else
	CXX:=g++
	CXXFLAGS:= -O0 -std=c++11 -fopenmp -mtune=native
endif

LD_LIB_FLAGS:= -L./ -lz
LD_INC_FLAGS:= -I./

test_kseq_buff: test_kseq_buff.cpp libksr.a
	$(CXX) $(CXXFLAGS) -o $@ $< $(LD_INC_FLAGS) $(LD_LIB_FLAGS) -lksr

libksr.a: kseq_reader.o kseq_reader.cpp kseq_reader.hpp Makefile
	ar -rs $@ $<

kseq_reader.o: kseq_reader.cpp kseq_reader.hpp kseq.hpp Makefile
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LD_INC_FLAGS) $(LD_LIB_FLAGS)

.PHONY: clean clobber

clean:
	$(RM) *.o

