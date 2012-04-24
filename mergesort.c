#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

#define DEBUG_MERGE             0
#define DEBUG_MERGESORT         0

typedef struct mergesort mergesort_t;
struct mergesort {
    void *data;
    void *aux;
    cmpfunc_t cmpfunc;
    cpyfunc_t cpyfunc;
    int datasize;
};


static int merge(mergesort_t *m, int left, int middle, int right)
{
    int i, j, k;

    if (DEBUG_MERGE)
        printf("MERGE: TODO left=%d (%d elements), right=%d (%d elements)\n",
               left, (middle-left)+1, right, (right-middle));
    /* Compare elements in left and right subarray/sublist and move
       into auxillary array */
    for (i = left, j = middle+1, k = 0; k < (right-left)+1; k++) {
        if (i > middle) {
            if (DEBUG_MERGE)
                printf("MERGE: Copy from RIGHT %d to AUX %d (LEFT EMPTY)\n", 
                       j, k);
            m->cpyfunc(m->data, j, m->aux, k);
            j++;
        } else if (j > right) {
            if (DEBUG_MERGE)
                printf("MERGE: Copy from LEFT %d to AUX %d (RIGHT EMPTY)\n", 
                       i, k);
            m->cpyfunc(m->data, i, m->aux, k);
            i++;
        } else {
            if (m->cmpfunc(m->data, i, j) > 0) {
                if (DEBUG_MERGE)
                    printf("MERGE: Copy from LEFT %d to AUX %d\n", 
                           i, k);
                m->cpyfunc(m->data, i, m->aux, k);
                i++;
            } else {
                if (DEBUG_MERGE)
                    printf("MERGE: Copy from RIGHT %d to AUX %d\n", 
                           j, k);
                m->cpyfunc(m->data, j, m->aux, k);
                j++;
            }
        }
    }
    if (DEBUG_MERGE)
        printf("MERGE: DONE\n");
    /* Copy from auxillary array back into original array/list */
    for (i = 0; i < (right-left)+1; i++)
        m->cpyfunc(m->aux, i, m->data, i+left);
}


static void _mergesort(mergesort_t *m, int left, int right)
{
    int middle;

    middle = (right+left)/2;    /* Note: vulnerable to overflow for large ints */

    if (right <= left)
        return;

    if (DEBUG_MERGESORT)
        printf("RECURSE: left=%d, middle=%d, right=%d\n",
               left, middle, right);

    _mergesort(m, left, middle);
    _mergesort(m, middle+1, right);

    merge(m, left, middle, right);
}


int sort(void *data, 
         int left, int right,
         cmpfunc_t cmpfunc,
         cpyfunc_t cpyfunc,
         int datasize)
{       
    mergesort_t *m;

    m = (mergesort_t*)calloc(1, sizeof(mergesort_t));
    if (m == NULL)
        goto error;
    m->aux = malloc(datasize*((right-left)+1));
    if (m->aux == NULL)
        goto error;
    m->data = data;
    m->cmpfunc = cmpfunc;
    m->cpyfunc = cpyfunc;
    m->datasize = datasize;

    _mergesort(m, left, right);

    free(m->aux);
    free(m);

    return 0;
 error:
    if (m != NULL) {
        if (m->aux != NULL)
            free(m->aux);
        free(m);
    }
    return -1;
}
