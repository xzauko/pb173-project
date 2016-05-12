IDIR=include
CXX=clang++
CXXFLAGS= -I$(IDIR) -std=c++14

BDIR=bin
ODIR=tests/obj
LDIR=lib
TDIR=tests

_DEPS = fixedpoint.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS)) tests/catch.hpp

_OBJ = testConst.o testInOut.o testCompare.o testArith.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

default: testInOut testConst testCompare testArith testEval


$(ODIR)/%.o: $(TDIR)/%.cpp $(DEPS) | $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: | $(BDIR)

$(BDIR):
	mkdir $(BDIR)
$(ODIR):
	mkdir $(ODIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(BDIR)/*

test: testInOut testConst testCompare testArith testEval
	./$(BDIR)/testInOut
	./$(BDIR)/testConst
	./$(BDIR)/testCompare
	./$(BDIR)/testArith
	./$(BDIR)/testEval

testInOut: $(BDIR)/testInOut
testCompare: $(BDIR)/testCompare
testConst: $(BDIR)/testConst
testArith: $(BDIR)/testArith
testEval: $(BDIR)/testEval

$(BDIR)/testInOut: $(ODIR)/inputoutput.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

$(BDIR)/testConst: $(ODIR)/constructors.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

$(BDIR)/testCompare: $(ODIR)/compareoperators.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

$(BDIR)/testArith: $(ODIR)/arithmetic.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

$(BDIR)/testEval: $(ODIR)/evaluators.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

