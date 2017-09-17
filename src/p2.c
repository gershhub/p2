#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include "p2.h"


p2_t* p2_create(void){
    p2_t *p2 = malloc(sizeof(p2_t));
    if(!p2) return NULL;
    
    memset(p2, 0, sizeof(p2_t));
    p2->p2_count = 0;
    
    p2_add_end_markers( p2 );
    return p2;
}


void p2_destroy(p2_t* p2){
    if(!p2) return;
    
    free(p2->p2_q);
    free(p2->p2_n);
    free(p2->p2_dn);
    free(p2->p2_np);
    free(p2);
}


void p2_add_end_markers( p2_t *p2 ){
	p2->p2_marker_count = 2;
	p2->p2_q = malloc( sizeof( double[p2->p2_marker_count])) ;
	p2->p2_dn = malloc( sizeof( double[ p2->p2_marker_count ]));
	p2->p2_np = malloc( sizeof( double[ p2->p2_marker_count ]));
	p2->p2_n = malloc( sizeof( int[ p2->p2_marker_count ] ));
	p2->p2_dn[0] = 0.0;
	p2->p2_dn[1] = 1.0;

	p2_update_markers( p2 );
}

double *p2_allocate_markers( p2_t *p2, int count ){
	double *p2_newq = malloc( sizeof( double[ p2->p2_marker_count + count ] ));
	double *p2_newdn = malloc( sizeof(  double[ p2->p2_marker_count + count ] ));
	double *p2_newnp = malloc( sizeof(  double[ p2->p2_marker_count + count ] ));
	int *p2_newn = malloc( sizeof(  int[ p2->p2_marker_count + count ] ));

	memcpy( p2_newq, p2->p2_q, sizeof(double) * p2->p2_marker_count );
	memcpy( p2_newdn, p2->p2_dn, sizeof(double) * p2->p2_marker_count );
	memcpy( p2_newnp, p2->p2_np, sizeof(double) * p2->p2_marker_count );
	memcpy( p2_newn, p2->p2_n, sizeof(int) * p2->p2_marker_count );

	free(p2->p2_q);
	free(p2->p2_dn);
	free(p2->p2_np);
	free(p2->p2_n);

	p2->p2_q = p2_newq;
	p2->p2_dn = p2_newdn;
	p2->p2_np = p2_newnp;
	p2->p2_n = p2_newn;

	p2->p2_marker_count += count;

	return p2->p2_dn + p2->p2_marker_count - count;
}

void p2_update_markers( p2_t *p2 ){
	p2_sort( p2->p2_dn, p2->p2_marker_count );

	/* Then entirely reset np markers, since the marker count changed */
	for( int i = 0; i < p2->p2_marker_count; i ++ ) {
		p2->p2_np[ i ] = (p2->p2_marker_count - 1) * p2->p2_dn[ i ] + 1;
	}
}

void p2_add_quantile(p2_t* p2, double quant ){
    double *markers = p2_allocate_markers( p2, 3 );

    /* Add in appropriate dn markers */
    markers[0] = quant;
    markers[1] = quant/2.0;
    markers[2] = (1.0+quant)/2.0;

    p2_update_markers( p2 );
}


void p2_add_equal_spacing(p2_t* p2, int count ){
    double *markers = p2_allocate_markers( p2, count - 1 );

    /* Add in appropriate dn markers */
    for( int i = 1; i < count; i ++ ) {
        markers[ i - 1 ] = 1.0 * i / count;
    }

    p2_update_markers( p2 );
}

int sign( double d ){
	if( d >= 0.0 ) {
		return 1.0;
	} else {
		return -1.0;
	}
}

// Simple bubblesort, because bubblesort is efficient for small count, and
// count is likely to be small
void p2_sort( double *q, int count ){
	double k;
	int i, j;
	for( j = 1; j < count; j ++ ) {
		k = q[ j ];
		i = j - 1;
		
		while( i >= 0 && q[ i ] > k ) {
			q[ i + 1 ] = q[ i ];
			i --;
		}
		q[ i + 1 ] = k;
	}
}

double p2_parabolic( p2_t * p2, int i, int d ){
	return p2->p2_q[ i ] + d / (double)(p2->p2_n[ i + 1 ] - p2->p2_n[ i - 1 ]) * ((p2->p2_n[ i ] - p2->p2_n[ i - 1 ] + d) * (p2->p2_q[ i + 1 ] - p2->p2_q[ i ] ) / (p2->p2_n[ i + 1] - p2->p2_n[ i ] ) + (p2->p2_n[ i + 1 ] - p2->p2_n[ i ] - d) * (p2->p2_q[ i ] - p2->p2_q[ i - 1 ]) / (p2->p2_n[ i ] - p2->p2_n[ i - 1 ]) );
}

double p2_linear( p2_t * p2, int i, int d )
{
	return p2->p2_q[ i ] + d * (p2->p2_q[ i + d ] - p2->p2_q[ i ] ) / (p2->p2_n[ i + d ] - p2->p2_n[ i ] );
}

void p2_add(p2_t* p2, double data ){
    int i;
    int k;
    double d;
    double newq;

    if( p2->p2_count >= p2->p2_marker_count ) {
        p2->p2_count ++;

        // B1
        if( data < p2->p2_q[0] ) {
            p2->p2_q[0] = data;
            k = 1;
        } else if( data >= p2->p2_q[p2->p2_marker_count - 1] ) {
            p2->p2_q[p2->p2_marker_count - 1] = data;
            k = p2->p2_marker_count - 1;
        } else {
            for( i = 1; i < p2->p2_marker_count; i ++ ) {
                if( data < p2->p2_q[ i ] ) {
                    k = i;
                    break;
                }
            }
        }

        // B2
        for( i = k; i < p2->p2_marker_count; i ++ ) {
            p2->p2_n[ i ] ++;
            p2->p2_np[ i ] = p2->p2_np[ i ] + p2->p2_dn[ i ];
        }
        for( i = 0; i < k; i ++ ) {
            p2->p2_np[ i ] = p2->p2_np[ i ] + p2->p2_dn[ i ];
        }

        // B3
        for( i = 1; i < p2->p2_marker_count - 1; i ++ ) {
            d = p2->p2_np[ i ] - p2->p2_n[ i ];
            if( (d >= 1.0 && p2->p2_n[ i + 1 ] - p2->p2_n[ i ] > 1)
             || ( d <= -1.0 && p2->p2_n[ i - 1 ] - p2->p2_n[ i ] < -1.0)) {
                newq = p2_parabolic( p2, i, sign( d ) );
                if( p2->p2_q[ i - 1 ] < newq && newq < p2->p2_q[ i + 1 ] ) {
                    p2->p2_q[ i ] = newq;
                } else {
                    p2->p2_q[ i ] = p2_linear( p2, i, sign( d ) );
                }
                p2->p2_n[ i ] += sign(d);
            }
        }
    } else {
        p2->p2_q[ p2->p2_count ] = data;
        p2->p2_count ++;

        if( p2->p2_count == p2->p2_marker_count ) {
            // We have enough to start the algorithm, initialize
            p2_sort( p2->p2_q, p2->p2_marker_count );

            for( i = 0; i < p2->p2_marker_count; i ++ ) {
                p2->p2_n[ i ] = i + 1;
            }
        }
    }
}

double p2_result( p2_t* p2, double quantile ){
    if( p2->p2_count < p2->p2_marker_count ) {
        int closest = 1;
        p2_sort(p2->p2_q, p2->p2_count);
        for( int i = 2; i < p2->p2_count; i ++ ) {
            if( fabs(((double)i)/p2->p2_count - quantile) < fabs(((double)closest)/p2->p2_marker_count - quantile ) ) {
                closest = i;
            }
        }
        return p2->p2_q[ closest ];
    } else {
        // Figure out which quantile is the one we're looking for by nearest dn
        int closest = 1;
        for( int i = 2; i < p2->p2_marker_count -1; i ++ ) {
            if( fabs(p2->p2_dn[ i ] - quantile) < fabs(p2->p2_dn[ closest ] - quantile ) ) {
                closest = i;
            }
        }
        return p2->p2_q[ closest ];
    }
}

