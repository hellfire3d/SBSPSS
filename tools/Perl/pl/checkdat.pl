$Errors=0;

while (<>)
	{
	/^\[(.*)\]/;

	if ($1 ne "")
		{
		local (@Text);
		local ($StrId)=($1);

		$Text[0]=GetLang("Eng");
		$Text[1]=GetLang("Fre");
		$Text[2]=GetLang("Ger");
		$Text[3]=GetLang("Swe");
		$Text[5]=GetLang("Jap");

		$Errors+=CheckPerc($StrId,@Text);
		}
	}

if ($Errors != 0)
	{
	print "Some monkey's messed up the dbase with $Errors errors";
	}

sub GetLang
{
	local ($LangStr)=@_;

	$_=<>;
	die if (!(/($LangStr=\".*\")/));
	("File $ARGV line $. : $1");
}


sub CheckPerc
{
	local ($StrId,@Text)=@_;
	local ($f);
	local ($Errors);

	@Master=MakePercArray($Text[0]);

	for ($f=1;$f<6;$f++)
		{
		local (@Local)=MakePercArray($Text[$f]);

		if (!CheckPercArray(@Local))
			{
			print "Error with $StrId\n";
			$Text[0]=~/^File .* line .* : (.*)/;
			local ($ShouldBe)=$1;
			print "Should be $ShouldBe\n";
			print "$Text[$f]\n\n";
			$Errors++;
			}
		}
	return($Errors);
}

sub CheckPercArray
{
	local(@Local)=@_;
	local ($f);

	for ($f=0;$f<@Local;$f++)
		{
		return(0) if ($Local[$f] ne $Master[$f])
		}

	return(1);
}

sub MakePercArray
{
	local ($Str)=@_;
	local (@Ret);

	$Str=~s/%%/!!/g;

	while (1)
		{
		if ($Str=~/%([A-Za-z0-9+-]*)/)
			{
			push (@Ret,$1);
			$Str=$';
			}
		else
			{
			last;
			}
		}

	return(@Ret);

}
