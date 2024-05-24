## Running annotate_af

```annotate_af``` is written in C, and uses HTSLib functions. HTSLib is required to compile and execute.

If an HTSLib module is already available on your system, it should be loaded before compiling or running the executable:

```
module spider htslib
module load bioinf/htslib/1.9
```

If HTSLib is unavailable on your system, you can [download HTSLib here](https://www.htslib.org/download/) and [install from source](https://github.com/samtools/htslib/blob/develop/INSTALL). If ```annotate_af``` can't find HTSLib at runtime, execution will fail with ```./annotate_af: error while loading shared libraries: libhts.so.2: cannot open shared object file: No such file or directory```. I compile and run with HTSLib 1.9; other versions of HTSLib have not been tested and may cause unexpected behaviour.

```annotate_af``` expects two arguments:
* An input VCF file (- for STDIN); these can be compressed or uncompressed, indexed or unindexed
* An output VCF path (- for STDOUT)

To test whether ```annotate_af``` is working correctly, you can run the following commands:

```
./annotate_af test/test.vcf.gz test/test_af_annotated.vcf
./annotate_af test/test_merged.vcf.gz test/test_merged_af_annotated.vcf
```

If you want to receive input from STDIN or write to STDOUT as part of a pipe, you can use the following syntax:

```
cat test/test.vcf.gz | ./annotate_af - - > test/test_af_annotated.vcf
cat test/test_merged.vcf.gz | ./annotate_af - - > test/test_merged_af_annotated.vcf
```

The GATK HaplotypeCaller with human germline presets, by default, assumes all sites are diploid. This assumption yields incorrect allele frequencies on chrX, chrY, and chrMT in the human genome when computed from uncorrected GT. To correct ploidy on these chromosomes prior to the calculation of allele frequencies, the following syntax can be used:

```
cat test/test.vcf.gz | bcftools +fixploidy test_merged.vcf.gz -- -s sample_sex.txt -p ploidy_regions.txt | ./annotate_af - - > test/test_af_annotated.vcf
cat test/test_merged.vcf.gz | bcftools +fixploidy test_merged.vcf.gz -- -s sample_sex.txt -p ploidy_regions.txt | ./annotate_af - - > test/test_merged_af_annotated.vcf
```

The ```bcftools +fixploidy``` plugin requires two additional files: one file of sexes for all samples in the VCF, formatted like the one in ```test/```, and a set of regions and ploidies to correct. For more information about options and syntax of bcftools plugins, see the [bcftools manual page](https://samtools.github.io/bcftools/bcftools.html).

## Compiling annotate_af

If you need to compile ```annotate_af``` from source, first load HTSLib. You can then compile the executable using the following command:

```gcc -o annotate_af vcf_annotate_af.c -lhts -lz```

Compilation was done using gcc 10.2.0 on Linux.
