
	while (<>)
		{
		/^.*:\s+(.*)\n/;
		$CodeFile=$1;

		if ($CodeFile ne "")
			{
			$Code=<>;
			$Data=<>;
			$Bss=<>;
			<>;

			$Code=&GetNum($Code);
			$Data=&GetNum($Data);
			$Bss=&GetNum($Bss);;
	
			$CodeSizeToFile{$Code}.="!" if ($CodeSizeToFile{$Code} ne "");
			$CodeSizeToFile{$Code}.=$CodeFile;

			$DataSizeToFile{$Data}.="!" if ($DataSizeToFile{$Data} ne "");
			$DataSizeToFile{$Data}.=$CodeFile;
			
			$BssSizeToFile{$Bss}.="!" if ($BssSizeToFile{$Bss} ne "");
			$BssSizeToFile{$Bss}.=$CodeFile;
			}
		else
			{
			die "Strange line $_";
			}
		}

print "Code Section\n";
print "------------\n";

foreach $Num (sort numerically keys(%CodeSizeToFile))
	{
	local(@Files)=split(/!/,$CodeSizeToFile{$Num});

	foreach $File (@Files)
		{
		print "$File\t$Num\n";
		}
	}

print "\n";


print "Data Section\n";
print "------------\n";

foreach $Num (sort numerically keys(%DataSizeToFile))
	{
	local(@Files)=split(/!/,$DataSizeToFile{$Num});

	foreach $File (@Files)
		{
		print "$File\t$Num\n";
		}
	}

print "\n";

print "Bss Section\n";
print "-----------\n";

foreach $Num (sort numerically keys(%BssSizeToFile))
	{
	local(@Files)=split(/!/,$BssSizeToFile{$Num});

	foreach $File (@Files)
		{
		print "$File\t$Num\n";
		}
	}

print "\n";

sub GetNum
{
	local ($Line)=@_;

	if ($Line=~/^\s+\w+\s+size:\s+(\d+)/)
		{
		($1);
		}
	else
		{
		die "Fuck";
		}

	($1);
}


sub numerically { $b <=> $a;}
