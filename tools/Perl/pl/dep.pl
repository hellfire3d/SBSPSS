# main
#{
	($InFile,$OutFile,$ObjFile)=@ARGV;

	$OutFile=">$OutFile";

	open(InFile) || die "Can't open in file $InFile; $!";
	open(OutFile) || die "Can't open in file $OutFile; $!";

	while (<InFile>)
		{
		s/^.*\.obj.*:\w*.*\.cpp/$ObjFile:/g;
		print (OutFile $_);
		}

	close(OutFile);
	close(InFile);
#}
