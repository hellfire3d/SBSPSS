while (<>)
	{
	if (/^\s+.*\s+(\d+)\s+(\d+)\s+(\d+)/)
		{
		if ($1 eq "0")
			{
			if ($2 eq "0")
				{
				if ($3 eq "0")
					{
					print "$_";
					}
				}
			
			}
		}
	else
		{
		print $_;
		
		}
	}