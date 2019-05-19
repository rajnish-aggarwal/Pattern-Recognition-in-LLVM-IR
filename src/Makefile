all: main.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

Tensor.o: Tensor.cpp ../include/Tensor.h
Patterns.o: Patterns.cpp ../include/Patterns.h
InstFinder.o: InstFinder.cpp ../include/InstFinder.h

%.so: %.o Tensor.o InstFinder.o Patterns.o
	$(CXX) -dylib -shared $^ -o $@


listing1:
	make all
	opt -load ./main.so -inst-finder ../testdir/listing1-m2r.bc -o out


listing2:
	make all
	opt -load ./main.so -inst-finder ../testdir/listing2-m2r.bc -o out


listing3:
	make all
	opt -load ./main.so -inst-finder ../testdir/listing3-m2r.bc -o out

clean:
	rm -f *.o *~ *.so *.bc *.ll out
