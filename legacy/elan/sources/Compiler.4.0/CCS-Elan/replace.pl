$muster = @ARGV[0];
$repldat = @ARGV[1];
open(REPL,$repldat);
$m = "";$r = "";
while (defined($i = <STDIN>)) { $m = $m . $i; }
while (defined($i = <REPL>)) { $r = $r . $i; }
$m =~ s/$muster/$r/g;
print $m;
close(MAIN);
close(REPL);
