#include "bspedupack.h"

/*  This is a test program which uses bsplu to decompose an n by n
    matrix A into triangular factors L and U, with partial row pivoting.
    The decomposition is A(pi(i),j)=(LU)(i,j), for 0 <= i,j < n,
    where pi is a permutation.
 
    The input matrix A is a row-rotated version of a matrix B:
        the matrix B is defined by: B(i,j)= 0.5*i+1     if i<=j 
                                            0.5*j+0.5      i>j,
        the matrix A is defined by: A(i,j)= B((i-1) mod n, j).

    This should give as output: 
        the matrix L given by: L(i,j)= 0   if i<j,
                                     = 1      i=j,
                                     = 0.5    i>j.
        the matrix U given by: U(i,j)= 1   if i<=j,
                                     = 0      i>j.
        the permutation pi given by:  pi(i)= (i+1) mod n.

    Output of L and U is in triples (i,j,L\U(i,j)):
        (i,j,0.5) for i>j
        (i,j,1)   for i<=j
    Output of pi is in pairs (i,pi(i))
        (i,(i+1) mod n) for all i.

    The matrix A is constructed such that the pivot choice is unique.
    In stage k of the LU decomposition, row k is swapped with row r=k+1.
    For the M by N cyclic distribution this forces a row swap
    between processor rows. 
*/

int M, N;

void bsplu_test(){
    int nloc(int p, int s, int n);
    void bsplu(int M, int N, int s, int t, int n, int *pi, double **a);
    int p, pid, q, s, t, n, nlr, nlc, i, j, iglob, jglob, *pi;
    double **a, time0, time1;
  
    bsp_begin(M*N);
    p=bsp_nprocs(); /* p=M*N */
    pid=bsp_pid();

    bsp_push_reg(&M,SZINT);
    bsp_push_reg(&N,SZINT);
    bsp_push_reg(&n,SZINT);
    bsp_sync();

    if (pid==0){
        printf("Please enter matrix size n:\n");
        scanf("%d",&n);
        for (q=0; q<p; q++){
            bsp_put(q,&M,&M,0,SZINT);
            bsp_put(q,&N,&N,0,SZINT);
            bsp_put(q,&n,&n,0,SZINT);
        }
    }
    bsp_sync();
    bsp_pop_reg(&n); /* not needed anymore */
    bsp_pop_reg(&N);
    bsp_pop_reg(&M);

    /* Compute 2D processor numbering from 1D numbering */
    s= pid%M;  /* 0 <= s < M */
    t= pid/M;  /* 0 <= t < N */

    /* Allocate and initialize matrix */
    nlr=  nloc(M,s,n); /* number of local rows */
    nlc=  nloc(N,t,n); /* number of local columns */
    a= matallocd(nlr,nlc);
    pi= vecalloci(nlr);
  
    if (s==0 && t==0){
        printf("LU decomposition of %d by %d matrix\n",n,n);
        printf("using the %d by %d cyclic distribution\n",M,N);
    }
    for (i=0; i<nlr; i++){
        iglob= i*M+s;         /* Global row index in A */
        iglob= (iglob-1+n)%n; /* Global row index in B */
        for (j=0; j<nlc; j++){
            jglob= j*N+t;     /* Global column index in A and B */
            a[i][j]= (iglob<=jglob ? 0.5*iglob+1 : 0.5*(jglob+1) );
        }
    }
  
    if (s==0 && t==0)
        printf("Start of LU decomposition\n");
    bsp_sync();
    time0=bsp_time();
 
    bsplu(M,N,s,t,n,pi,a);
    bsp_sync();
    time1=bsp_time();
 
    if (s==0 && t==0){
        printf("End of LU decomposition\n");
        printf("This took only %.6lf seconds.\n", time1-time0);
        printf("\nThe output permutation is:\n"); fflush(stdout);
    }

    if (t==0){
        for (i=0; i<nlr; i++){
            iglob=i*M+s;
            printf("i=%d, pi=%d, proc=(%d,%d)\n",iglob,pi[i],s,t);
        }
        fflush(stdout);
    }
    bsp_sync();

    if (s==0 && t==0){  
        printf("\nThe output matrix is:\n"); fflush(stdout);
    }   
    for (i=0; i<nlr; i++){
        iglob=i*M+s;
        for (j=0; j<nlc; j++){
            jglob=j*N+t;
            printf("i=%d, j=%d, a=%f, proc=(%d,%d)\n",
                   iglob,jglob,a[i][j],s,t);
        }
    }

    vecfreei(pi);
    matfreed(a);

    bsp_end();
}

int main(int argc, char **argv){
 
    bsp_init(bsplu_test, argc, argv);

    printf("Please enter number of processor rows M:\n");
    scanf("%d",&M);
    printf("Please enter number of processor columns N:\n");
    scanf("%d",&N);
    if (M*N > bsp_nprocs()){
        printf("Sorry, not enough processors available.\n"); 
        fflush(stdout);
        exit(1);
    }

    bsplu_test();
    exit(0);

} /* end main */

