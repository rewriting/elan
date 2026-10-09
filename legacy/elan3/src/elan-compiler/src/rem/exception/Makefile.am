javadir         = ${prefix}/classes/rem/exception
CLASSPATH_ENV   = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):$$CLASSPATH

BOOTSOURCES	= \
ACPatternNotCompiled.java	     CompileError.java	RuleNotCompiled.java\
ACPatternWithSeveralACSubterms.java  RuleCompiled.java	RuleTransformed.java

java_JAVA       = $(BOOTSOURCES)

JAVAROOT        = ${top_builddir}/src
EXTRA_DIST      = $(BOOTSOURCES)

