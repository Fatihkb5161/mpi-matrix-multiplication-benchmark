import numpy as np

# Matris boyutunu (N) buradan ayarlayabilirsiniz. 
# Testler icin 4, 8, 16 islemciye tam bolunebilen bir sayi secin (orn: 512, 1024, 2048)
N = 1024 

def create_matrix_file(filename, N):
    # 1.0 ile 10.0 arasinda rastgele sayilardan olusan NxN matris uret
    mat = np.random.uniform(1.0, 10.0, (N, N))
    
    with open(filename, 'w') as f:
        f.write(f"{N}\n") # Ilk satira boyutu yaz [cite: 12]
        for row in mat:
            # Satirdaki elemanlari aralarinda bosluk birakarak yaz [cite: 12]
            f.write(" ".join([f"{val:.2f}" for val in row]) + "\n")

print(f"{N}x{N} boyutunda matrisler uretiliyor...")
create_matrix_file('a.txt', N)
create_matrix_file('b.txt', N)
print("a.txt ve b.txt basariyla olusturuldu! [cite: 11]")