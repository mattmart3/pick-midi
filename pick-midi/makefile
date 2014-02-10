CC=gcc

SRCDIR := src
OUTDIR := bin
CFLAGS := -c -Wall
LDFLAGS := -lm -lfftw3
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
NF:= fft_note_finder
EXECUTABLE := pick-midi

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OUTDIR) $(OBJECTS) 
	$(CC) -o $(OUTDIR)/$@ $(OBJECTS) $(LDFLAGS) 
	rm $(OBJECTS)
	
$(OUTDIR):
	mkdir $(OUTDIR)

$(NF):
	$(CC) $(LDFLAGS) $(SRCDIR)/$(NF).c -o $(OUTDIR)/$(NF)

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUTDIR)
