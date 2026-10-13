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
    print "          #  Combination of unification --- General Case \n";
    print "          #  ========================== \n";
    print "          #\n";
    print "          #  1. Bool-unif. algorithm \n";
    print "          #  2. Encapsulation of the Bool-unif. algorithm \n";
    print "          #  3. Restricted Bool-unification \n";
    print "          #  4. 0 + Bool unification \n";
    print "          #\n";
    print "          ##################################################\n";
    $ex = getc; getc;
    system "clear";
#===============================================
if ($ex == 1) {
    banner("Bool-unif. algorithm");
    system "$shower unifFA";
#    cont();
    banner("enter an equational system") ;
    system "unifFA";
}
#===============================================
if ($ex == 2) {
    banner("Encapsulation of the Bool-unif. algorithm");
    system "$shower callUnifFA.eln";

    banner("The Bool. signature") ;
    system "$shower termUnifFA.eln" ;

    banner("Examples") ;
    system "$elan UnifFA sigFA" ;
}
#===============================================
if ($ex == 3) {
    banner("Restricted Unif. module") ;
    system "$shower RestrictUnifFA.eln" ;
    banner("Encapsulation of the Bool-elim. algorithm");
    system "$shower callElimFA.eln";
#    cont();
    banner("Examples") ;
    system "$elan RestrictUnifFA sigFA";

}

#===============================================
if ($ex == 4) {
    banner("0 + Bool unification");
#    cont();
    system "$elan combi0plusFA sig0plusFA"; 
}
#===============================================
}


