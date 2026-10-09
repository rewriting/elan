all : 
	javac -sourcepath src src/completion/Rule.java -d classes/
	javac -sourcepath src src/completion/Operator.java -d classes/
	javac -sourcepath src src/completion/OpTree.java -d classes/
	javac -sourcepath src src/completion/Specification.java -d classes/
	javac -sourcepath src src/completion/Pair.java -d classes/
	javac -sourcepath src src/completion/Substitution.java -d classes/
	javac -sourcepath src src/completion/KbcException.java -d classes/
	javac -sourcepath src src/completion/CompilationException.java -d classes/
	javac -sourcepath src src/completion/ColoredLabel.java -d classes/
	javac -sourcepath src src/completion/Settings.java -d classes/
	javac -sourcepath src src/completion/SettingFrame.java -d classes/
	javac -sourcepath src src/completion/CriticalPairsFrame.java -d classes/
	javac -sourcepath src src/completion/OrientationFrame.java -d classes/
	javac -sourcepath src src/completion/CompletionFrame.java -d classes/
	javac -sourcepath src src/completion/MyTableModel.java -d classes/
	javac -sourcepath src src/completion/SpecificationFrame.java -d classes/
	javac -sourcepath src src/InterfaceElan.java -d classes/

cc:
	rm -f classes/completion/parser/*.class
	rm -f src/completion/parser/*.java
	javacc -OUTPUT_DIRECTORY:src/completion/parser/ src/completion/parser/*.jj
	javac -sourcepath src src/completion/parser/*.java -d classes


run :
	java InterfaceElan
