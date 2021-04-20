CFLAGS += -I../ -I${ProjectRoot_Directory}

.ifdef _DEBUG
POSTFIX=_d
CFLAGS += -ggdb
OBJDIR = Debug
.else
POSTFIX =
OBJDIR  = Release
.endif

all: libspell$(POSTFIX).a

clean:
	rm -fr $(OBJDIR)

clean_all: clean
	rm -f libspell$(POSTFIX).a

libspell$(POSTFIX).a:	$(OBJDIR) $(OBJDIR)/spellChecker.o 	\
			$(OBJDIR)/spellCheckerRu.o 		\
			$(OBJDIR)/spellCheckerEn.o 		\
			$(OBJDIR)/spellCheckerUa.o
	ar -rc libspell$(POSTFIX).a	$(OBJDIR)/spellChecker.o	\
					$(OBJDIR)/spellCheckerRu.o	\
					$(OBJDIR)/spellCheckerEn.o 	\
					$(OBJDIR)/spellCheckerUa.o

$(OBJDIR)/spellChecker.o: src/spellChecker.c include/spellChecker.h $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ src/spellChecker.c

$(OBJDIR)/spellCheckerRu.o: src/spellCheckerRu.c include/spellChecker.h \
                            include/spellCheckerRu.h $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ src/spellCheckerRu.c

$(OBJDIR)/spellCheckerEn.o: src/spellCheckerEn.c include/spellChecker.h \
			    include/spellCheckerEn.h $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ src/spellCheckerEn.c

$(OBJDIR)/spellCheckerUa.o: src/spellCheckerUa.c include/spellChecker.h \
			    include/spellCheckerUa.h $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ src/spellCheckerUa.c

$(OBJDIR):
	@mkdir -p $(OBJDIR)

