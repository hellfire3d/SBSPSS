#=========================================================================
#
#	MKSTR.PL
#
#	Author:  Gary Liddon @ Climax
#	Created: 
#	Project: Diablo PSX
#	Purpose: Makes some bse files from a base file
#	Usage:   pl mkstr.pl <base file> <data dir> <gfx dir> <music dir> <stream sfx dir> <sfx dir>
#
#	Copyright (c) 1997 Climax Development Ltd
#
#===========================================================================


#MAIN
#{

	local ($BaseFile,$DataDir,$GfxDir,$VagDir,$SfxDir,$OutFile)=@ARGV;

	#die if ($BaseFile eq "" or $DataDir  eq "" or $GfxDir eq "" or $MusicDir eq "" or $StreamSfxDir eq "" or  $SfxDir or $OutFile eq "");

	$OUTFILE=">";
	$OUTFILE.=$OutFile;

	local ($INFILE)=$BaseFile;

	open(OUTFILE) || die "Can't open output file $OutFile: $!";
	open(INFILE) || die "Can't open monst inffile $InfFile; $!";

	while (<INFILE>)
		{
		s/\[DATA\]/$DataDir/g;
		s/\[GFX\]/$GfxDir/g;
		s/\[VAG\]/$VagDir/g;
		s/\[SFX\]/$SfxDir/g;
		s/\//\\/g;
		print (OUTFILE $_);
		}

	print "written $OutFile\n";

	close(INFILE);
	close($OUTFILE);

#}


