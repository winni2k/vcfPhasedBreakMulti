# Test file created outside of h2xs framework.
# Run this like so: `perl 10-multi.t'
#   wkretzsch@gmail.com     2014/04/28 09:42:12

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test::More;
BEGIN { plan tests => 4 }

use Test::Files;
use warnings;
use strictures;
$| = 1;
use File::Path qw/make_path/;

ok(1);    # If we made it this far, we're ok.

#########################

my $expNum   = 15;
my $exampDir = "../examples";
my $expDir   = "${expNum}-expected";
my $resDir   = "results/${expNum}-results";

make_path($resDir);

## Please see file perltidy.ERR
my $bin    = "vcfPhasedBreakMulti";
my $inFile = "$exampDir/multiall.faulty.vcf";
ok( system("../bin/$bin < $inFile  2> /dev/null 1> /dev/null") != 0,
    "$bin failed as expected" );

ok(
    system(
"../bin/$bin --malformedLinesFile $resDir/multiall.brokenLines.txt < $inFile  > /dev/null"
      ) == 0,
    "$bin completed successfully"
);

compare_filter_ok( $inFile, $inFile, \&noChrom,
    "$bin result matches expectation" );

sub noChrom {
    my $line = shift;
    return $line if $line !~ m/^#CHROM/;
}
