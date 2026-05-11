from mpi4py import MPI
import numpy as np
import os

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

def read_matrix(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        N = int(lines[0].strip())
        data = []
        for line in lines[1:]:
            data.extend([float(x) for x in line.split()])
        return N, np.array(data, dtype=np.float64).reshape((N, N))

N, A, B, C = None, None, None, None

if rank == 0:
    N, A = read_matrix('a.txt')
    N2, B = read_matrix('b.txt')
    C = np.zeros((N, N), dtype=np.float64)

N = comm.bcast(N, root=0)

if rank != 0:
    B = np.zeros((N, N), dtype=np.float64)

comm.Bcast(B, root=0)

rows_per_proc = N // size
local_A = np.zeros((rows_per_proc, N), dtype=np.float64)
local_C = np.zeros((rows_per_proc, N), dtype=np.float64)

comm.Scatter(A, local_A, root=0)

# --- ZAMAN ÖLÇÜMÜ ---
comm.Barrier()
start_time = MPI.Wtime()

# ---------------------------------------------------------
# numpy.dot() YERİNE C KODUNDAKİ GİBİ STANDART FOR DÖNGÜSÜ
# ---------------------------------------------------------
for i in range(rows_per_proc):
    for j in range(N):
        local_C[i, j] = 0.0
        for k in range(N):
            local_C[i, j] += local_A[i, k] * B[k, j]
# ---------------------------------------------------------

end_time = MPI.Wtime()
local_duration = end_time - start_time
max_duration = comm.reduce(local_duration, op=MPI.MAX, root=0) # T_P

comm.Gather(local_C, C, root=0)

# --- S ve E HESAPLAMA ---
if rank == 0:
    T_1 = max_duration
    
    if size == 1:
        # 1 islemci ise T1'i dosyaya kaydet
        with open("t1_py.txt", "w") as f:
            f.write(str(max_duration))
    else:
        # Birden fazla islemci ise T1'i dosyadan oku
        if os.path.exists("t1_py.txt"):
            with open("t1_py.txt", "r") as f:
                T_1 = float(f.read())
        else:
            print("Uyari: t1_py.txt bulunamadi! S ve E yanlis hesaplanacak. Once -np 1 ile calistirin.")

    S = T_1 / max_duration
    E = S / size

    print(f"P: {size:<2} | T_P: {max_duration:<8.4f} sn | S (Speedup): {S:<6.4f} | E (Efficiency): {E:<6.4f}")