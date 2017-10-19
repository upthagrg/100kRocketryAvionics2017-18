LATEX	= latex -shell-escape
DVIPS	= dvips

SRC	:= $(shell egrep -l '^[^%]*\\begin\{document\}' *.tex)
TRG	= $(SRC:%.tex=%.dvi)
PSF	= $(SRC:%.tex=%.ps)
PDF	= $(SRC:%.tex=%.pdf)

pdf: $(PDF)

ps: $(PSF)

$(TRG): %.dvi: %.tex

	$(LATEX) $<
	$(LATEX) $<
	$(LATEX) $<


$(PSF):%.ps: %.dvi
	$(DVIPS) -R -P outline -t letter $< -o $@

$(PDF): %.pdf: %.ps
	ps2pdf $<

all: pdf

clean:
	rm -f *.pdf *.ps *.dvi *.log *.aux *.toc

.PHONY: all clean ps pdf

