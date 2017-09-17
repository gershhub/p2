#ifndef __p2_H__
#define __p2_H__

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


typedef struct {
    double *p2_q;
    double *p2_dn;
    double *p2_np;
    int *p2_n;
    int p2_count;
    int p2_marker_count;
} p2_t;


DLLEXPORT p2_t *p2_create(void);
DLLEXPORT void p2_destroy(p2_t* p2);

// target a particular quantile
DLLEXPORT void p2_add_quantile(p2_t* p2, double quant);

// Set a p^2 structure to target n equally spaced quantiles
DLLEXPORT void p2_add_equal_spacing(p2_t* p2, int n );

// Call to add a data point into the structure
DLLEXPORT void p2_add(p2_t* p2, double data );

// Retrieve the value at a particular quantile.
DLLEXPORT double p2_result(p2_t* p2, double quantile );

void p2_add_end_markers( p2_t *p2 );
double *p2_allocate_markers( p2_t* p2, int count );
void p2_update_markers( p2_t *p2 );
void p2_sort( double *q, int count );
double p2_parabolic( p2_t *p2, int i, int d );
double p2_linear( p2_t *p2, int i, int d );
int sign( double d );

#endif
