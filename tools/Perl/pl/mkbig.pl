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

	local ($BaseFile,$BinDir,$OutFile)=@ARGV;

	$OUTFILE=">";
	$OUTFILE.=$OutFile;

	local ($INFILE)=$BaseFile;

	open(OUTFILE) || die "Can't open output file $OutFile: $!";
	open(INFILE) || die "Can't open monst inffile $InfFile; $!";

	while (<INFILE>)
		{
		s/\[BINDIR\]/$BinDir/g;
		s/\//\\/g;
		print (OUTFILE $_);
		}

	print "written $OutFile\n";

	close(INFILE);
	close($OUTFILE);
#}


