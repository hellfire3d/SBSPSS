$OutFile=shift(@ARGV);
$Actor=shift(@ARGV);

# printf("I got\n0: $OutFile\n1: $Actor\n");

$OutFile=">$OutFile";
open(OutFile) || die "Can't create makefile $OutFile; $!";
# print  <<eot
print OutFile <<eot
ACTOR_$Actor\:\t\$(ACTOR_$Actor\_OUT)

ACTOR_$Actor\_IN_DIR :=\t\$(ACTOR_IN_DIR)/$Actor
ACTOR_$Actor\_OUT_DIR :=\t\$(ACTOR_OUT_DIR)

ACTOR_$Actor\_IN :=\t\$(ACTOR_$Actor\_IN_DIR)/$Actor.gin
ACTOR_$Actor\_OUT :=\t\$(ACTOR_$Actor\_OUT_DIR)/$Actor.a3d

ACTOR_$Actor\_ANIM_LIST :=\t\$(foreach FILE, \$($Actor\_ANIM_LIST),-a:\$(ACTOR_$Actor\_IN_DIR)/\$(FILE).gin)
ACTOR_$Actor\_TEX_LIST :=\t\$(foreach FILE,	\$($Actor\_EXTRA_TEX),-x:textures/\$(FILE).bmp)

\$(ACTOR_$Actor\_OUT) : \$(ACTOR_$Actor\_IN)
	\@\$(MKACTOR3D) \$(ACTOR_$Actor\_IN) -o:\$(ACTOR_$Actor\_OUT_DIR) -t:24,1,1 -s:256 -i:\$(INC_DIR) \$(ACTOR_$Actor\_TEX_LIST) \$(ACTOR_$Actor\_ANIM_LIST)


eot
;
close(OutFile);
