	&GetSizes($ARGV[0]);



sub GetSizes
{
	local ($INFILE)=@_;

	open(INFILE) || die "Can't open in file $INFILE; $!";

	while (<INFILE>)
		{
		last if (/.*Names in address order/);
		}

	while (<INFILE>)
		{
		chop $_;

		if (/^\s([A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9])\s(.*)/)
			{
			$Addr=$1;
			$Label=$2;
			$Addr=hex($Addr);

			if ($Addr >= hex("80000000"))
				{
				if (!$Seen{$Label})
					{
					$Seen{$Label}++;
					$Addr=$Addr-hex("80000000");

					if ($LastLabel ne "")
						{
						print "$LastLabel\tsize\t",$Addr-$LastAddr,"\n";
						}

					$LastAddr=$Addr;
					$LastLabel=$Label;
					}
				}
			}
		else
			{
			last;
			}
		}


	close(INFILE);
}
