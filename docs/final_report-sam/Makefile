default: pdf

pdf: ps
	ps2pdf techreview.ps
ps: dvi
	dvips -t letter techreview.dvi

dvi:
	latex techreview
	latex techreview
	latex techreview

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *bbl *.pyg *.toc
