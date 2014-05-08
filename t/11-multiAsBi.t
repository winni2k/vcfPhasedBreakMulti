# Test file created outside of h2xs framework.
# Run this like so: `perl 10-multi.t'
#   wkretzsch@gmail.com     2014/04/28 09:42:12

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test::More;
BEGIN { plan tests => 3 }

use Test::Files;
use warnings;
use strictures;
$| = 1;
use File::Path qw/make_path/;

ok(1);    # If we made it this far, we're ok.

#########################

my $expNum   = 11;
my $exampDir = "../examples";
my $expDir   = "${expNum}-expected";
my $resDir   = "results/${expNum}-results";

make_path($resDir);

my $bin = "vcfPhasedBreakMulti";

ok(
    system("../bin/$bin --recodeAsBiallelic < $exampDir/multiall.vcf > $resDir/multiall.res.vcf")
      == 0,
    "$bin called successfully"
);

compare_ok(
    "$resDir/multiall.res.vcf",
    "$expDir/multiall.expected.vcf",
    "$bin result matches expectation"
);

