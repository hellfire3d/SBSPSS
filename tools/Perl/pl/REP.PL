# main
#{
	($ReportFile,$OldFile)=@ARGV;

	if ($OldFile ne "")
		{
		ReadOldFile($OldFile);
		}

	&FindStuff($ReportFile);
#}

sub ReadOldFile
{
	local ($INFILE)=@_;

	open(INFILE) || die "Can't open in file $INFILE; $!";

	while (<INFILE>)
		{
		chop $_;
		next if ($_ eq "");

		if (/^Reporting Section (\w+)\s/)
			{
			local($Section)=$1;

			while (<INFILE>)
				{
				chop $_;
				last if ($_ eq "");
				if (/^(\w+)\s+(\d+)/)
					{
					local ($FileName)=$1;
					local ($Size)=$2;
					local ($SecFile)="$Section:$FileName";

					$OldSecFileToSize{$SecFile}=$2;
					}
				}
			}
		else
			{
			die "Expection section got $_";
			}
		}
}

sub FindStuff
{
	local ($INFILE)=@_;

	open(INFILE) || die "Can't open in file $INFILE; $!";

	while (<INFILE>)
		{
		chop $_;
		last if (/^Program/);

		if (/^\s[A-Z0-9]+\s[A-Z0-9]+\s([A-Z0-9]+)\s[A-Z0-9]+\s(\w+)+\s+\.(\w+)_.*/)
			{
			$Size=$1;
			$Section=$2;
			$File=$3;

			push (@AllSecs,$Section) if (!$SecSeen{$Section}++);

			$SecToRec{$Section}.="!" if ($SecToRec{$Section} ne "");
			$SecToRec{$Section}.="$File:$Size";
			}
		}

	close(INFILE);

	foreach $Sec (@AllSecs)
		{
		local(@Recs)=split(/!/,$SecToRec{$Sec});
		local(%SizeToFile);

		local ($SizeInAll)=0;

		foreach $DatRec (@Recs)
			{
			($File,$Size)= split(/:/,$DatRec);
			$Size=hex($Size);

			$SizeInAll+=$Size;

			$SizeToFile{$Size}.=":" if ($SizeToFile{$Size} ne "");
			$SizeToFile{$Size}.=$File;
			}

		$SizeK=$SizeInAll/1024;

		print "\nReporting Section $Sec ($SizeK $SizeInAll)\n";

		foreach $Num (sort numerically keys(%SizeToFile))
			{
			local(@Files)=split(/:/,$SizeToFile{$Num});

			foreach $LastFile (@Files)
				{
				print "$LastFile\t$Num";
				local($SecFile)="$Sec:$LastFile";

				if ($OldSecFileToSize{$SecFile})
					{
					local($Changed)=$Num-$OldSecFileToSize{$SecFile};
					print"\t$Changed";
		#			print"was $OldSecFileToSize{$SecFile}";
					}

				print "\n";
				}

			}
		}
}

sub numerically { $b <=> $a;}
