default: pdf

pdf: ps
	ps2pdf report.ps
	rm -f *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc

ps:	dvi
	dvips -t letter report.dvi

dvi:
	latex report
	bibtex report||true
	latex report
	latex report

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
