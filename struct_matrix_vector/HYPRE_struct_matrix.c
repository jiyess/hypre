/*BHEADER**********************************************************************
 * (c) 1997   The Regents of the University of California
 *
 * See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
 * notice, contact person, and disclaimer.
 *
 * $Revision$
 *********************************************************************EHEADER*/
/******************************************************************************
 *
 * HYPRE_StructMatrix interface
 *
 *****************************************************************************/

#ifdef HYPRE_USE_PTHREADS
#define NO_PTHREAD_MANGLING
#endif

#include "headers.h"
#ifdef HYPRE_USE_PTHREADS
#include "threading.h"
#endif
/*--------------------------------------------------------------------------
 * HYPRE_NewStructMatrix
 *--------------------------------------------------------------------------*/

HYPRE_StructMatrix 
HYPRE_NewStructMatrix( MPI_Comm             comm,
                       HYPRE_StructGrid     grid,
                       HYPRE_StructStencil  stencil )
{
   return ( (HYPRE_StructMatrix)
            hypre_NewStructMatrix( comm,
                                   (hypre_StructGrid *) grid,
                                   (hypre_StructStencil *) stencil ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_FreeStructMatrix
 *--------------------------------------------------------------------------*/

int 
HYPRE_FreeStructMatrix( HYPRE_StructMatrix matrix )
{
   return( hypre_FreeStructMatrix( (hypre_StructMatrix *) matrix ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_InitializeStructMatrix
 *--------------------------------------------------------------------------*/

int
HYPRE_InitializeStructMatrix( HYPRE_StructMatrix matrix )
{
   return ( hypre_InitializeStructMatrix( (hypre_StructMatrix *) matrix ) );
}

#ifdef HYPRE_USE_PTHREADS
typedef struct {
   HYPRE_StructMatrix matrix;
   int *returnvalue;
} HYPRE_InitializeStructMatrixArgs;

void
HYPRE_InitializeStructMatrixVoidPtr( void *argptr)
{
   HYPRE_InitializeStructMatrixArgs *localargs =
                                (HYPRE_InitializeStructMatrixArgs *) argptr;

   *(localargs->returnvalue) = 
                 HYPRE_InitializeStructMatrix( localargs->matrix );
}

int
HYPRE_InitializeStructMatrixPush( HYPRE_StructMatrix matrix )
{
   HYPRE_InitializeStructMatrixArgs  pushargs;
   int                               i;
   int                               returnvalue;

   pushargs.matrix = matrix;
   pushargs.returnvalue = (int *) malloc(sizeof(int));
   for (i=0; i<NUM_THREADS; i++)
      hypre_work_put( HYPRE_InitializeStructMatrixVoidPtr, (void *)&pushargs);

   hypre_work_wait();

   returnvalue = *(pushargs.returnvalue);

   free( pushargs.returnvalue );

   return returnvalue;
}
#endif


/*--------------------------------------------------------------------------
 * HYPRE_SetStructMatrixValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_SetStructMatrixValues( HYPRE_StructMatrix  matrix,
                             int                *grid_index,
                             int                 num_stencil_indices,
                             int                *stencil_indices,
                             double             *values              )
{
   hypre_StructMatrix *new_matrix = (hypre_StructMatrix *) matrix;
   hypre_Index         new_grid_index;

   int                 d;
   int                 ierr = 0;

   hypre_ClearIndex(new_grid_index);
   for (d = 0;
        d < hypre_StructGridDim(hypre_StructMatrixGrid(new_matrix));
        d++)
   {
      hypre_IndexD(new_grid_index, d) = grid_index[d];
   }

   ierr = hypre_SetStructMatrixValues( new_matrix,
                                       new_grid_index,
                                       num_stencil_indices, stencil_indices,
                                       values );

   return (ierr);
}

/*--------------------------------------------------------------------------
 * HYPRE_SetStructMatrixBoxValues
 *--------------------------------------------------------------------------*/

int 
HYPRE_SetStructMatrixBoxValues( HYPRE_StructMatrix  matrix,
                                int                *ilower,
                                int                *iupper,
                                int                 num_stencil_indices,
                                int                *stencil_indices,
                                double             *values              )
{
   hypre_StructMatrix *new_matrix = (hypre_StructMatrix *) matrix;
   hypre_Index         new_ilower;
   hypre_Index         new_iupper;
   hypre_Box          *new_value_box;
                    
   int                 d;
   int                 ierr = 0;

   hypre_ClearIndex(new_ilower);
   hypre_ClearIndex(new_iupper);
   for (d = 0;
        d < hypre_StructGridDim(hypre_StructMatrixGrid(new_matrix));
        d++)
   {
      hypre_IndexD(new_ilower, d) = ilower[d];
      hypre_IndexD(new_iupper, d) = iupper[d];
   }
   new_value_box = hypre_NewBox(new_ilower, new_iupper);

   ierr = hypre_SetStructMatrixBoxValues( new_matrix,
                                          new_value_box,
                                          num_stencil_indices, stencil_indices,
                                          values );

   hypre_FreeBox(new_value_box);

   return (ierr);
}

#ifdef HYPRE_USE_PTHREADS
typedef struct {
   HYPRE_StructMatrix  matrix;
   int                *ilower;
   int                *iupper;
   int                 num_stencil_indices;
   int                *stencil_indices;
   double             *values;
   int                *returnvalue;
} HYPRE_SetStructMatrixBoxValuesArgs;

void
HYPRE_SetStructMatrixBoxValuesVoidPtr( void *argptr)
{
   HYPRE_SetStructMatrixBoxValuesArgs *localargs =
                                (HYPRE_SetStructMatrixBoxValuesArgs *) argptr;

   *(localargs->returnvalue) = 
                 HYPRE_SetStructMatrixBoxValues( localargs->matrix,
                                                 localargs->ilower,
                                                 localargs->iupper,
                                                 localargs->num_stencil_indices,
                                                 localargs->stencil_indices,
                                                 localargs->values);
}

int
HYPRE_SetStructMatrixBoxValuesPush( HYPRE_StructMatrix matrix,
                                    int               *ilower,
                                    int               *iupper,
                                    int                num_stencil_indices,
                                    int               *stencil_indices,
                                    double            *values)
{
   HYPRE_SetStructMatrixBoxValuesArgs  pushargs;
   int                                 i;
   int                                 returnvalue;

   pushargs.matrix              = matrix;
   pushargs.ilower              = ilower;
   pushargs.iupper              = iupper;
   pushargs.num_stencil_indices = num_stencil_indices;
   pushargs.stencil_indices     = stencil_indices;
   pushargs.returnvalue = (int *) malloc(sizeof(int));
   for (i=0; i<NUM_THREADS; i++)
      hypre_work_put( HYPRE_SetStructMatrixBoxValuesVoidPtr, (void *)&pushargs);

   hypre_work_wait();

   returnvalue = *(pushargs.returnvalue);

   free( pushargs.returnvalue );

   return returnvalue;
}
#endif


/*--------------------------------------------------------------------------
 * HYPRE_AssembleStructMatrix
 *--------------------------------------------------------------------------*/

int 
HYPRE_AssembleStructMatrix( HYPRE_StructMatrix matrix )
{
   return( hypre_AssembleStructMatrix( (hypre_StructMatrix *) matrix ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SetStructMatrixNumGhost
 *--------------------------------------------------------------------------*/
 
void
HYPRE_SetStructMatrixNumGhost( HYPRE_StructMatrix  matrix,
                               int                *num_ghost )
{
   hypre_SetStructMatrixNumGhost( (hypre_StructMatrix *) matrix, num_ghost);
}

/*--------------------------------------------------------------------------
 * HYPRE_StructMatrixGrid
 *--------------------------------------------------------------------------*/

HYPRE_StructGrid
HYPRE_StructMatrixGrid( HYPRE_StructMatrix matrix )
{
   return ( (HYPRE_StructGrid)
            (hypre_StructMatrixGrid( (hypre_StructMatrix *) matrix) ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SetStructMatrixSymmetric
 *--------------------------------------------------------------------------*/
 
void
HYPRE_SetStructMatrixSymmetric( HYPRE_StructMatrix  matrix,
                                int                 symmetric )
{
   hypre_StructMatrixSymmetric( (hypre_StructMatrix *) matrix ) = symmetric;
}

/*--------------------------------------------------------------------------
 * HYPRE_PrintStructMatrix
 *--------------------------------------------------------------------------*/

void 
HYPRE_PrintStructMatrix( char               *filename,
                         HYPRE_StructMatrix  matrix,
                         int                 all )
{
   hypre_PrintStructMatrix( filename, (hypre_StructMatrix *) matrix, all );
}

#ifdef HYPRE_USE_PTHREADS
typedef struct {
   char *filename;
   HYPRE_StructMatrix matrix;
   int all;
} HYPRE_PrintStructMatrixArgs;

void
HYPRE_PrintStructMatrixVoidPtr( void *argptr )
{
   HYPRE_PrintStructMatrixArgs *localargs = 
                                    (HYPRE_PrintStructMatrixArgs *) argptr;

   HYPRE_PrintStructMatrix( localargs->filename, 
                            localargs->matrix,
                            localargs->all );
}

void
HYPRE_PrintStructMatrixPush( char               *filename,
                             HYPRE_StructMatrix  matrix,
                             int                 all )
{
   HYPRE_PrintStructMatrixArgs  pushargs;
   int                          i;

   pushargs.filename = filename;
   pushargs.matrix   = matrix;
   pushargs.all      = all;

   for (i = 0; i < NUM_THREADS; i++)
      hypre_work_put( HYPRE_PrintStructMatrixVoidPtr, (void *)&pushargs );

   hypre_work_wait();
}
#endif
