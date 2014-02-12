CC=gcc

SRCDIR := src
OUTDIR := bin
CFLAGS := -c -Wall
LDFLAGS := -lm -lfftw3
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
EXECUTABLE := pick-midi

NOTE_FINDER := fft_note_finder

all: $(SOURCES) $(EXECUTABLE)
	
#debug entry
debug: CFLAGS+=-DFFT_DEBUG
debug: clean all

$(EXECUTABLE): $(OUTDIR) $(OBJECTS) 
	$(CC) -o $(OUTDIR)/$@ $(OBJECTS) $(LDFLAGS) 
	rm $(OBJECTS)
	
$(OUTDIR):
	mkdir $(OUTDIR)

$(NOTE_FINDER):
	$(CC) $(SRCDIR)/$(NOTE_FINDER).c -o $(OUTDIR)/$(NOTE_FINDER) $(LDFLAGS)

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUTDIR)
