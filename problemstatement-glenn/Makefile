default: pdf

pdf: ps
	ps2pdf problemstatement.ps

ps:	dvi
	dvips -t letter problemstatement.dvi

dvi:
	latex problemstatement
	latex problemstatement
	latex problemstatement

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
