javadir         = ${prefix}/classes/rem/parser
CLASSPATH_ENV   = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):$(JATERM):$$CLASSPATH

BOOTSOURCES	= 

generated_java  = ASCII_CharStream.java\
                  REFParserConstants.java\
                  TokenMgrError.java\
	          ParseException.java\
                  REFParserTokenManager.java\
                  REFParser.java\
                  SimpleCharStream.java\
	          Token.java

java_JAVA       = $(BOOTSOURCES) $(generated_java) 

JAVAROOT        = ${top_builddir}/src

JAVACC          = @JAVACC@

MAINTAINERCLEANFILES = $(generated_java) 

$(generated_java): REFParser.jj
	$(JAVACC) $<
	touch $(generated_java)

EXTRA_DIST 	= REFParser.jj $(BOOTSOURCES) $(generated_java) 
