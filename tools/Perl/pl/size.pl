
	&PrintSize($ARGV[0]);


sub PrintSize
{
	local ($INFILE)=@_;

	open(INFILE) || die "Can't open in file $INFILE; $!";
	
	while (<INFILE>)
		{
		chop $_;

		if (/^\s[A-Z0-9]+\s([A-Z0-9]+)\s[A-Z0-9]+\s[A-Z0-9]+\s\w+\s+\.last/)
			{
			$Addr=$1;

			$Addr=hex($Addr)-hex("80000000");
			$Over=$Addr-(2*1024*1024);
			if ($Over ge 0)
				{
				print "Size is $Addr ($Over over)\n";
				}
			else
				{
				$Over=0-$Over;
				print "Size is $Addr ($Over to spare)\n";
				}
			last;
			}
			
		}
}



