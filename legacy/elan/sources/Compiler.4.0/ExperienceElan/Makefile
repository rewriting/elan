
all: progC progElan


progC: experienceC.C
	gcc experienceC.C -o progC

progElan: experience.lgi experienceElan.eln
	elanc experience -O
	gmake --file experience.make
	mv a.out progElan