SUBDIRS = .elan.orient/

all clean veryclean:
	@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; cd $$DIR && $(MAKE) $@; cp -f orient.out ..; cd ..; done

