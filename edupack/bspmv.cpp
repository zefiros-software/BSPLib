#include "bspedupack.h"

void bspmv(int p, int s, int n, int nz, int nrows, int ncols,
           double *a, int *inc,
           int *srcprocv, int *srcindv, int *destprocu, int *destindu,
           int nv, int nu, double *v, double *u){

    /* This function multiplies a sparse matrix A with a
       dense vector v, giving a dense vector u=Av.
       A is n by n, and u,v are vectors of length n. 
       A, u, and v are distributed arbitrarily on input.
       They are all accessed using local indices, but the local
       matrix indices may differ from the local vector indices.
       The local matrix nonzeros are stored in an incremental
       compressed row storage (ICRS) data structure defined by
       nz, nrows, ncols, a, inc.
       All rows and columns in the local data structure are nonempty.
      
       p is the number of processors.
       s is the processor number, 0 <= s < p.
       n is the global size of the matrix A.
       nz is the number of local nonzeros.
       nrows is the number of local rows.
       ncols is the number of local columns.

       a[k] is the numerical value of the k'th local nonzero of the
            sparse matrix A, 0 <= k < nz.
       inc[k] is the increment in the local column index of the
              k'th local nonzero, compared to the column index of the
              (k-1)th nonzero, if this nonzero is in the same row;
              otherwise, ncols is added to the difference.
              By convention, the column index of the -1'th nonzero is 0.

       srcprocv[j] is the source processor of the component in v
              corresponding to the local column j, 0 <= j < ncols.
       srcindv[j] is the local index on the source processor
              of the component in v corresponding to the local column j.
       destprocu[i] is the destination processor of the partial sum
                  corresponding to the local row i, 0 <= i < nrows.
       destindu[i] is the local index in the vector u on the destination
                   processor corresponding to the local row i.
    
       nv is the number of local components of the input vector v.
       nu is the number of local components of the output vector u.
       v[k] is the k'th local component of v, 0 <= k < nv.
       u[k] is the k'th local component of u, 0 <= k < nu.
    */

    int i, j, k, tagsz, status, nsums, nbytes, *pinc;
    double sum, *psum, *pa, *vloc, *pvloc, *pvloc_end;

    /****** Superstep 0. Initialize and register ******/
    for(i=0; i<nu; i++)
        u[i]= 0.0;
    vloc= vecallocd(ncols);
    bsp_push_reg(v,nv*SZDBL);
    tagsz= SZINT;
    bsp_set_tagsize(&tagsz);
    bsp_sync();

    /****** Superstep 1. Fanout ******/
    for(j=0; j<ncols; j++)
        bsp_get(srcprocv[j],v,srcindv[j]*SZDBL,&vloc[j],SZDBL); 
    bsp_sync();

    /****** Superstep 2. Local matrix-vector multiplication and fanin */
    psum= &sum;
    pa= a;
    pinc= inc;
    pvloc= vloc;
    pvloc_end= pvloc + ncols;

    pvloc += *pinc;
    for(i=0; i<nrows; i++){
        *psum= 0.0;
        while (pvloc<pvloc_end){
            *psum += (*pa) * (*pvloc);
            pa++; 
            pinc++;
            pvloc += *pinc;
        }
        bsp_send(destprocu[i],&destindu[i],psum,SZDBL); 
        pvloc -= ncols;
    }
    bsp_sync();

    /****** Superstep 3. Summation of nonzero partial sums ******/
    bsp_qsize(&nsums,&nbytes);
    bsp_get_tag(&status,&i);
    for(k=0; k<nsums; k++){
        /* status != -1, but its value is not used */
        bsp_move(&sum,SZDBL); 
        u[i] += sum;
        bsp_get_tag(&status,&i);
    }

    bsp_pop_reg(v);
    vecfreed(vloc);

} /* end bspmv */

int nloc(int p, int s, int n){
    /* Compute number of local components of processor s for vector
       of length n distributed cyclically over p processors. */

    return  (n+p-s-1)/p ;

} /* end nloc */

void bspmv_init(int p, int s, int n, int nrows, int ncols,
                int nv, int nu, int *rowindex, int *colindex,
                int *vindex, int *uindex, int *srcprocv, int *srcindv,
                int *destprocu, int *destindu){

    /* This function initializes the communication data structure
       needed for multiplying a sparse matrix A with a dense vector v,
       giving a dense vector u=Av.

       Input: the arrays rowindex, colindex, vindex, uindex,
       containing the global indices corresponding to the local indices
       of the matrix and the vectors.
       Output: initialized arrays srcprocv, srcindv, destprocu, destindu
       containing the processor number and the local index on the
       remote processor of vector components corresponding to
       local matrix columns and rows. 
      
       p, s, n, nrows, ncols, nv, nu are the same as in bspmv.

       rowindex[i] is the global index of the local row i, 0 <= i < nrows.
       colindex[j] is the global index of the local column j, 0 <= j < ncols.
       vindex[j] is the global index of the local v-component j, 0 <= j < nv.
       uindex[i] is the global index of the local u-component i, 0 <= i < nu.

       srcprocv, srcindv, destprocu, destindu are the same as in bspmv.
    */

    int nloc(int p, int s, int n);
    int np, i, j, iglob, jglob, *tmpprocv, *tmpindv, *tmpprocu, *tmpindu;

    /****** Superstep 0. Allocate and register temporary arrays */
    np= nloc(p,s,n);
    tmpprocv=vecalloci(np); bsp_push_reg(tmpprocv,np*SZINT);
    tmpindv=vecalloci(np);  bsp_push_reg(tmpindv,np*SZINT);
    tmpprocu=vecalloci(np); bsp_push_reg(tmpprocu,np*SZINT);
    tmpindu=vecalloci(np);  bsp_push_reg(tmpindu,np*SZINT);
    bsp_sync();

    /****** Superstep 1. Write into temporary arrays ******/
    for(j=0; j<nv; j++){
        jglob= vindex[j];
        /* Use the cyclic distribution */
        bsp_put(jglob%p,&s,tmpprocv,(jglob/p)*SZINT,SZINT);
        bsp_put(jglob%p,&j,tmpindv, (jglob/p)*SZINT,SZINT);
    }
    for(i=0; i<nu; i++){
        iglob= uindex[i];
        bsp_put(iglob%p,&s,tmpprocu,(iglob/p)*SZINT,SZINT);
        bsp_put(iglob%p,&i,tmpindu, (iglob/p)*SZINT,SZINT);
    }
    bsp_sync();

    /****** Superstep 2. Read from temporary arrays ******/
    for(j=0; j<ncols; j++){
        jglob= colindex[j];
        bsp_get(jglob%p,tmpprocv,(jglob/p)*SZINT,&srcprocv[j],SZINT);
        bsp_get(jglob%p,tmpindv, (jglob/p)*SZINT,&srcindv[j], SZINT);
    }
    for(i=0; i<nrows; i++){
        iglob= rowindex[i];
        bsp_get(iglob%p,tmpprocu,(iglob/p)*SZINT,&destprocu[i],SZINT);
        bsp_get(iglob%p,tmpindu, (iglob/p)*SZINT,&destindu[i], SZINT);
    }
    bsp_sync();

    /****** Superstep 3. Deregister temporary arrays ******/
    bsp_pop_reg(tmpindu); bsp_pop_reg(tmpprocu);
    bsp_pop_reg(tmpindv); bsp_pop_reg(tmpprocv);
    bsp_sync();

    /****** Superstep 4. Free temporary arrays ******/
    vecfreei(tmpindu); vecfreei(tmpprocu);          
    vecfreei(tmpindv); vecfreei(tmpprocv);   

} /* end bspmv_init */
