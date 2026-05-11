# Paralel Matris Çarpımı ve Performans Analizi (MPI)

Bu proje, paralel bilgisayar mimarileri üzerinde C (OpenMPI) ve Python (mpi4py) dillerini kullanarak matris-matris çarpımı işleminin performans analizini gerçekleştirmek amacıyla geliştirilmiştir. Proje kapsamında paralel programlama metrikleri olan **Hızlanma (Speedup)** ve **Verimlilik (Efficiency)** hesaplanmış, ayrıca donanım sınırlarının aşılmasının (oversubscription) performansa etkileri incelenmiştir.

## 🚀 Proje Amacı ve Kapsamı
- Hem yüksek seviyeli (Python) hem de düşük seviyeli (C) dillerde paralel programlama performansını ölçmek.
- Derlenen (Compiled) ve Yorumlanan (Interpreted) dillerin algoritmik (saf O(N^3) döngü) kıyaslamasını yapmak.
- Veri dağıtımı (Scatter) ve toplanması (Gather) gibi temel MPI iletişim fonksiyonlarını kullanmak.
- Fiziksel çekirdek sayısından fazla süreç (process) oluşturmanın bağlam geçişi (Context Switching) maliyetlerini ve performans çöküşünü analiz etmek.

## 🛠️ Kullanılan Teknolojiler
- **C:** OpenMPI (mpi.h)
- **Python:** Python 3.x, mpi4py, NumPy (Sadece veri yapısı olarak), Matplotlib (Grafikleme)
- **İşletim Sistemi:** Linux (Ubuntu / WSL)

## 📂 Dosya Yapısı
- `main.c`: C dili ile yazılmış paralel matris çarpım kodu.
- `main.py`: Python (mpi4py) ile yazılmış, saf döngü mimarisine sahip paralel matris çarpım kodu.
- `matris_uret.py`: İstenilen boyutta (N) rastgele A ve B test matrislerini (txt) üreten betik.
- `otomasyon.py`: Tüm 1, 2, 4, 8 ve 16 işlemcili testleri otomatik koşturan ve çıktıları ayrıştırıp hızlanma grafiğini (PNG) çizen benchmarking betiği.

## ⚙️ Nasıl Çalıştırılır?

**1. Matrisleri Üretme:**
```bash
python3 matris_uret.py
