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
 * HYPRE_ParCSRMatrix interface
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * HYPRE_CreateParCSRMatrix
 *--------------------------------------------------------------------------*/

HYPRE_ParCSRMatrix 
HYPRE_CreateParCSRMatrix( MPI_Comm  comm,
                          int       global_num_rows,
                          int       global_num_cols,
                          int      *row_starts,
                          int      *col_starts,
                          int       num_cols_offd,
                          int       num_nonzeros_diag,
                          int       num_nonzeros_offd )
{
   hypre_ParCSRMatrix *matrix;

   matrix = hypre_CreateParCSRMatrix(comm, global_num_rows, global_num_cols,
                                     row_starts, col_starts, num_cols_offd,
                                     num_nonzeros_diag, num_nonzeros_offd);

   return ( (HYPRE_ParCSRMatrix) matrix );
}

/*--------------------------------------------------------------------------
 * HYPRE_DestroyParCSRMatrix
 *--------------------------------------------------------------------------*/

int 
HYPRE_DestroyParCSRMatrix( HYPRE_ParCSRMatrix matrix )
{
   return( hypre_DestroyParCSRMatrix( (hypre_ParCSRMatrix *) matrix ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_InitializeParCSRMatrix
 *--------------------------------------------------------------------------*/

int
HYPRE_InitializeParCSRMatrix( HYPRE_ParCSRMatrix matrix )
{
   return ( hypre_InitializeParCSRMatrix( (hypre_ParCSRMatrix *) matrix ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_ReadParCSRMatrix
 *--------------------------------------------------------------------------*/

HYPRE_ParCSRMatrix 
HYPRE_ReadParCSRMatrix( MPI_Comm comm,
                        char    *file_name )
{
   return ( (HYPRE_ParCSRMatrix) hypre_ReadParCSRMatrix( comm, file_name ));
}

/*--------------------------------------------------------------------------
 * HYPRE_PrintParCSRMatrix
 *--------------------------------------------------------------------------*/

void 
HYPRE_PrintParCSRMatrix( HYPRE_ParCSRMatrix  matrix,
                         char               *file_name )
{
   hypre_PrintParCSRMatrix( (hypre_ParCSRMatrix *) matrix,
                            file_name );
}

/*--------------------------------------------------------------------------
 * HYPRE_GetCommParCSR
 *--------------------------------------------------------------------------*/

int
HYPRE_GetCommParCSR( HYPRE_ParCSRMatrix  matrix,
                         MPI_Comm *comm )
{  
   int ierr = 0;

   *comm = hypre_ParCSRMatrixComm((hypre_ParCSRMatrix *) matrix);

   return( ierr );
}
/*--------------------------------------------------------------------------
 * HYPRE_GetDimsParCSR
 *--------------------------------------------------------------------------*/

int
HYPRE_GetDimsParCSR( HYPRE_ParCSRMatrix  matrix,
                         int *M, int *N )
{  
   int ierr = 0;

   *M = hypre_ParCSRMatrixGlobalNumRows((hypre_ParCSRMatrix *) matrix);
   *N = hypre_ParCSRMatrixGlobalNumRows((hypre_ParCSRMatrix *) matrix);

   return( ierr );
}

/*--------------------------------------------------------------------------
 * HYPRE_GetRowPartitioningParCSR
 *--------------------------------------------------------------------------*/

int
HYPRE_GetRowPartitioningParCSR( HYPRE_ParCSRMatrix  matrix,
                                int **row_partitioning_ptr)
{  
   int ierr = 0;
   int *row_partitioning, *row_starts;
   int num_procs, i;

   MPI_Comm_size(hypre_ParCSRMatrixComm((hypre_ParCSRMatrix *) matrix), 
			&num_procs);
   row_starts = hypre_ParCSRMatrixRowStarts((hypre_ParCSRMatrix *) matrix);
   if (!row_starts) return -1;
   row_partitioning = hypre_CTAlloc(int, num_procs+1);
   for (i=0; i < num_procs + 1; i++)
	row_partitioning[i] = row_starts[i];

   *row_partitioning_ptr = row_partitioning;
   return( ierr );
}

/*--------------------------------------------------------------------------
 * HYPRE_GetColPartitioningParCSR
 *--------------------------------------------------------------------------*/

int
HYPRE_GetColPartitioningParCSR( HYPRE_ParCSRMatrix  matrix,
                                int **col_partitioning_ptr)
{  
   int ierr = 0;
   int *col_partitioning, *col_starts;
   int num_procs, i;

   MPI_Comm_size(hypre_ParCSRMatrixComm((hypre_ParCSRMatrix *) matrix), 
			&num_procs);
   col_starts = hypre_ParCSRMatrixColStarts((hypre_ParCSRMatrix *) matrix);
   if (!col_starts) return -1;
   col_partitioning = hypre_CTAlloc(int, num_procs+1);
   for (i=0; i < num_procs + 1; i++)
	col_partitioning[i] = col_starts[i];

   *col_partitioning_ptr = col_partitioning;
   return( ierr );
}



/*--------------------------------------------------------------------------
 * HYPRE_GetLocalRangeParcsr
 *--------------------------------------------------------------------------*/

int
HYPRE_GetLocalRangeParcsr( HYPRE_ParCSRMatrix  matrix,
                         int               *row_start,
                         int               *row_end,
                         int               *col_start,
                         int               *col_end )
{  
   int ierr = 0;

   ierr = hypre_GetLocalRangeParCSRMatrix( (hypre_ParCSRMatrix *) matrix,
                            row_start, row_end, col_start, col_end );
   return( ierr );
}

/*--------------------------------------------------------------------------
 * HYPRE_GetRowParCSRMatrix
 *--------------------------------------------------------------------------*/

int 
HYPRE_GetRowParCSRMatrix( HYPRE_ParCSRMatrix  matrix,
                         int                row,
                         int               *size,
                         int               **col_ind,
                         double            **values )
{  
   int ierr=0;
   
   ierr = hypre_GetRowParCSRMatrix( (hypre_ParCSRMatrix *) matrix,
                            row, size, col_ind, values );
   return( ierr );
}

/*--------------------------------------------------------------------------
 * HYPRE_RestoreRowParCSRMatrix
 *--------------------------------------------------------------------------*/

int 
HYPRE_RestoreRowParCSRMatrix( HYPRE_ParCSRMatrix  matrix,
                         int                row,
                         int               *size,
                         int               **col_ind,
                         double            **values )
{  
   int ierr = 0;

   ierr = hypre_RestoreRowParCSRMatrix( (hypre_ParCSRMatrix *) matrix,
                            row, size, col_ind, values );
   return( ierr );
}

/*--------------------------------------------------------------------------
 * HYPRE_CSRMatrixToParCSRMatrix
 *--------------------------------------------------------------------------*/

HYPRE_ParCSRMatrix 
HYPRE_CSRMatrixToParCSRMatrix( MPI_Comm comm,
			       HYPRE_CSRMatrix A_CSR,
			       int *row_partitioning,
                               int *col_partitioning)
{
   return ( (HYPRE_ParCSRMatrix) hypre_CSRMatrixToParCSRMatrix( comm, 	
		(hypre_CSRMatrix *) A_CSR, row_partitioning, 
		col_partitioning) );
}

/*--------------------------------------------------------------------------
 * HYPRE_ParMatvec
 *--------------------------------------------------------------------------*/

int
HYPRE_ParMatvec( double alpha, HYPRE_ParCSRMatrix A, HYPRE_ParVector x,
		 double beta, HYPRE_ParVector y)
{
   return ( hypre_ParMatvec( alpha, (hypre_ParCSRMatrix *) A,
		(hypre_ParVector *) x, beta, (hypre_ParVector *) y) );
}
