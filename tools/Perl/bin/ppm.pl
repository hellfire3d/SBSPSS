#!/usr/bin/perl

use Getopt::Long;
use File::Basename;
use strict;

#use Win32::Shell;
#my $hidden;
#GetOptions('hide' => \$hidden);
#Win32::Shell::SetWindow(SW_HIDE) if $hidden;

use PPM;

$PPM::VERSION = "0.9.5";

my $usage = <<'EOT';
    usage: ppm help [command]
           ppm install [--location=location] package1 [... packageN]
           ppm query [--case|nocase] [--searchtag=abstract|author|title] PATTERN
           ppm remove package1 [... packageN]
           ppm search [--case|nocase] [--location=location] [--searchtag=abstract|author|title] PATTERN
           ppm set [option]
           ppm verify [--location=location] [--upgrade] [package1 ... packageN]
           ppm version
           ppm [--location=location]

EOT

my %help;

$help{'set'} = <<'EOT';
    set
        - Displays current settings.
    set build DIRECTORY
        - Changes the package build directory.
    set case [Yes|No]
        - Set case-sensitive searches.
    set clean [Yes|No]
        - Set removal of temporary files from package's build area
          following successful installation.
    set confirm [Yes|No]
        - Sets confirmation of 'install', 'remove' and 'upgrade'.
    set force_install [Yes|No]
        - Continue installing a package if a dependency cannot be installed.
    set more NUMBER
        - Pause after displaying 'NUMBER' lines.  Specifying '0' turns
          off paging capability.
    set repository /remove NAME
        - Removes the repository 'NAME' from the list of repositories.
    set repository NAME LOCATION
        - Adds a repository to the list of PPD repositories for this session.
          'NAME' is the name by which this repository will be referred;
          'LOCATION' is a URL or directory name.
    set root DIRECTORY
        - Changes install root directory for this session.
    set save
        - save current options as defaults.

EOT


my %options = PPM::GetPPMOptions();
use vars qw ($location $Ignorecase $clean $confirm $force_install $root $build_dir $more);
*Ignorecase = \$options{'IGNORECASE'};
*clean = \$options{'CLEAN'};
*confirm = \$options{'CONFIRM'};
*force_install = \$options{'FORCE_INSTALL'};
*root = \$options{'ROOT'};
*build_dir = \$options{'BUILDDIR'};
*more = \$options{'MORE'};

my $moremsg = "[Press return to continue]";
my $interactive = 0;

my %repositories = PPM::ListOfRepositories();

if ($#ARGV == -1 || ($#ARGV == 0 && $ARGV[0] =~ /^--location/)) {
    my $prompt = 'PPM> ';
	$interactive = 1;
    GetOptions("location=s" => \$location);
$help{'help'} = <<'EOT';
Commands:
    help [command]   - prints this screen, or help on 'command'.
    install PACKAGES - installs specified PACKAGES.
    quit             - leave the program.
    query [options]  - query information about installed packages.
    remove PACKAGES  - removes the specified PACKAGES from the system.
    search [options] - search information about available packages.
    set [options]    - set/display current options.
    verify [options] - verifies current install is up to date.

EOT

$help{'search'} = <<'EOT';
    search [PATTERN]
    search /abstract [PATTERN]
    search /author [PATTERN]
    search /title [PATTERN]
    search /location LOCATION [PATTERN]

    Searches for available packages.  With no arguments, will display
    a list of all available packages. If a regular expression 'PATTERN'
    is supplied, only packages matching the pattern will be displayed.
    If the '/abstract', '/author' or '/title' argument is specified,
    the respective field of the package will be searched.  If the
    '/location' option is specified, matching packages from that
    URL or directory will be listed.

EOT

$help{'install'} = <<'EOT';
    install PACKAGE
    install /location LOCATION PACKAGE

    Installs the specified 'PACKAGE' onto the system.  If the '/location'
    option is specified, the package will be looked for at that URL
    or directory.

EOT

$help{'remove'} = <<'EOT';
    remove PACKAGE

    Removes the specified 'PACKAGE' from the system.

EOT

$help{'query'} = <<'EOT';
    query [PATTERN]
    query /abstract [PATTERN]
    query /author [PATTERN]
    query /title [PATTERN]

    Queries information about installed packages. With no arguments, will
    display a list of all installed packages. If a regular expression
    'PATTERN' is supplied, only packages matching the pattern will be
    displayed.  If the '/abstract', '/author' or '/title' argument is
    specified, the respective field of the package will be searched.

EOT

$help{'verify'} = <<'EOT';
    verify [packages]
    verify /upgrade [packages]
    verify /location LOCATION [packages]

    Verifies that the currently installed 'packages' are up to date.
    If 'packages' is not specified, all installed packages are verified.
    If the '/upgrade' option is specified, any packages for which an
    upgrade is available will be upgraded.  If the '/location' option
    is specified, upgrades will be looked for at the specified URL
    or directory.

EOT

    print "PPM interactive shell ($PPM::VERSION) - type 'help' for available commands.\n";
    $| = 1;
    while () {
        print $prompt;
        last unless defined ($_ = <> );
        chomp;
        s/^\s+//;
        my @line = split(/\s+/, $_);
        my $cmd = shift @line;
        next if /^$/;

        if ($cmd  =~ /^quit/i) {
            print "Quit!\n";
            last;
        }
        elsif ($cmd =~ /^help$/i) {
            if (defined $line[0] && $help{$line[0]}) {
                print $help{$line[0]};
            }
            else {
                print $help{'help'};
            }
        }
        elsif ($cmd =~ /^install$/i) {
            my $package;
            local $location = $location;
            if (defined $line[0]) {
                if ($line[0] =~ /^\/location$/i) {
                    shift @line;
                    $location = shift @line;
                }
            }
            if (!defined @line) {
                print "Package not specified.\n";
                next;
            }
            foreach $package (@line) {
                if ($confirm eq "Yes") {
                    print "Install package \'$package?\' (y/N): ";
                    my $response = <>;
                    if ($response ne "y\n") {
                        next;
                    }
                }
                if(!InstallPackage("package" => $package, "location" => $location)) {
                    print "Error installing package '$package': $PPM::PPMERR\n";
                }
            }
        }
        elsif ($cmd =~ /^set$/i) {
            set(@line);
        }
        elsif ($cmd =~ /^remove$/i) {
            my $package;
            if (!defined @line) {
                print "Package not specified.\n";
                next;
            }
            foreach $package (@line) {
                if ($confirm eq "Yes") {
                    print "Remove package \'$package?\' (y/N): ";
                    my $response = <>;
                    if ($response ne "y\n") {
                        next;
                    }
                }
                if (!RemovePackage("package" => $package)) {
                    print "Error removing $package: $PPM::PPMERR\n";
                }
            }
        }
        elsif ($cmd =~ /^search$/i) {
            my $searchtag;
            local $location = $location;
            while (defined $line[0]) {
                if ($line[0] =~ /^\/abstract$/i) {
                    shift @line;
                    $searchtag = 'abstract';
                }
                elsif ($line[0] =~ /^\/author$/i) {
                    shift @line;
                    $searchtag = 'author';
                }
                elsif ($line[0] =~ /^\/title$/i) {
                    shift @line;
                    $searchtag = 'title';
                }
                elsif ($line[0] =~ /^\/location$/i) {
                    shift @line;
                    $location = shift @line;
                }
                else { last; }
            }
            my $searchRE = $line[0];
            search_PPDs("location" => $location, "searchtag" => $searchtag, 
                        "searchRE" => $searchRE, "ignorecase" => $Ignorecase);
        }
        elsif ($cmd =~ /^query$/i) {
            my $searchtag;
            if (defined $line[0] && ($line[0] =~ /^\/abstract$/i || 
                     $line[0] =~ /^\/author$/i || $line[0] =~ /^\/title$/i)) {
                 $searchtag = shift @line;
                 $searchtag = substr($searchtag, 1);
            }
            my $RE = shift @line;
            my %packages = QueryInstalledPackages("searchtag" => $searchtag, "searchRE" => $RE);
            if (!%packages && defined $PPM::PPMERR) {
                print "$PPM::PPMERR\n";
                next;
            }
            my $package;
            my $lines = 0;
            foreach $package (sort keys %packages) {
                print "\t$package" . (defined $searchtag ? ": $packages{$package}\n" : "\n");
                if ($more && ++$lines == $more) {
                    print $moremsg;
                    $_ = <>;
                    $lines = 0;
                }
            }
        }
        elsif ($cmd =~ /^verify$/i) {
            my $upgrade;
            local $location = $location;
            while (defined $line[0]) {
                if ($line[0] =~ /^\/upgrade$/i) {
                    $upgrade = 1;
                    shift @line;
                    next;
                }
                if ($line[0] =~ /^\/location$/i) {
                    shift @line;
                    $location = shift @line;
                    next;
                }
                last;
            }
            if ($upgrade && $confirm eq "Yes") {
                if ($#line == 0) {
                    print "Upgrade package $line[0]? (y/N): ";
                }
                else {
                    print "Upgrade packages? (y/N): ";
                }
                my $response = <>;
                if ($response ne "y\n") {
                    next;
                }
            }
            verify_packages("packages" => \@line, "location" => $location, "upgrade" => $upgrade);
        }
        else {
            print "Unknown command '$cmd'; type 'help' for commands.\n";
        }
    }
}
elsif ($ARGV[0] eq 'help') {
$help{'install'} = <<'EOT';
    install [--location=location] PACKAGE

    Installs the specified 'PACKAGE' onto the system.  If 'location' is
    not specified, the default locations in the PPM data file will be
    used to locate the package.

EOT

$help{'remove'} = <<'EOT';
    remove PACKAGE

    Removes the specified 'PACKAGE' from the system.

EOT

$help{'search'} = <<'EOT';
    search [--case|nocase] [--location=location] [PATTERN]
    search [--case|nocase] [--location=location] --searchtag=abstract [PATTERN]
    search [--case|nocase] [--location=location] --searchtag=author [PATTERN]
    search [--case|nocase] [--location=location] --searchtag=title [PATTERN]

    Searches for available packages.  With no arguments, will display
    a list of all available packages. If a regular expression 'PATTERN'
    is supplied, only packages matching the pattern will be displayed.
    If the 'abstract', 'author' or 'title' --searchtag argument is
    specified, the respective field of the package will be searched.
    If 'location' is not specified, the repository locations in the PPM
    data file will be searched.  '--case' or '--nocase' may be used to
    request case-sensitive or case-insensitive searches, respectively.

EOT

$help{'query'} = <<'EOT';
    query [PATTERN]
    query [--case|nocase] --searchtag=abstract [PATTERN]
    query [--case|nocase] --searchtag=author [PATTERN]
    query [--case|nocase] --searchtag=title [PATTERN]

    Queries information about installed packages. With no arguments, will
    display a list of all installed packages. If a regular expression
    'PATTERN' is supplied, only packages matching the pattern will
    be displayed.  If the 'abstract', 'author' or 'title' --searchtag
    argument is specified, the respective field of the package will
    be searched.  '--case' or '--nocase' may be used to request
    case-sensitive or case-insensitive searches, respectively.

EOT

$help{'verify'} = <<'EOT';
    verify [--location=location] [packages]
    verify --upgrade [--location=location] [packages]

    Verifies that the currently installed 'packages' are up to date.
    If 'packages' is not specified, all installed packages are verified.
    If the '--upgrade' option is specified, any packages for which
    an upgrade is available will be upgraded.  If 'location' is not
    specified, the repository locations in the PPM data file will be
    searched.

EOT
    shift;
    if (defined $ARGV[0] && $help{$ARGV[0]}) {
        print $help{$ARGV[0]};
    }
    else {
        print $usage;
    }
}
elsif ($ARGV[0] eq 'version') {
    print $PPM::VERSION;
}
elsif ($ARGV[0] eq 'set') {
    shift;
    if (set(@ARGV) == 0) {
        PPM::SetPPMOptions("options" => \%options, "save" => 1);
    }
}
elsif ($ARGV[0] eq 'install') {
    my ($package);

    shift;
    GetOptions("location=s" => \$location);

    $package = shift;

    while ($package) {
        if(!InstallPackage("package" => $package, "location" => $location)) {
            print "Error installing package '$package': $PPM::PPMERR\n";
        }
        $package = shift;
    }
}
elsif ($ARGV[0] eq 'remove') {
    shift;

    my $package = shift;

    while ($package) {
        if (!RemovePackage("package" => $package)) {
            print "Error removing $package: $PPM::PPMERR\n";
        }
        $package = shift;
    }
}
elsif ($ARGV[0] eq 'verify') {
    my ($upgrade, $package, @packages);

    shift;
    GetOptions("location=s" => \$location, "upgrade" => \$upgrade);

    verify_packages("packages" => \@ARGV, "location" => $location, "upgrade" => $upgrade);
}
elsif ($ARGV[0] eq 'query') {
    my ($case, $nocase, $searchtag, $searchRE);
    shift;
    GetOptions("nocase" => \$nocase, "case" => \$case,
                "searchtag=s" => \$searchtag );
    $searchRE = shift;
    if (!defined $nocase) {
        if (!defined $case) {
            $nocase = $Ignorecase;  # use the default setting
        }
    }
    if (defined $searchtag) {
        if (!$searchtag =~ /(abstract|author|title)/i) {
            print $usage;
            exit 1;
        }
    }
    my %packages = QueryInstalledPackages("ignorecase" => $nocase,
                         "searchtag" => $searchtag, "searchRE" => $searchRE);
    if (!%packages && defined $PPM::PPMERR) {
        print "$PPM::PPMERR\n";
        exit 1;
    }
    my $package;
    foreach $package (sort keys %packages) {
        print "\t$package" . (defined $searchtag ? ": $packages{$package}\n" : "\n");
    }
}
elsif ($ARGV[0] eq 'search') {
    my ($case, $nocase, $searchtag, %ppds, $searchRE);
    shift;
    GetOptions("nocase" => \$nocase, "case" => \$case,
                "location=s" => \$location, "searchtag=s" => \$searchtag );
    $searchRE = shift;
    if (defined $case) {
        $nocase = "No";
    }
    elsif (defined $nocase) {
        $nocase = "Yes";
    }
    else {
        $nocase = $Ignorecase;
    }
    if (defined $searchtag && !($searchtag =~ /(abstract|author|title)/i)) {
        print $usage;
        exit 1;
    }

    search_PPDs("location" => $location, "searchtag" => $searchtag, 
                "searchRE" => $searchRE, "ignorecase" => $nocase);
}
else {
    print $usage;
    exit 1;
}

exit 0;

sub set
{
    my ($option) = shift @_;
    my $rc = 0;
    if (defined $option) {
        my ($value) = shift @_;
        if ($option =~ /^repository$/i) {
            if ($value =~ /\/remove/i) {
                $value = shift @_;
                while (@_) {
                    $value .= " " . shift @_;
                }
                if (!defined $value) {
                    print "Location not specified.\n";
                    $rc = 1;
                }
                else {
                    PPM::RemoveRepository("repository" => $value);
                    %repositories = PPM::ListOfRepositories();
                }
            }
            else {
                my $location = shift @_;
                if (!defined $value || !defined $location) {
                    print "Repository not specified.\n";
                    $rc = 1;
                }
                else {
                    PPM::AddRepository("repository" => $value,
                                       "location" => $location);
                    %repositories = PPM::ListOfRepositories();
                }
            }
        }
        else {
            if ($option =~ /^confirm$/i) {
                if (defined $value) {
                    if ($value =~ /^Yes$/) { $confirm = $value; }
                    elsif ($value =~ /^No$/) { $confirm = $value; }
                    else {
                        print "Value must be 'Yes' or 'No'.\n";
                        $rc = 1;
                        return $rc;
                    }
                }
                else { $confirm = $confirm eq "Yes" ? "No" : "Yes"; }
                print "Commands will " . ($confirm eq "Yes" ? "" : "not ") . "be confirmed.\n";
            }
            elsif ($option =~ /^save$/i) {
                PPM::SetPPMOptions("options" => \%options, "save" => 1);
                return $rc;
            }
            elsif ($option =~ /^case$/i) {
                if (defined $value) {
                    # N.B. These are reversed.
                    if ($value =~ /^Yes$/) { $Ignorecase = "No"; }
                    elsif ($value =~ /^No$/) { $Ignorecase = "Yes"; }
                    else {
                        print "Value must be 'Yes' or 'No'.\n";
                        $rc = 1;
                        return $rc;
                    }
                }
                else { $Ignorecase = $Ignorecase eq "Yes" ? "No" : "Yes"; }
                print "Case-" . ($Ignorecase eq "Yes" ? "in" : "") . "sensitive searches will be performed.\n";
            }
            elsif ($option =~ /^root$/i) {
                my $old_root;
                if (!defined $value) {
                    print "Directory not specified.\n";
                    $rc = 1;
                }
                elsif(!($old_root = PPM::chroot("location" => $value))) {
                    print "$PPM::PPMERR";
                    $rc = 1;
                }
                else {
                    $root = $value;
                    print "Root is now $value [was $old_root].\n";
                }
            }
            elsif ($option =~ /^build$/i) {
                if (!defined $value) {
                    print "Directory not specified.\n";
                    $rc = 1;
                }
                elsif (-d $value) {
                    $build_dir = $value;
                    print "Build directory is now $value.\n";
                }
                else {
                    print "Directory '$value' does not exist.\n";
                    $rc = 1;
                }
            }
            elsif ($option =~ /^force_install$/i) {
                if (defined $value) {
                    if ($value =~ /^Yes$/) { $force_install = $value; }
                    elsif ($value =~ /^No$/) { $force_install = $value; }
                    else {
                        print "Value must be 'Yes' or 'No'.\n";
                        $rc = 1;
                        return $rc;
                    }
                }
                else { $force_install = $force_install eq "Yes" ? "No" : "Yes"; }
                print "Package installations will " .
                      ($force_install eq "Yes" ? "" : "not ") .
                      "continue if a dependency cannot be installed.\n";
            }
            elsif ($option =~ /^clean$/i) {
                if (defined $value) {
                    if ($value =~ /^Yes$/) { $clean = $value; }
                    elsif ($value =~ /^No$/) { $clean = $value; }
                    else {
                        print "Value must be 'Yes' or 'No'.\n";
                        $rc = 1;
                        return $rc;
                    }
                }
                else { $clean = $clean eq "Yes" ? "No" : "Yes"; }
                print "Temporary files will " . ($clean eq "Yes" ? "" : "not ") . "be deleted.\n";
            }
            elsif ($option =~ /^more$/i) {
                if (defined $value && $value =~ /^\d+$/) {
                    $more = $value;
                }
                else {
                    print "Numeric value must be given.\n";
                    $rc = 1;
                    return $rc;
                }
                print "Screens will " . ($more > 0 ? "pause after $more lines.\n" : "not pause.\n");
            }
            else {
                print "Unknown option '$option'; see 'help set' for available options.\n";
                $rc = 1;
            }
            if (!$rc) {
                PPM::SetPPMOptions("options" => \%options);
            }
        }
    }
    else {
        print "Commands will " . ($confirm eq "Yes" ? "" : "not ") . "be confirmed.\n";
        print "Temporary files will " . ($clean eq "Yes" ? "" : "not ") . "be deleted.\n";
        print "Case-" . ($Ignorecase eq "Yes" ? "in" : "") . "sensitive searches will be performed.\n";
        print "Package installations will " . ($force_install eq "Yes" ? "" : "not ") . "continue if a dependency cannot be installed.\n";
        print "Screens will " . ($more > 0 ? "pause after $more lines.\n" : "not pause.\n");
        if (defined $location) { print "Current PPD repository: $location\n"; }
        else {
            print "Current PPD repository paths:\n";
            my $location;
            foreach $_ (keys %repositories) {
                print "\t$_: $repositories{$_}\n";
            }
        }
        if (defined $root) { print "Packages will be installed under: $root\n"; }
        if (defined $build_dir) { print "Packages will be built under: $build_dir\n"; }
    }
    return $rc;
}

sub search_PPDs
{
    my (%argv) = @_;
    my ($arg, $ignorecase, $searchtag, $searchRE, $oldfh);
    local $location = $location;
    foreach $arg (keys %argv) {
        if ($arg eq 'location') { $location = $argv{$arg}; }
        if ($arg eq 'searchtag') { $searchtag = $argv{$arg}; }
        if ($arg eq 'ignorecase') { $ignorecase = $argv{$arg}; }
        if ($arg eq 'searchRE' && defined $argv{$arg}) {
            $searchRE = $argv{$arg};
            eval { $searchRE =~ /$searchRE/ };
            if ($@) {
                print "'$searchRE': invalid regular expression.\n";
                return;
            }
        }
    }
    if (!defined $ignorecase) {
        $ignorecase = $Ignorecase;
    }
    my (%ppds, $loc, $package);
    %ppds = PPM::RepositoryPackages("location" => $location);
    foreach $loc (keys %ppds) {
        if (!defined $searchtag) {
            print "Packages available from $loc:\n";
            my $lines = 0;
            foreach $package (@{$ppds{$loc}}) {
                if (defined $searchRE) {
                    if ($ignorecase eq "Yes" && $package =~ /$searchRE/i) {
                        print "\t" . $package . "\n";
                    }
                    elsif ($package =~ /$searchRE/) {
                        print "\t" . $package . "\n";
                    }
                }
                else {
                    print "\t" . $package . "\n";
                }
                if ($interactive && $more && ++$lines == $more) {
                    print $moremsg;
                    $_ = <>;
                    $lines = 0;
                }
            }
        }
        else {
            print "Matching packages found at $loc:\n";
            my $lines = 0;
            foreach $package (@{$ppds{$loc}}) {
                my ($string);
                if ($string = QueryPPD("package" => $package, "location" => $loc,
                           "ignorecase" => $ignorecase, "searchtag" => $searchtag,
                           "searchRE" => $searchRE)) {
                    print "\t $package: $string\n";
                    if ($interactive && $more && ++$lines == $more) {
                        print $moremsg;
                        $_ = <>;
                        $lines = 0;
                    }
                }
            }
        }
    }
}

sub verify_packages
{
    my (%argv) = @_;
    my ($arg, @packages, $upgrade);
    local $location = $location;
    foreach $arg (keys %argv) {
        if ($arg eq 'packages') { @packages = @{$argv{$arg}}; }
        if ($arg eq 'location') { $location = $argv{$arg}; }
        if ($arg eq 'upgrade') { $upgrade = $argv{$arg}; }
    }
    my ($package);

    if (!defined $packages[0]) {
        my ($i, %info);

        @packages = ();
        %info = QueryInstalledPackages();
        foreach $i (keys %info) {
            push @packages, $i;
        }
    }

    $package = shift @packages;

    # for each specified package
    while ($package) {
        my $status = VerifyPackage("package" => $package, "location" => $location, "upgrade" => $upgrade);
        if (defined $status) {
            if ($status eq "0") {
                print "Package \'$package\' is up to date.\n";
            }
            elsif ($upgrade) {
                print "Package $package upgraded to version $status\n";
            }
            else {
                print "An upgrade to package \'$package\' is available.\n";
            }
        }
        else {
            print "Error verifying $package: $PPM::PPMERR\n";
        }
        $package = shift @packages;
    }
}

__END__

=head1 Name

PPM - Perl Package Manager: locate, install, upgrade software packages.

=head1 Synopsis

 ppm help [command]
 ppm install [--location=location] package1 [... packageN]
 ppm query [--case|nocase] [--searchtag=abstract|author|title] PATTERN
 ppm remove package1 [... packageN]
 ppm search [--case|nocase] [--location=location] [--searchtag=abstract|author|title] PATTERN
 ppm set [option]
 ppm verify [--location=location] [--upgrade] [package1 ... packageN]
 ppm version
 ppm [--location=location]

=head1 Description

ppm is a utility intended to simplify the tasks of locating, installing,
upgrading and removing software packages.  It is a front-end to the
functionality provided in PPM.pm.  It can determine if the most recent
version of a software package is installed on a system, and can install
or upgrade that package from a local or remote host.

ppm runs in one of two modes: an interactive shell from which commands
may be entered; and command-line mode, in which one specific action is
performed per invocation of the program.

ppm uses files containing an extended form of the Open Software
Description (OSD) specification for information about software packages.
These description files, which are written in Extensible Markup
Language (XML) code, are referred to as 'PPD' files.  Information about
OSD can be found at the W3C web site (at the time of this writing,
http://www.w3.org/TR/NOTE-OSD.html).  The extensions to OSD used by ppm
are documented in PPM.ppd.

=head1 Command-line mode

=over 4

=item Installing

ppm install [--location=location] package1 [... packageN]

Reads information from the PPD file (See the 'Files' section
below) for the named software package and performs an
installation.  The 'package' arguments may be either package
names ('foo'), or pathnames (P:\PACKAGES\FOO.PPD) or URLs
(HTTP://www.ActiveState.com/packages/foo.ppd) to specific PPD files.

In the case where a package name is specified, and the '--location'
option is not used, the function will refer to repository locations stored
in the PPM data file (see 'Files' section below) to locate the PPD file
for the requested package.

=item Removing

ppm remove package1 [... packageN]

Reads information from the PPD file for the named software package and
removes the package from the system.

=item Verifying

ppm verify [--location=location] [--upgrade] [package1 ... packageN]

Reads a PPD file for the specified package and compares the currently
installed version of the package to the version available according to
the PPD.  The PPD file is expected to be on a local directory or remote
site specified either in the PPM data file or on the command
line using the '--location' option.  The --location' argument may be
a directory location or an Internet address.  The '--upgrade' option
forces an upgrade if the installed package is not up-to-date.

If no packages are specified, all packages currently installed on the
system will be verified (and updated if desired).  The PPD file for each
package will initially be searched for at the location specified with the
'--location' argument, and if not found will then be searched for using
the location specified in the PPM data file.

=item Querying

ppm query [--case|nocase] PATTERN

Reports currently installed packages matching 'PATTERN' or all installed
packages if no 'PATTERN' is specified.

ppm query [--case|nocase] [--searchtag=abstract|author|title] PATTERN

Searches for 'PATTERN' (a regular expression) in the <ABSTRACT>, <AUTHOR>
or <TITLE> tags of all installed packages, according to the value of
the '--searchtag' option.  If a '--searchtag' value of 'abstract',
'author' or 'title' is not provided, any occurence of 'PATTERN' in the
package name will match successfully.  A case-sensitive search will be
conducted by default, but this may be overridden by the options set in
the PPM data file, which may in turn be overridden by the '--nocase' or
'--case' option.  If a search is successful, the name of the package
and the matching string are displayed.

=item Searching

ppm search [--case|nocase] [--location=location] PATTERN

Displays a list of all packages matching 'PATTERN', with package
description (PPD) files available at the specified location.  'location'
may be either a remote address or a directory path.  If a location is
not specified, the repository location as specified in the PPM data file
will be used.

ppm search [--case|nocase] [--location=location] [--searchtag=abstract|author|title] PATTERN

Searches for 'PATTERN' (a regular expression) in the <ABSTRACT>, <AUTHOR>
or <TITLE> tags of all PPD files at 'location', according to the value
of the '--searchtag' option.  If a '--searchtag' value of 'abstract',
'author' or 'title' is not provided, any occurence of 'PATTERN' in
the package name will match successfully.  'location' may be either a
remote address or a directory path, and if it is not provided, repository
locations specified in the PPM data file will be used.  A case-sensitive
search will be conducted by default, but this may be overridden by the
options set in the PPM data file, which may in turn be overridden by the
'--nocase' or '--case' option.  If a search is successful, the name of
the package and the matching string are displayed.

=back

=head1 Interactive mode

If ppm is invoked with no command specified, it is started in interactive
mode.  If the '--location' argument is specified, it is used as the
search location, otherwise the repositories specified in the PPM data file are 
used. The available commands, which may be displayed at any time by entering
'help', are:

    help [command]
        - Prints a screen of available commands, or help on a specific command.

    install [/location LOCATION] PACKAGES
        - Installs the specified software PACKAGES.  Attempts to install
          from the URL or directory 'LOCATION' if the '/location' option
          is specfied.  See 'Installing' in the 'Command-line mode' 
          section for details.  See also: 'confirm' option.

    query [options] PATTERN
        - Queries information about currently installed packages.

        Available options:
        /abstract PATTERN
            - Searches for the regular expression 'PATTERN' in the 'abstract' section
              of all installed packages.  See also: 'case' option.
        /author PATTERN
            - Searches for the regular expression 'PATTERN' in the 'author' section
              of all installed packages.  See also: 'case' option.
        /title PATTERN
            - Searches for the regular expression 'PATTERN' in the 'title' section
              of all installed packages.  See also: 'case' option.

    remove PACKAGES
        - Removes the specified 'PACKAGES'.  See 'Removing' in the 'Command-line 
          mode' section for details.  See also: 'confirm' option.

    search [options] PATTERN
        - Searches for information about available packages.

        Available options:
        /abstract PATTERN
            - Searches for the regular expression 'PATTERN' in the 'abstract' section
              of all available PPD files.  See also: 'case' option.
        /author PATTERN
            - Searches for the regular expression 'PATTERN' in the 'author' section
              of all available PPD files.  See also: 'case' option.
        /title PATTERN
            - Searches for the regular expression 'PATTERN' in the 'title' section
              of all available PPD files.  See also: 'case' option.
        /location LOCATION
            - Searches for packages available from the URL or directory
              'LOCATION'.

    set [option value]
        - Sets or displays current options.  With no arguments, options are
          displayed.

          Available options:
              build DIRECTORY
                  - Changes the package build directory.
              case [Yes|No]
                  - Sets case-sensitive searches.  If one of 'Yes' or 'No is
                    not specified, the current setting is toggled.
              clean [Yes|No]
                  - Sets removal of temporary files from package's build 
                    area, on successful installation of a package.  If one of
                    'Yes' or 'No is not specified, the current setting is
                    toggled.
              confirm [Yes|No]
                  - Sets confirmation of 'install', 'remove' and 'upgrade'.
                    If one of 'Yes' or 'No is not specified, the current
                    setting is toggled.
              force_install [Yes|No]
                  - Continue installing a package even if a dependency cannot
                    be installed.
              more NUMBER
                  - Causes output to pause after NUMBER lines have been
                    displayed.  Specifying '0' turns off this capability.
              set repository /remove NAME
                  - Removes the repository 'NAME' from the list of repositories.
              set repository NAME LOCATION
                  - Adds a repository to the list of PPD repositories for this
                    session.  'NAME' is the name by which this repository will
                    be referred; 'LOCATION' is a URL or directory name.
              root DIRECTORY
                  - Changes the install root directory.  Packages will be
                    installed under this new root.
              save
                  - Saves the current options as default options for future
                    sessions.

    quit
        - Exits the program.

    verify [/upgrade] [/location LOCATION] PACKAGE
        - Verifies that currently installed 'PACKAGE' is up to date.  If
          'PACKAGE' is not specified, all installed packages are verified.  If
          the /upgrade option is specified, any out-dated packages will be
          upgraded.  If the /location option is specified, upgrades will
          be looked for at the URL or directory 'LOCATION'.  See also: 'confirm'
          option.

=over 4

=item 

=back

=head1 Examples

=over 4

=item ppm

Starts ppm in interactive mode, using the repository locations specified
in the PPM data file.  A session might look like this:

    [show all available packages]
    PPM> search
    Packages available from P:\PACKAGES:
            bar
            baz
            foo
    [list what has already been installed]
    PPM> query
            baz
    [install a package]
    PPM> install foo
    Install package foo? (y/N): y
    [...]
    [toggle confirmations]
    PPM> set confirm
    Commands will not be confirmed.
    [see if 'baz' is up-to-date]
    PPM> verify baz
    An upgrade to package 'baz' is available.
    [upgrade 'baz']
    PPM> verify /upgrade baz
    [...]
    [toggle case-sensitive searches]
    PPM> set case
    Case-sensitive searches will be performed.
    [display the abstract for all available packages]
    PPM> query /abstract
            bar: Bars foos
            baz: Tool for bazing and cleaning BAZs.
            [...]
    [search for packages containing a certain regex in the /ABSTRACT tag]
    PPM> query /abstract clean.*baz
    Matching packages found at P:\PACKAGES:
             baz: Tool for bazing and cleaning BAZs.
    PPM> quit

=item ppm install http://www.ActiveState.com/packages/foo.ppd

Installs the software package 'foo' based on the information in the PPD
obtained from the specified URL.

=item ppm verify --upgrade foo

Compares the currently installed version of the software package 'foo'
to the one available according to the PPD obtained from the location
specified for this package in the PPM data file, and upgrades
to a newer version if available.

=item ppm verify --location=P:\PACKAGES --upgrade foo

Compares the currently installed version of the software package 'foo'
to the one available according to the PPD obtained from the specified
directory, and upgrades to a newer version if available.

=item ppm verify --upgrade

Verifies and updates every installed package on the system, using upgrade
locations specified in the PPM data file.

=item ppm search --location=http://www.ActiveState.com/packages

Displays the packages with PPD files available at the specified location.

=item ppm search --location=P:\PACKAGES --searchtag=author ActiveState

Searches the specified location for any package with an <AUTHOR> tag
containing the string 'ActiveState'.  On a successful search, the package
name and the matching string are displayed.

=back

=head1 Environment variables

=over 4

=item HTTP_proxy

If the environment variable 'HTTP_proxy' is set, then it will
be used as the address of a proxy server for accessing the Internet.

=back

=head1 Files

These files are fully described in the 'Files' section of PPM:ppm.

=over 4

=item package.ppd

A description of a software package, in extended Open Software Description
(OSD) format.  More information on this file format can be found in
PPM::ppd.

=item ppm.xml - PPM data file.

This file is specified using the environment variable 'PPM_DAT';  if this
is not set, the file 'ppm.xml' is expected to be located in the same 
directory as this script.

An XML format file containing information about the local system,
specifics regarding the locations from which PPM obtains PPD files, and
the installation details for any package installed by ppm.

=back

=cut

