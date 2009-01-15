SRCS = AST.cxx main.cxx token.cxx parse.cxx
CPPFLAGS   = `llvm-config --cppflags`
CXXFLAGS   = -g -Wall $(CPPFLAGS) `llvm-config --cxxflags core`
LDFLAGS    = `llvm-config --ldflags core` 
LDADD      = `llvm-config --libs core`
MAKEDEPEND = @echo "  DEP " $<; g++ -M $(CPPFLAGS) -o $(df).d $<
LDC        = @echo "  LD  " $@; g++ $(LDFLAGS) 
CCC        = @echo "  C++ " $@; g++ $(CXXFLAGS)
DEPDIR     = .deps

.PHONY: dep-init all clean

all: dep-init ncc
clean:
	rm -f ncc
	rm -f *.o
	rm -f *.P

ncc: $(SRCS:.cxx=.o)
	$(LDC) -o ncc $(SRCS:.cxx=.o) $(LDADD)

df = $(DEPDIR)/$(*F)

%.o : %.cxx
	@if [ ! -d $(DEPDIR) ]; then mkdir $(DEPDIR); fi;
	$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CCC) -o $@ -c $<

-include $(SRCS:%.cxx=$(DEPDIR)/%.P)
