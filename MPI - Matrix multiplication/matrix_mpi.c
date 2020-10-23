#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define R_A 2
#define C_A 2
#define R_B 2 
#define C_B 2
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

void printMatrix(double matrix[][2],int nr,int nc, char* title) {
    printf("\n");
    printf("%s:\n",title);
    for (int i=0; i<nr; i++) {
        printf("\n");
        for (int j=0; j<nc; j++) {
            printf("%6.2f\t", matrix[i][j]);
        }
    }
    printf("\n");
}


int main (int argc, char *argv[]) {

    int tasks, task_id, workers, source, dest, mtype, rows, averow, extra, offset, i, j, k, rc;
    double a[R_A][C_A], b[R_B][C_B], c[R_A][C_B];
    MPI_Status status;


    
    srand(time(NULL));

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&task_id);
    MPI_Comm_size(MPI_COMM_WORLD,&tasks);

  
    workers = tasks-1;

   
    if (task_id == MASTER) {
        printf("Enter the first matrix element=\n");    
        for (i=0; i<R_A; i++) {
            for (j=0; j<C_A; j++) {
                
                scanf("%lf",&a[i][j]);   
            }
        }
        printf("Enter the second matrix element=\n");    
        for (i=0; i<R_A; i++) {
            for (j=0; j<C_B; j++) {
                
                scanf("%lf",&b[i][j]);    
            }
        }

        printMatrix(a,R_A,C_A,"First Matrix ");
        printMatrix(b,R_B,C_B,"Second Matrix ");


        double start = MPI_Wtime();

       
        averow = R_A/workers;
        extra = R_A%workers;
        offset = 0; 
        mtype = FROM_MASTER;
        for (dest=1; dest<=workers; dest++) {
            rows = (dest <= extra) ? averow+1 : averow;
            

            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
 
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         
            MPI_Send(&a[offset][0], rows*C_A, MPI_DOUBLE, dest, mtype,
                     MPI_COMM_WORLD);
           
            MPI_Send(&b, R_A*C_B, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }
        mtype = FROM_WORKER;
        for (i=1; i<=workers; i++) {
            source = i;

            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows*C_B, MPI_DOUBLE, source, mtype,
                     MPI_COMM_WORLD, &status);
            
        }

        printMatrix(c,R_A,C_B,"Result matrix is");


        double finish = MPI_Wtime();
        printf("timetaken %f seconds.\n", finish - start);
    }



    if (task_id > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows*C_A, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, R_B*C_B, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k=0; k<C_B; k++) {
            for (i=0; i<rows; i++) {
                c[i][k] = 0.0;
                for (j=0; j<C_A; j++) {
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
                }
            }
        }
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows*C_B, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}



