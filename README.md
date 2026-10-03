# Dokumentasi Program Analisis Statistika

Dokumentasi ini berisi diagram alir (flowchart) serta penjelasan logis untuk program utama dan fungsi-fungsi pendukungnya.

---

## 1. Flowchart Utama (Program Utama / `main`)

Flowchart ini menggambarkan alur kerja program utama mulai dari input data, pengurutan, kalkulasi statistik, hingga penyajian grafik/kurva.

```mermaid
flowchart TD
    A([Mulai]) --> B[/"Cetak judul program"/]
    B --> C[/"Input n (jumlah data)"/]
    C --> D{"n kurang dari atau sama dengan 0 ?"}
    D -- Ya --> E[/"Cetak pesan error"/]
    E --> C
    D -- Tidak --> F["i := 0"]
    F --> G{"i lebih kecil dari n ?"}
    G -- Ya --> H[/"Cetak Data ke-(i+1) dan Input data[i]"/]
    H --> I["i := i + 1"]
    I --> G
    G -- Tidak --> J[["sort(data, data + n)"]]
    J --> K[["mean := hitungMean(data, n)"]]
    K --> L[["median := hitungMedian(data, n)"]]
    L --> M[["modus := hitungModus(data, n)"]]
    M --> N["minimum := data[0]<br>maksimum := data[n-1]<br>range := maksimum - minimum"]
    N --> O[["q1 := hitungQ1(data, n)<br>q3 := hitungQ3(data, n)"]]
    O --> P["iqr := q3 - q1<br>lowerFence := q1 - 1.5 * iqr<br>upperFence := q3 + 1.5 * iqr"]
    P --> Q[["varians := hitungVarians(data, n, mean)"]]
    Q --> R[["standarDeviasi := hitungStandarDeviasi(varians)"]]
    R --> S[["skewness := hitungSkewness(data, n, mean, standarDeviasi)"]]
    S --> T[["tampilkanData(data, n)"]]
    T --> U[/"Cetak judul HASIL ANALISIS<br>Cetak Mean dan Median"/]
    U --> V{"isnan(modus) ?"}
    V -- Ya --> W[/"Tidak ada modus"/]
    V -- Tidak --> X[/"Cetak modus"/]
    W --> Y[/"Cetak Q1, Q3, IQR<br>Cetak Lower Fence, Upper Fence<br>Cetak info batas Z-Score"/]
    X --> Y
    Y --> Z[["analisisOutlier(data, n, ...)"]]
    Z --> AA[/"Cetak Maximum dan Minimum<br>Cetak Range<br>Cetak Varians, Standar Deviasi"/]
    AA --> AB[["analisisEkor(skewness)"]]
    AB --> AC[["tampilkanKurva(data, n)"]]
    AC --> AD([Selesai])
```

### Penjelasan Alur (Program Utama)
1. **Inisialisasi & Validasi Input**: Program meminta masukan `n` (jumlah data). Jika $n \le 0$, muncul pesan error dan pengguna diminta memasukkan ulang data.
2. **Penginputan Array**: Menggunakan perulangan (*loop*) `i` dari `0` sampai `n-1` untuk membaca setiap nilai elemen `data[i]`.
3. **Pengurutan Data**: Memanggil fungsi bawaan `sort(data, data + n)` untuk menyusun data secara terurut (*ascending*).
4. **Kalkulasi Nilai Statistika**:
   * Memanggil fungsi pemroses parameter deskriptif: `mean`, `median`, dan `modus`.
   * Menentukan nilai `minimum` (elemen `data[0]`), `maksimum` (elemen `data[n-1]`), serta menghitung `range`.
   * Memanggil pemroses kuartil (`q1` dan `q3`), lalu menghitung `iqr`, `lowerFence`, dan `upperFence`.
   * Menhitung nilai sebaran (`varians`, `standarDeviasi`, dan `skewness`).
5. **Output Hasil & Pemanggilan Modul**:
   * Menampilkan array data terurut.
   * Mengecek ketersediaan modus (`isnan(modus)`). Jika tidak ada, mencetak informasi "Tidak ada modus".
   * Memanggil fungsi modul `analisisOutlier`, `analisisEkor`, dan `tampilkanKurva` untuk visualisasi.

---

## 2. Flowchart 2: Fungsi `analisisOutlier`

Flowchart ini menggambarkan proses deteksi pencilan (outlier) menggunakan dua metode sekaligus: metode jangkauan antar-kuartil (*Fence/IQR*) dan metode *Z-Score*.

```mermaid
flowchart TD
    S([analisisOutlier]) --> A[/"Cetak judul ANALISIS OUTLIER"/]
    A --> B["jumlahOutlier := 0 ; i := 0"]
    B --> L{"i lebih kecil dari n ?"}
    L -- Tidak --> C1((B1))
    L -- Ya --> Z[["z := hitungZScore(data[i], mean, standarDeviasi)"]]
    Z --> F["outlierFence := data[i] di bawah lowerFence atau di atas upperFence<br>outlierZScore := abs(z) lebih besar dari 3.0"]
    F --> O{"outlierFence atau outlierZScore ?"}
    O -- Tidak --> INC["i := i + 1"]
    O -- Ya --> P[/"Cetak Data dan Z-Score"/]
    P --> D1{"data[i] lebih kecil dari lowerFence ?"}
    D1 -- Ya --> R1[/"Cetak Outlier bawah"/]
    D1 -- Tidak --> D2{"data[i] lebih besar dari upperFence ?"}
    D2 -- Ya --> R2[/"Cetak Outlier atas"/]
    D2 -- Tidak --> D3{"abs(z) lebih besar dari 3.0 ?"}
    D3 -- Ya --> R3[/"Cetak Outlier berdasarkan Z-Score"/]
    D3 -- Tidak --> CNT
    R1 --> CNT["jumlahOutlier := jumlahOutlier + 1"]
    R2 --> CNT
    R3 --> CNT
    CNT --> INC
    INC --> L
    C2((B2)) --> Q{"jumlahOutlier = 0 ?"}
    Q -- Ya --> T1[/"Cetak Tidak ditemukan outlier"/]
    Q -- Tidak --> T2[/"Cetak Jumlah outlier"/]
    T1 --> E([Selesai])
    T2 --> E
```

### Penjelasan Alur (`analisisOutlier`)
1. **Inisialisasi**: Menetapkan penghitung pencilan `jumlahOutlier = 0` dan indeks perulangan `i = 0`.
2. **Pemeriksaan Per-Elemen**:
   * Untuk tiap `data[i]`, dihitung nilai skor standar `z` melalui `hitungZScore()`.
   * Mengecek dua batas: apakah data melampaui pagar/fence (`< lowerFence` atau `> upperFence`) DAN apakah $\vert{}z\vert{} > 3.0$.
3. **Pengkategorian Outlier**:
   * Jika memenuhi salah satu syarat pencilan, cetak nilai data beserta skor Z-nya.
   * Tentukan label spesifiknya: **Outlier Bawah**, **Outlier Atas**, atau **Outlier berdasarkan Z-Score**.
   * Tambahkan penghitung `jumlahOutlier` sebanyak 1.
4. **Keluaran Akhir**:
   * Setelah seluruh elemen diperiksa (`i >= n`), cek jika `jumlahOutlier == 0`.
   * Jika tidak ada pencilan, tampilkan teks *"Tidak ditemukan outlier"*. Jika ada, cetak total nilai `jumlahOutlier`.

---

## 3. Flowchart 3: Fungsi `hitungModus`

Flowchart ini memperlihatkan algoritma pencarian nilai yang paling sering muncul (modus) di dalam array.

```mermaid
flowchart TD
    S([hitungModus]) --> A["modus := data[0]<br>frekuensiMaks := 1"]
    A --> B["i := 0"]
    B --> D1{"i lebih kecil dari n ?"}
    D1 -- Tidak --> C1((C1))
    D1 -- Ya --> P1["frekuensi := 0 ; j := 0"]
    P1 --> D2{"j lebih kecil dari n ?"}
    D2 -- Ya --> D3{"data[i] sama dengan data[j] ?"}
    D3 -- Ya --> P2["frekuensi := frekuensi + 1"]
    D3 -- Tidak --> P3
    P2 --> P3["j := j + 1"]
    P3 --> D2
    D2 -- Tidak --> F{"frekuensi lebih besar dari frekuensiMaks ?"}
    F -- Ya --> P4["frekuensiMaks := frekuensi<br>modus := data[i]"]
    F -- Tidak --> INC
    P4 --> INC["i := i + 1"]
    INC --> D1
    C2((C2)) --> Q{"frekuensiMaks sama dengan 1 ?"}
    Q -- Ya --> R1([Return NaN])
    Q -- Tidak --> R2([Return modus])
```

### Penjelasan Alur (`hitungModus`)
1. **Inisialisasi**: Asumsikan kandidat `modus = data[0]` dan frekuensi terbanyak saat ini `frekuensiMaks = 1`.
2. **Nested Loop (Perulangan Bersarang)**:
   * **Loop Luar (`i`)**: Mengiterasi setiap data sebagai acuan pembanding.
   * **Loop Dalam (`j`)**: Menghitung berapa kali nilai `data[i]` muncul di seluruh isi array. Setiap kali ada nilai yang sama (`data[i] == data[j]`), variabel `frekuensi` bertambah 1.
3. **Pembaruan Modus**:
   * Setelah *loop* dalam selesai, bandingkan `frekuensi` data saat ini dengan `frekuensiMaks`.
   * Jika `frekuensi > frekuensiMaks`, perbarui `frekuensiMaks` dengan nilai baru dan tetapkan kandidat `modus = data[i]`.
4. **Pemeriksaan Unik / Hasil Akhir**:
   * Setelah seluruh data selesai dihitung, cek `frekuensiMaks`.
   * Jika `frekuensiMaks == 1` (semua data hanya muncul 1 kali), kembalikan nilai `NaN` (Not a Number).
   * Jika ada frekuensi > 1, kembalikan nilai `modus`.
