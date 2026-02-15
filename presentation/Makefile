SOURCES = $(wildcard ./*.tex)
OUTDIR = ./publish
RESULT = $(OUTDIR)/main.pdf

$(RESULT): $(OUTDIR) $(SOURCES)
	lualatex --output-directory=$(OUTDIR) main.tex

$(OUTDIR):
	mkdir $(OUTDIR)

open: $(RESULT)
	firefox $(RESULT)

clean:
	rm -fr $(OUTDIR)
	
.PHONY: open clean
