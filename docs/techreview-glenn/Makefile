default: pdf

pdf: ps
	ps2pdf techreview.ps
	rm -f *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc

ps:	dvi
	dvips -t letter techreview.dvi

dvi:
	latex techreview
	bibtex techreview||true
	latex techreview
	latex techreview

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc
