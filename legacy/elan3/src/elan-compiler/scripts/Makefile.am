
ELANSCRIPTS    =
GENELANSCRIPTS =  elanc

bin_SCRIPTS = $(ELANSCRIPTS) $(GENELANSCRIPTS)
EXTRA_DIST  = $(ELANSCRIPTS) elanc.src

CLEANFILES  = elanc

elanc: elanc.src
	sed 's@__PREFIX__@${prefix}@g' < $< > $@

