$OutFile=shift(@ARGV);
$ActorList=shift(@ARGV);

# printf("I got\n0: $OutFile\n1: $ActorList\n");

$OutFile=">$OutFile";
open(OutFile) || die "Can't create makefile $OutFile; $!";
# print  <<eot
print OutFile <<eot
$ActorList\:\t\$($ActorList\_OUT)

$ActorList\_IN :=\t\$(foreach ACTOR,\$($ActorList),\$(ACTOR_IN_DIR)/\$(ACTOR)/\$(ACTOR).gin)
$ActorList\_OUT :=\t\$(foreach ACTOR,\$($ActorList),\$(ACTOR_OUT_DIR)/\$(ACTOR).a3d)


\$($ActorList\_OUT) : \$($ActorList\_IN)
	\@\$(MKACTOR3D) \$($ActorList\_IN) -o:\$(ACTOR_OUT_DIR) \$($ActorList\_OPTS) -i:\$(INC_DIR) -g:$ActorList


eot
;
close(OutFile);
