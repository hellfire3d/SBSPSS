	use	Win32;
	
if ($ARGV[0] ne "")
	{
	&WriteUserFile($ARGV[0]);
	}

sub WriteUserFile
{
	local ($OutName)=@_;
	local ($OUTFILE);
	$OUTFILE=">";
	$OUTFILE.=$OutName;

	open(OUTFILE) || die "Can't open output file $OutName; $!";

	$Name=lc(Win32::LoginName);
	print (OUTFILE "USER_NAME := $Name\n");
	
	close($OUTFILE);
}
