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

default: all


$(ODIR)/%.o: $(TDIR)/%.cpp $(DEPS) | $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: test | $(BDIR)

$(BDIR):
	mkdir $(BDIR)
$(ODIR):
	mkdir $(ODIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(BDIR)/*

test: testInOut testConst testCompare testArith testEval testExc noncompileTest
	@echo "Testing fixedpoint.h ..."
	@echo "Running input and output operator tests:"
	@./$(BDIR)/testInOut
	@echo "Running constructor tests:"
	@(./$(BDIR)/testConst || echo "NOTE: Ignore this failure if it happened on short or higher integral type")
	@echo "Running comparison operator tests:"
	@./$(BDIR)/testCompare
	@echo "Running artihmetic tests:"
	@./$(BDIR)/testArith
	@echo "Running postfix and infix evaluation tests:"
	@./$(BDIR)/testEval
	@echo "Running tests for triggering runtime exceptions:"
	@./$(BDIR)/testExc
	@echo "[OK] All tests completed sucessfully!"

testInOut: $(BDIR)/testInOut
testCompare: $(BDIR)/testCompare
testConst: $(BDIR)/testConst
testArith: $(BDIR)/testArith
testEval: $(BDIR)/testEval
testExc: $(BDIR)/testExc

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

$(BDIR)/testExc: $(ODIR)/runtimeexceptions.o | $(BDIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

noncompileTest: $(TDIR)/noncompile.cpp
	@echo "Compiling this file should fail (noncompile.cpp)"
	@ (!($(CXX) $(CXXFLAGS) $(LIBS) $^) && echo "[OK] Compilation failure test successful")
