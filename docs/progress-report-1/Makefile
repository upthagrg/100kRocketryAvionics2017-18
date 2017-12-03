default: pdf

all: pdf2

pdf: ps
	ps2pdf progress.ps
	rm -f *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc

pdf2: ps
	ps2pdf progress.ps

ps:	dvi
	dvips -t letter progress.dvi

dvi:
	latex progress
	bibtex progress||true
	latex progress
	latex progress

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
