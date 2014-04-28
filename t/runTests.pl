#!/bin/perl
use strict;
use warnings;
use TAP::Harness;
use File::Path qw(make_path remove_tree);

my $harness = TAP::Harness->new( );

my $resultsDir = "results";
remove_tree($resultsDir) if -d $resultsDir;
make_path($resultsDir);

my @tests = glob "*.t";
$harness->runtests(sort @tests);
