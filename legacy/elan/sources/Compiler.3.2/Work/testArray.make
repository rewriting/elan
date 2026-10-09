SUBDIRS = .elan.testArray/

all clean veryclean:
	@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; cd $$DIR && $(MAKE) $@; cp -f a.out ..; cd ..; done

