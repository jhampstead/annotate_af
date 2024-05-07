#include <stdio.h>
#include <htslib/vcf.h>

int main(int argc, char *argv[]) {
    if (argc!=3) {
        fprintf(stderr, "Usage: %s input.vcf <output.vcf>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    htsFile *fp_in = hts_open(input_file, "r");
    if (fp_in == NULL) {
	    fprintf(stderr, "Error opening input vcf file: %s\n", input_file);
	    return 1;
    } 

    bcf_hdr_t *header = bcf_hdr_read(fp_in);

    const char *new_info_field = "##INFO=<ID=CAF,Number=A,Type=Float,Description=\"Combined AF\">";
    if (bcf_hdr_append(header, new_info_field) != 0) {
        fprintf(stderr, "Failed to add new INFO field to header\n");
        bcf_hdr_destroy(header);
        hts_close(fp_in);
        return 1;
    }

    htsFile *fp_out = hts_open(output_file, "w");
    if (fp_out == NULL) {
	    fprintf(stderr, "Error opening output VCF file: %s\n", output_file);
	    return 1;
    }


    if (bcf_hdr_write(fp_out, header) != 0) {
	    fprintf(stderr, "Error writing header to output VCF file: %s\n", output_file);
	    bcf_hdr_destroy(header);
	    hts_close(fp_in);
	    hts_close(fp_out);
	return 1;
    }


    bcf1_t *r = bcf_init();

    int nsamples = bcf_hdr_nsamples(header);
    
    // Pre-initialize loop variables so it only happens once instead of each loop
    int i, j, allele, n_alts;
    int ngt_arr = 0, *gt_arr = NULL, ngt;

    while(bcf_read(fp_in, header, r) >= 0) {
	if (r->rid > 21) break;

	n_alts = r->n_allele-1;
	int n_alleles_per_sample[n_alts];
	memset(n_alleles_per_sample, 0, sizeof(n_alleles_per_sample));

	ngt = bcf_get_genotypes(header, r, &gt_arr, &ngt_arr);

	for (i = 0; i < nsamples; i++) {
	    for (j = 0; j < 2; j++) { // maximum of two alleles
		allele = bcf_gt_allele(gt_arr[i * 2 + j]);
		if (allele > 0) {
		    n_alleles_per_sample[allele-1]++;
		}
	    }
	}

	float frequencies[n_alts];
	for (j = 0; j < n_alts; j++) {
	    frequencies[j] = (float) n_alleles_per_sample[j] / ngt; 
	}

	bcf_update_info_float(header, r, "CAF", &frequencies, n_alts);
	bcf_write(fp_out, header, r);

	bcf_clear(r);
    }

    bcf_destroy(r);
    bcf_hdr_destroy(header);
    hts_close(fp_in);
    hts_close(fp_out);

    return 0;
}
