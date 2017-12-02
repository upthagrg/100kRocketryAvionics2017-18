default: pdf

pdf: ps
	ps2pdf design.ps
	rm -f *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc

ps:	dvi
	dvips -t letter design.dvi

dvi:
	latex design
	bibtex design||true
	latex design
	latex design

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
