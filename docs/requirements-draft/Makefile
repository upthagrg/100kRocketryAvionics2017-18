default: pdf

pdf: ps
	ps2pdf requirements.ps

ps:	dvi
	dvips -t letter requirements.dvi

dvi:
	latex requirements
	latex requirements
	latex requirements

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
