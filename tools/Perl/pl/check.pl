$HadOver=0;

while (<>)
	{
	chop $_;
	$File=$_;

	if ($File ne "")
		{
		local($INFILE)=$File;
		binmode($INFILE);

		open(INFILE) || die "Can't open in file $INFILE; $!";
		binmode INFILE;
		$Size= -s INFILE;

		$buf='';
		read(INFILE,$buf,$Size);

		if ($buf=~/_GLOBAL_\.[ID]/)
			{
			print "Construction code in $File\n";
			$HadOver=1;
			}

		close(INFILE);
		}
	}

if ($HadOver == 0)
	{
	print "No errors";
	}
else
	{
	print "Error: Constructors in overlay code";
	}
