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
* An output VCF path (> for STDOUT)

## Compiling annotate_af

If you need to compile ```annotate_af``` from source, first load HTSLib. You can then compile the executable using the following command:

```gcc -o annotate_af vcf_annotate_af.c -lhts -lz```

Compilation was done using gcc 10.2.0 on Linux.
