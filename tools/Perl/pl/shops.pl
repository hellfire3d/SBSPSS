$OutFile=shift(@ARGV);
$World=shift(@ARGV);
$Type=shift(@ARGV);
$Command=shift(@ARGV);
$DataOut=shift(@ARGV);
$GrafDir=shift(@ARGV);

$OutFile=">$OutFile";

open(OutFile) || die "Can't open in file $OutFile; $!";

printf(OutFile "ifdef POO\n");

printf(OutFile  ".PHONY : $World$Type clean$World$Type\n\n");

foreach $Val (@ARGV)
	{
	@Stuff=split(/-/,$Val);
	$ArsName=shift(@Stuff);

	push(@ArsNames,$ArsName);
	
	$ArsNameToAnims{$ArsName}=join('!',@Stuff);
	}

printf(OutFile  "$World$Type");
printf(OutFile  "_ARS_FILES := ");

foreach $Ars (@ArsNames)
	{printf(OutFile "$DataOut$Ars.ars ");}

printf(OutFile "\n");

foreach  $Ars (@ArsNames)
	{
	printf(OutFile "$DataOut$Ars.ars : ");

	@Anims=split(/!/,$ArsNameToAnims{$Ars});

	foreach $Anim (@Anims)
		{printf(OutFile "$GrafDir$Ars/$Anim.gin ");}

	printf(OutFile "\n\t$Command ");

	foreach $Anim (@Anims)
		{printf(OutFile "$GrafDir$Ars/$Anim.gin ");}

	printf(OutFile "\n\n");
	}
	
printf(OutFile "$World$Type :  \$($World$Type");
printf(OutFile "_ARS_FILES)\n\n");

printf(OutFile "clean$World$Type :\n");
printf(OutFile "\t\@\$(RM) -f \$($World$Type");
printf(OutFile "_ARS_FILES)\n");
printf(OutFile "\t\@\$(ECHO) Cleaned $World $Type\n");

printf(OutFile "\n\nendif");

close(OutFile);
	


