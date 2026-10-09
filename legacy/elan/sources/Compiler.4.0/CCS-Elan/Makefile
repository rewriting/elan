add_in_file = /home/thiemann/elan/version.3.5.4/elanlib/Compiler/truth_add_in.c

all: n 
	

n: nonac/a.out set.eln
	 

nonac/a.out: ccs.eln set.eln
	cd nonac && rm -rf *; cp ../ccs.eln .; cp ../ccs.lgi .; cp ../set.eln .; \
	elanc -optimiseChoicePoint ccs; \
	sed -e s/CALC_STRAT_VAL/`grep "^ *[0-9]*:\"calc_str:.*/.*\"\." ccs.ref | tr -d -c "[0-9]"`/g  $(add_in_file).preview | sed -e s/STERM_CODE_VAL/code_`grep "fsym_init(code_[0-9]*,[0-9]*,\"(;)\"" .elan.ccs/ccs.c | tr "," "\n" | head -n 1 | tr -c -d "0-9"`/g - >$(add_in_file); \
	gmake --file ccs.make; \
	cd ..


clean:
	rm -rf nonac; mkdir nonac
