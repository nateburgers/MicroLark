OBJS := ast.o main.o lexer.yy.o parser.tab.o
CFLAGS := $(shell llvm-config --cppflags)
LDFLAGS := $(shell llvm-config --cxxflags --libs core) -lncurses
BINARY := microlark
CC := clang++

all: $(BINARY)

# parser
parser.tab.cpp parser.tab.hpp: parser.y
	bison -d -o parser.tab.cpp parser.y

lexer.yy.cpp: lexer.l parser.tab.hpp
	flex -o lexer.yy.cpp lexer.l	

# link
$(BINARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(BINARY)

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info;
# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# remove compilation products
clean:
	rm -f $(BINARY) *.o *.d *.yy.cpp *.yy.hpp *.yy.o *.tab.hpp *.tab.cpp
