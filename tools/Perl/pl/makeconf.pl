	use	Win32;

&WriteConf($ARGV[0],$ARGV[1]);

sub WriteConf
{
	local ($INFILE,$OutName)=@_;
	local ($OUTFILE);
	$OUTFILE=">";
	$OUTFILE.=$OutName;

	$Name=Win32::LoginName;

	open(INFILE) || die "Can't open in file $INFILE; $!";
	open(OUTFILE) || die "Can't open output file $OutName; $!";

	while (<INFILE>)
		{
		s/\!/$Name/g;
		print (OUTFILE $_);
		}

	close($OUTFILE);
	close(INFILE);
}
