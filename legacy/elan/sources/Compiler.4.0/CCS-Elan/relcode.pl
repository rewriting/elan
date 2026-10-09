#[] appl F_L on set_ff_L => set_ff end
$r = @ARGV[0];
$m = "";
while (defined($i = <STDIN>)) 
  { if (defined($j = <STDIN>)) { 
    chomp($i);
    chomp($j);
    $m = $m."[] appl $r on $j => $i end\n";
  }}
print $m;
