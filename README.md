vcfPhasedBreakMulti
===================

Very simple, but hopefully fast <code>boost::spirit::qi</code> based VCF GT field parser for breaking multiallelic variants into multiple biallelic variants


Synopsis
--------

See all command line options

    ./vcfPhasedBreakMulti --help

Break multiallelics into biallelics
    ./vcfPhasedBreakMulti < input.vcf > output.vcf

Don't die when encountering malformed VCF lines.  Write the malformed
VCF lines to malformed.txt
    ./vcfPhasedBreakMulti --malformedLinesFile malformed.txt < input.vcf >
    output.vcf

Keep INFO field when breaking multiallelics into multiple lines.
    ./vcfPhasedBreakMulti --keepInfo < input.vcf > output.vcf

Instead of breaking the multiallelics into multiple lines, simply
recode all alternative alleles as a single allele.
    ./vcfPhasedBreakMulti --recodeAsBiallelic < input.vcf > output.vcf

Input file
----------

Needs to be a valid VCF file with only a GT format field.  All genotypes need to be phased.  I use the following commands to get an input file that matches those requirements:

```
# strip everything but GT field using binary from vcflib
vcflib/bin/vcfkeepgeno file1.vcf GT > file1.onlyGT.vcf

# now split multiallelic sites
/vcfPhasedBreakMulti < file1.onlyGT.vcf > file1.onlyGT.asBiAllelics.vcf
```

Acknowledgements
----------------

The QI parser was modified from code kindly shared by Leo Goodstadt.
