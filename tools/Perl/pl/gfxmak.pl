
$OutFile=shift(@ARGV);
$World=shift(@ARGV);
$DataOut=shift(@ARGV);
$GrafDir=shift(@ARGV);

if (0)
	{
	printf "OutFile :  $OutFile\n";
	printf "World :    $World\n";
	printf "DataOut :  $DataOut\n";
	printf "GrafDir :  $GrafDir\n";
	printf "\n";
	}

$OutFile=">$OutFile";

open(OutFile) || die "Can't open in file $OutFile; $!";

$RidesDir=$GrafDir;

$RidesOutDir=$DataOut;

foreach $Val (@ARGV)
	{
	@Stuff=split(/-/,$Val);

	$RideBank=shift(@Stuff);

	$RideName=shift(@Stuff);

	if ($RideBank eq 1)
		{
		push(@RideNamesBank1,$RideName);
		}

	if ($RideBank eq 2)
		{
		push(@RideNamesBank2,$RideName);
		}


	push(@RideNames,$RideName);
	
	$RideNameToAnims{$RideName}=join('!',@Stuff);
	$RideNameToRideBank{$RideName}=$RideBank;
	}

foreach $Ride (@RideNames)
	{push (@AllArsFiles,"$RidesOutDir$Ride.ars");}

$SharedOutWorld_1 ="shared_out_";
$SharedOutWorld_1 .="$World";
$SharedOutWorld_1 .="_1";


$SharedOutWorld_2 ="shared_out_";
$SharedOutWorld_2 .="$World";
$SharedOutWorld_2 .="_2";

print (OutFile ".PHONY : make$World clean$World shared$World shared$World");
print (OutFile "1 shared$World");
print (OutFile "2 cleanshared$World\n\n");

printf(OutFile  "shared_out_$World := \$(REPORT_DIR)/$World.rep\n");

printf (OutFile "$SharedOutWorld_1 := \$(REPORT_DIR)/$World");
printf (OutFile "_1.rep\n");

printf (OutFile "$SharedOutWorld_2 := \$(REPORT_DIR)/$World");
printf (OutFile "_2.rep\n");

printf (OutFile "\n\n");

print (OutFile $World,"_ARS_FILES := ",join(' ',@AllArsFiles),"\n\n");


printf (OutFile "BANK_1_GIN_$World := ");

foreach $Ride (@RideNamesBank1)
	{
	@TheseGins=split(/!/,$RideNameToAnims{$Ride});
	foreach $Gin (@TheseGins)
		{
		printf(OutFile "$GrafDir$Ride/$Gin.gin ");
		}
	}

printf (OutFile "\n");


printf (OutFile "BANK_2_GIN_$World := ");

foreach $Ride (@RideNamesBank2)
	{
	@TheseGins=split(/!/,$RideNameToAnims{$Ride});
	foreach $Gin (@TheseGins)
		{
		printf(OutFile "$GrafDir$Ride/$Gin.gin ");
		}
	}
printf (OutFile "\n");

printf (OutFile "\n\n");


foreach $Ride (@RideNames)
	{
	@Anims=split(/!/,$RideNameToAnims{$Ride});

	printf (OutFile "$RidesOutDir$Ride.ars : ");

	foreach $RideAnim (@Anims)
		{printf (OutFile "$RidesDir$Ride/$RideAnim.gin ");}

	print (OutFile "\n\t@\$(ECHO) Creating $Ride.ars from ",join(" ",@Anims));
	print (OutFile "\n\t\$(");
	print (OutFile "$World");
	print (OutFile "_MAKE_RIDE_");
	print (OutFile "$RideNameToRideBank{$Ride}) ");

	foreach $RideAnim (@Anims)
		{printf (OutFile "$RidesDir$Ride/$RideAnim.gin ");}

	print (OutFile "\n\n");
	}


printf(OutFile  "\nshared$World");
printf(OutFile  "1 : \$($SharedOutWorld_1)\n\n");

printf(OutFile  "\nshared$World");
printf(OutFile  "2 : \$($SharedOutWorld_2)\n\n");

printf(OutFile  "\ncleanshared$World");
printf(OutFile  "1 :\n\t\$(RM) -f \$($SharedOutWorld_1)\n\n");

printf(OutFile  "\ncleanshared$World");
printf(OutFile  "2 :\n\t\$(RM) -f \$($SharedOutWorld_2)\n\n");

printf(OutFile  "\nshared$World : \$(shared_out_$World)\n\n");

printf(OutFile  "cleanshared$World : \n\t\$(RM) -f \$(shared_out_$World)\n\n");

printf(OutFile  "\$(shared_out_$World) : \$($World");
printf(OutFile  "_shared_tx_full)\n");
printf(OutFile	"\t\$(MAKE_SHARED) \$($World");

printf(OutFile	"_shared_tx_full) \$($World");
printf(OutFile	"_anim_shared_tx_full)\n\n");

printf(OutFile  "\$(");
printf(OutFile  $World);
printf(OutFile  "_ARS_FILES) : \$(shared_out_$World) \$($SharedOutWorld_1) \$($SharedOutWorld_2)\n\n");

printf(OutFile  "\$($SharedOutWorld_1) : \$(BANK_1_GIN_$World)\n\t\$(MAKE_SHARED_1) \$(BANK_1_GIN_$World)\n\n");
printf(OutFile  "\$($SharedOutWorld_2) : \$(BANK_1_GIN_$World)\n\t\$(MAKE_SHARED_2) \$(BANK_2_GIN_$World)\n\n");

print (OutFile "make$World : shared$World shared$World");
print (OutFile "1 shared$World");
print (OutFile "2 \$(",$World,"_ARS_FILES)\n\n");

print (OutFile "shared$World");
print (OutFile "1 shared$World");
print (OutFile "2 : shared$World\n");

printf (OutFile "\n\n");

print (OutFile "clean$World :\n\t@\$(RM) -f \$(",$World,"_ARS_FILES)\n\t@\$(ECHO) cleaned $World rides\n");

printf (OutFile "\n");
printf (OutFile "\n");

printf (OutFile "\n\n");


close(OutFile);



























