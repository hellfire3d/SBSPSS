#=========================================================================
#
#	LANG.PL
#
#	Author:  Gary Liddon @ Climax
#	Created: 
#	Project: TPW PSX
#	Purpose:
#	Usage:
#
#	Copyright (c) 1999 Climax Development Ltd
#
#===========================================================================

# PKG - Added check for invalid id tags


#MAIN
#{
	local ($englishFile,$outFile,@langFiles)=@ARGV;

	readTransFile($englishFile,"eng",\%english);

	foreach $Id (keys(%english))
		{
		$idToTrans{$Id}="eng=$english{$Id}";
		}


	foreach $file (@langFiles)
		{
		local (%otherLang);
		$otherLang={};

		printf "trying $file\n";

		if ($file=~/^.*\\(.*)\.dat/)
			{
			$prefix=$1;
			}
		else
			{
			if ($file=~/^.*\/(.*)\.dat/)
				{
				$prefix=$1;
				}
			}
			

		printf "reading $file\n";
		readTransFile($file,$prefix,\%otherLang);
		printf "read $file\n";

		foreach $Id (keys(%english))
			{
			if ($otherLang{$Id} ne "")
				{
				local (@trans)=split(/arsebiscuits/,$idToTrans{$Id});
				push(@trans,"$prefix=$otherLang{$Id}");
				$idToTrans{$Id}=join('arsebiscuits',@trans);
				}
			}

		}

	if ($outFile ne "")
		{
		$OUTFILE= ">$outFile";

		open(OUTFILE) || die "Can't open file $outFile; $!";

		foreach $Id (keys(%idToTrans))
			{
			print (OUTFILE "[$Id]\n");

			local (@trans)=split(/arsebiscuits/,$idToTrans{$Id});

			foreach $text (@trans)
				{
				print (OUTFILE "$text\n");
				}

			print (OUTFILE "\n");
			}

		close(OUTFILE);
		}
	
#}

# Read a translation file into an associative array
sub readTransFile
{
	local($inFile,$prefix,$destArray)=@_;

	$INFILE = $inFile;

	open(INFILE) || die "Can't open file $inFile; $!";

	while (<INFILE>)
		{
		if (/^\[(.*)]\n/)
			{
			$id=$1;
			if($id=~/[^a-zA-Z0-9_]/)
			{
				die("Invalid id tag [$id] in $inFile\nTag may only contain a-z, A-Z, 0-9 and _\n");
			}

			$Done = 0;

			while (!$Done)
				{
				$_=<INFILE>;
				chop $_;
				s/;.*//g;

				if ($_ ne "")
					{
					if (/^(.*)\=(.*)/)
						{
						if ($1 eq $prefix)
							{
							
							$text=$2;
							$Done=1;
							}
						}
					else
						{
						print "!$_!";
						die "incorrectly formatted file $inFile\nlooking for $prefix=<text> for id $id\n";
						}
					}
				}

			$$destArray{$id}="$text";
			}
		}

	close(INFILE);
}



#===========================================================================
#ends
