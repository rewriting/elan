#
RM = /bin/rm -f
LATEX = latex
BIBTEX = bibtex
MAKEINDEX = makeindex
SRC_TEX=*.tex *.eps
MP = mpost

# ELANDIR = 
# MANUAL = $(ELANDIR)/manual

PRETTYPRINTALL = prettyprintAll
#======================================================================

all: manual

manual: metapost manual.dvi manual.ps
	$(PRETTYPRINTALL)
	$(LATEX) manual.tex
	$(MAKEINDEX) manual
	$(BIBTEX) manual

metapost: elan.mp
	$(MP) elan.mp

manual.dvi: $(SRC_TEX)
	latex manual.tex

manual.ps: $(SRC_TEX)
	dvips -o manual.ps manual.dvi
#======================================================================

clean:
	$(RM) *~ \#~ .\#* *.log core 
veryclean: clean
	$(RM) *.idx *.scn *.nms *.bbl *.ilg *.ind

touch:
	touch *.tex
