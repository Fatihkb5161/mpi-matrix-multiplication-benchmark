#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void read_matrix(const char* filename, double** mat, int* N) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Hata: %s dosyasi acilamadi!\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    if (fscanf(fp, "%d", N) != 1) {
        fprintf(stderr, "Hata: %s matris boyutu okunamadi!\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    *mat = (double*)malloc((*N) * (*N) * sizeof(double));
    for (int i = 0; i < (*N) * (*N); i++) {
        fscanf(fp, "%lf", &(*mat)[i]);
    }
    fclose(fp);
}

int main(int argc, char** argv) {
    int rank, size, N;
    double *A = NULL, *B = NULL, *C = NULL;
    double *local_A = NULL, *local_C = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        read_matrix("a.txt", &A, &N);
        int N2;
        read_matrix("b.txt", &B, &N2);
        if (N != N2) { MPI_Abort(MPI_COMM_WORLD, 1); }
        C = (double*)malloc(N * N * sizeof(double));
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) B = (double*)malloc(N * N * sizeof(double));
    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int rows_per_proc = N / size;
    local_A = (double*)malloc(rows_per_proc * N * sizeof(double));
    local_C = (double*)malloc(rows_per_proc * N * sizeof(double));

    MPI_Scatter(A, rows_per_proc * N, MPI_DOUBLE, local_A, rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // --- ZAMAN ÖLÇÜMÜ ---
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i * N + j] = 0.0;
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    double end_time = MPI_Wtime();
    double local_duration = end_time - start_time;
    double max_duration; // T_P
    
    MPI_Reduce(&local_duration, &max_duration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Gather(local_C, rows_per_proc * N, MPI_DOUBLE, C, rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // --- S ve E HESAPLAMA ---
    if (rank == 0) {
        double T_1 = max_duration;
        
        if (size == 1) {
            // 1 islemci ise T1'i dosyaya kaydet
            FILE* f = fopen("t1_c.txt", "w");
            if(f) { fprintf(f, "%lf", max_duration); fclose(f); }
        } else {
            // Birden fazla islemci ise T1'i dosyadan oku
            FILE* f = fopen("t1_c.txt", "r");
            if (f) {
                fscanf(f, "%lf", &T_1);
                fclose(f);
            } else {
                printf("Uyari: t1_c.txt bulunamadi! S ve E yanlis hesaplanacak. Once -np 1 ile calistirin.\n");
            }
        }

        double S = T_1 / max_duration;
        double E = S / size;

        printf("P: %-2d | T_P: %-8.4f sn | S (Speedup): %-6.4f | E (Efficiency): %-6.4f\n", size, max_duration, S, E);

        free(A); free(C);
    }

    free(B); free(local_A); free(local_C);
    MPI_Finalize();
    return 0;
}