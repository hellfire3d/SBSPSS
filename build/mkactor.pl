$OutFile=shift(@ARGV);
$Actor=shift(@ARGV);

# printf("I got\n0: $OutFile\n1: $Actor\n");

$OutFile=">$OutFile";
open(OutFile) || die "Can't create makefile $OutFile; $!";
# print  <<eot
print OutFile <<eot
$Actor\:\t\$($Actor\_OUT)

$Actor\_IN :=\t\$(ACTOR_IN_DIR)/$Actor/\$(ACTOR_SPRITE_DIR)/*
$Actor\_OUT :=\t\$(ACTOR_OUT_DIR)/$Actor.sbk

\$($Actor\_OUT) : \$($Actor\_IN)
	\@\$(MKACTOR) -r:\$(ACTOR_IN_DIR) -s:\$(ACTOR_SPRITE_DIR) -o:\$(ACTOR_OUT_DIR) -i:\$(INC_DIR) $Actor \$(MKACTOR_OPTS)


eot
;
close(OutFile);

