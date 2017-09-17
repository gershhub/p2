#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "p2.h"


int main(int argc, char *argv[]){
    // We will calculate the 10, 50, and 90th percentile in several different ways to compare accuracy
    // p2_10, p2_50 and p2_90 will each be used to calculate a single percentile
	p2_t *p2_10 = p2_create();
    p2_add_quantile(p2_10, 0.1);
    
    p2_t *p2_50 = p2_create();
    p2_add_quantile(p2_50, 0.5);
    
    p2_t *p2_90 = p2_create();
    p2_add_quantile(p2_90, 0.9);
    
    // multi will be used to simultaneously calculate the 10, 50 and 90 percentiles
	p2_t *multi = p2_create();
    // equal will track all deciles (0%, 10%, 20% ... 90%, 100%)
    // densequal will track all multiples of 5% (0%, 5%, 10% ... 95%, 100%)
	p2_t *equal = p2_create(); 
    p2_t *denseequal = p2_create();
	FILE *fi;
	double d;
	char buf[30];

	p2_add_quantile(multi, 0.1);
	p2_add_quantile(multi, 0.5);
	p2_add_quantile(multi, 0.9);
	
    p2_add_equal_spacing(equal, 10);
	p2_add_equal_spacing(denseequal, 20);

	if( argc != 2 ) {
		printf( "No file specified\n");
		exit(1);
	}

	fi = fopen(argv[1], "r");

	if( fi == NULL ) {
		printf( "Failed to open %s\n", argv[1] );
		exit(1);
	}

    // Read data from a file, one value per line. Add it into each of the p2_t trackers
	while(true) {
		fgets(buf, 30, fi);
        if(feof(fi)) break;
		d = strtod(buf, NULL);

		p2_add(p2_10, d );
		p2_add(p2_50, d );
		p2_add(p2_90, d );
		p2_add(multi, d );
		p2_add(equal, d );
		p2_add(denseequal, d );
	}
	fclose(fi);

    // Print out the results from each of the trackers at the 10, 50 and 90 percentiles
	printf("%g %g %g\n", p2_result( p2_10, 0.1 ), p2_result( p2_50, 0.5 ), p2_result( p2_90, 0.9 ) );
	printf("%g %g %g\n", p2_result( multi, 0.1 ), p2_result( multi, 0.5 ), p2_result( multi, 0.9 ) );
	printf("%g %g %g\n", p2_result( equal, 0.1 ), p2_result( equal, 0.5 ), p2_result( equal, 0.9 ) );
	printf("%g %g %g\n", p2_result( denseequal, 0.1 ), p2_result( denseequal, 0.5 ), p2_result( denseequal, 0.9 ) );
}
