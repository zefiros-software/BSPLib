#include "bspedupack.h"

/*  This is a test program which uses bspfft to perform 
    a Fast Fourier Transform and its inverse.

    The input vector is defined by x[j]=j+i, for 0 <= j < n.
    Here i= sqrt(-1).
 
    The output vector should equal the input vector,
    up to roundoff errors. Output is by triples (j, Re x[j], Im x[j]).
    Warning: don't rely on this test alone to check correctness. 
    (After all, deleting the main loop will give similar results ;) 

*/

#define NITERS 5   /* Perform NITERS forward and backward transforms.
                      A large NITERS helps to obtain accurate timings. */
#define NPRINT 10  /* Print NPRINT values per processor */
#define MEGA 1000000.0

int P;

void bspfft_test(){
    void bspfft(double *x, int n, int p, int s, int sign, double *w0,
                double *w, double *tw, int *rho_np, int *rho_p);
    void bspfft_init(int n, int p, int s, double *w0, 
                double *w, double *tw, int *rho_np, int *rho_p);
    int k1_init(int n, int p);

    int p, s, n, q, np, k1, j, jglob, it, *rho_np, *rho_p;
    double time0, time1, time2, ffttime, nflops,
           max_error, error_re, error_im, error,
           *Error, *x, *w0, *w, *tw;
  
    bsp_begin(P);
    p= bsp_nprocs();
    s= bsp_pid();

    bsp_push_reg(&n,SZINT);
    Error= vecallocd(p);
    bsp_push_reg(Error,p*SZDBL);
    bsp_sync();

    if (s==0){
        printf("Please enter length n: \n");
        scanf("%d",&n);
        if(n<2*p)
            bsp_abort("Error in input: n < 2p");
        for (q=0; q<p; q++)
            bsp_put(q,&n,&n,0,SZINT);
    }
    bsp_sync();

    if (s==0){
        printf("FFT of vector of length %d using %d processors\n",n,p);
        printf("performing %d forward and %d backward transforms\n",
                NITERS,NITERS);
    }

    /* Allocate, register,  and initialize vectors */
    np= n/p;
    x= vecallocd(2*np);
    bsp_push_reg(x,2*np*SZDBL);
    k1= k1_init(n,p);
    w0= vecallocd(k1);
    w=  vecallocd(np);
    tw= vecallocd(2*np+p);
    rho_np= vecalloci(np);
    rho_p=  vecalloci(p);

    for (j=0; j<np; j++){
        jglob= j*p+s;
        x[2*j]= (double)jglob;
        x[2*j+1]= 1.0;
    }
    bsp_sync();
    time0= bsp_time();

    /* Initialize the weight and bit reversal tables */
    for(it=0; it<NITERS; it++)
        bspfft_init(n,p,s,w0,w,tw,rho_np,rho_p);
    bsp_sync();
    time1= bsp_time();
  
    /* Perform the FFTs */
    for(it=0; it<NITERS; it++){
        bspfft(x,n,p,s,1,w0,w,tw,rho_np,rho_p);
        bspfft(x,n,p,s,-1,w0,w,tw,rho_np,rho_p);
    }
    bsp_sync();
    time2= bsp_time();

    /* Compute the accuracy */
    max_error= 0.0;
    for(j=0; j<np; j++){
        jglob= j*p+s;
        error_re= fabs(x[2*j]-(double)jglob);
        error_im= fabs(x[2*j+1]-1.0);
        error= sqrt(error_re*error_re + error_im*error_im);
        if (error>max_error)
            max_error= error;
    }
    bsp_put(0,&max_error, Error, s*SZDBL, SZDBL);
    bsp_sync();

    if (s==0){
        max_error= 0.0;
        for(q=0; q<p; q++){
            if (Error[q]>max_error)
                max_error= Error[q];
        }
    }

    for(j=0; j<NPRINT && j<np; j++){
        jglob= j*p+s;
        printf("proc=%d j=%d Re= %f Im= %f \n",s,jglob,x[2*j],x[2*j+1]);
    }
    fflush(stdout);
    bsp_sync();

    if (s==0){
        printf("Time per initialization = %lf sec \n",
                (time1-time0)/NITERS);
        ffttime= (time2-time1)/(2.0*NITERS);
        printf("Time per FFT = %lf sec \n", ffttime);
        nflops= 5*n*log((double)n)/log(2.0) + 2*n;
        printf("Computing rate in FFT = %lf Mflop/s \n",
                nflops/(MEGA*ffttime));
        printf("Absolute error= %e \n", max_error);
        printf("Relative error= %e \n\n", max_error/n);
    }

  
    bsp_pop_reg(x);
    bsp_pop_reg(Error);
    bsp_pop_reg(&n);
    bsp_sync();

    vecfreei(rho_p);
    vecfreei(rho_np);
    vecfreed(tw);
    vecfreed(w);
    vecfreed(w0);
    vecfreed(x);
    vecfreed(Error);
    bsp_end();

} /* end bspfft_test */

int main(int argc, char **argv){
 
    bsp_init(bspfft_test, argc, argv);
 
    printf("How many processors do you want to use?\n"); fflush(stdout);
    scanf("%d",&P);
    if (P > bsp_nprocs()){
        printf("Sorry, not enough processors available.\n"); fflush(stdout);
        exit(1);
    }
 
    bspfft_test();
 
    exit(0);
 
} /* end main */
