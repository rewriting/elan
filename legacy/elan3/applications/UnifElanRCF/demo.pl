# run with command perl p.pl

$ex     = 0;
#$shower = "cat ";
$shower = "more ";
$elan   = "/u/protheo/pot/ELAN/version.test4/bin/`uname -m`/elan --elanlib /u/protheo/pot/ELAN/version.test4/elanlib -q ";
$celan   = "/u/protheo/pot/ELAN/version.test4/bin/`uname -m`/elan --elanlib /u/protheo/pot/ELAN/version.test4/elanlib -c";

sub banner {
    print "\n";
    print "          ##########################\n";
    print "          #\n";
    print "          #   $_[0]  \n";
    print "          #\n";
    print "          ##########################\n\n";
}

sub cont {
   print "$ex:> Press enter to continue\n"; 
   getc;
}

while (1) {
#===============================================
    print "\n";
    print "          ##################################################\n";
    print "          #\n";
    print "          #  Combining unification algorithms --- RCF Case \n";
    print "          #  ================================ \n";
    print "          #\n";
    print "          #  1. AC-unif. algorithm \n";
    print "          #  2. Encapsulation of the AC-unif. algorithm \n";
    print "          #  3. AC + 0 unification \n";
    print "          #  4. AC + C unification \n";
    print "          #\n";
    print "          ##################################################\n";
    $ex = getc; getc;
    system "clear";
#===============================================
if ($ex == 1) {
    banner("AC-unif. algorithm");
    system "$shower unifelan";
#    cont();
    banner("enter an equational system") ;
    system "unifelan";
}
#===============================================
if ($ex == 2) {
    banner("Encapsulation of the AC-unif. algorithm");
    system "$shower callUnif.eln";
#    cont();
    system "$elan Unif simplesig";
}
#===============================================
if ($ex == 3) {
    banner("AC + 0 unification");
#    cont();
    system "$elan combiRCF sigRCF"; 
}

#===============================================
if ($ex == 4) {
    banner("C-unif. module") ;
    system "$shower commutDec.eln" ;
    banner("AC + C unification");
#    cont();
    system "$elan combiACplusC sigACplusC"; 
}
#===============================================
}


