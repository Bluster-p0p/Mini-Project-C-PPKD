#include <iostream>
#include <iomanip> // library mengatur format output
#include <cmath> // library matematika
#include <algorithm> // library algoritma

using namespace std;

struct Statistik { // menggunakan ilmu tentang struct. sebenaranya tidak diperlukan karena hanya input hanya 1 data set. tapi biar berguna saja materi tentang struct
    double mean;
    double median;
    double modus;
    double minimum;
    double maksimum;
    double range;

    double q1;
    double q3;
    double iqr;
    double lowerFence;
    double upperFence;

    double varians;
    double standarDeviasi;
    double skewness;

    int jumlahOutlier;
};

double hitungMean(double data[], int n) {
    double total = 0;

    for (int i = 0; i < n; i++) {
        total += data[i];
    }

    return total / n;
}

double hitungMedian(double data[], int n) {
    if (n % 2 == 0) {
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    } else {
        return data[n / 2];
    }
}

double hitungModus(double data[], int n) {
    double modus = data[0];
    int frekuensiMaks = 1;

    for (int i = 0; i < n; i++) {
        int frekuensi = 0;

        for (int j = 0; j < n; j++) {
            if (data[i] == data[j]) {
                frekuensi++;
            }
        }

        if (frekuensi > frekuensiMaks) {
            frekuensiMaks = frekuensi;
            modus = data[i];
        }
    }
    
    if (frekuensiMaks == 1) { // jika semua data memiliki frekuensi yang sama
        return NAN;
    }

    return modus;
}

double hitungQ1(double data[], int n) {
    int posisi = n / 4;

    if (n % 4 == 0) {
        return (data[posisi - 1] + data[posisi]) / 2.0;
    } else {
        return data[posisi];
    }
}

double hitungQ3(double data[], int n) {
    int posisi = (3 * n) / 4;

    if ((3 * n) % 4 == 0) {
        return (data[posisi - 1] + data[posisi]) / 2.0;
    } else {
        return data[posisi];
    }
}

double hitungVarians(double data[], int n, double mean) {
    double total = 0;

    for (int i = 0; i < n; i++) {
        total += pow(data[i] - mean, 2);
    }

    return total / n;
}

double hitungStandarDeviasi(double varians) {
    return sqrt(varians);
}

double hitungSkewness(double data[], int n, double mean, double standarDeviasi) {

    if (standarDeviasi == 0) {
        return 0;
    }

    double total = 0;

    for (int i = 0; i < n; i++) {
        total += pow((data[i] - mean) / standarDeviasi, 3);
    }

    return total / n;
}

double hitungZScore(double nilai, double mean, double standarDeviasi) {

    if (standarDeviasi == 0) {
        return 0;
    }

    return (nilai - mean) / standarDeviasi;
}

void tampilkanData(double data[], int n) {

    cout << "\nData setelah diurutkan:\n";

    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";

        if ((i + 1) % 10 == 0) {
            cout << endl;
        }
    }

    cout << endl;
}

void analisisOutlier(double data[], int n,
                     double mean,
                     double standarDeviasi,
                     double lowerFence,
                     double upperFence) {

    cout << "\n============================================\n";
    cout << "             ANALISIS OUTLIER\n";
    cout << "============================================\n";

    int jumlahOutlier = 0;

    for (int i = 0; i < n; i++) {

        double z = hitungZScore(data[i], mean, standarDeviasi);

        bool outlierFence =
            data[i] < lowerFence ||
            data[i] > upperFence;

        bool outlierZScore =
            fabs(z) > 3.0; // fabs untuk nilai mutlak

        if (outlierFence || outlierZScore) {

            cout << fixed << setprecision(2);

            cout << "Data: " << data[i]
                 << " | Z-Score: " << z;

            if (data[i] < lowerFence) {
                cout << " | Outlier bawah";
            }
            else if (data[i] > upperFence) {
                cout << " | Outlier atas";
            }
            else if (fabs(z) > 3.0) {
                cout << " | Outlier berdasarkan Z-Score";
            }

            cout << endl;

            jumlahOutlier++;
        }
    }

    if (jumlahOutlier == 0) {
        cout << "Tidak ditemukan outlier.\n";
    }
    else {
        cout << "\nJumlah outlier = "
             << jumlahOutlier << endl;
    }
}

void tampilkanKurva(double data[], int n) {

    cout << "\n============================================\n";
    cout << "\t\tKURVA FREKUENSI\n";
    cout << "============================================\n";

    double nilaiUnik[n];
    int frekuensi[n];

    int jumlahUnik = 0;

    for (int i = 0; i < n; i++) {

        bool sudahAda = false;

        for (int j = 0; j < jumlahUnik; j++) {

            if (data[i] == nilaiUnik[j]) {
                frekuensi[j]++;
                sudahAda = true;
                break;
            }
        }

        if (!sudahAda) {
            nilaiUnik[jumlahUnik] = data[i];
            frekuensi[jumlahUnik] = 1;
            jumlahUnik++;
        }
    }

    int frekuensiMaks = frekuensi[0];

    for (int i = 1; i < jumlahUnik; i++) {

        if (frekuensi[i] > frekuensiMaks) {
            frekuensiMaks = frekuensi[i];
        }
    }

    const int lebarKolom = 10;

    cout << "\n";

    for (int y = frekuensiMaks; y >= 1; y--) {

        // Sumbu Y
        cout << setw(4) << y << " |";

        // Isi grafik
        for (int x = 0; x < jumlahUnik; x++) {

            if (frekuensi[x] >= y) {
                cout << setw(lebarKolom) << "*";
            }
            else {
                cout << setw(lebarKolom) << " ";
            }
        }

        cout << endl;
    }

    cout << "     +"; // sumbu X

    for (int i = 0; i < jumlahUnik; i++) {

        cout << setw(lebarKolom)
             << "----------";
    }

    cout << endl;

    cout << "      ";

    for (int i = 0; i < jumlahUnik; i++) { // label nilai 

        cout << setw(lebarKolom) // setw berfungsi untuk mengatur lebar kolom output
             << fixed // adalah fungsi pelengkap dari setprecision
             << setprecision(2) // setprecision berfungsi untuk mengatur jumlah angka dibelakang koma
             << nilaiUnik[i];
    }

    cout << endl;

    cout << "\n";

    int lebarGrafik = jumlahUnik * lebarKolom;

    int posisiNilai = 5 + (lebarGrafik / 2) - 3;

    if (posisiNilai < 0) {
        posisiNilai = 0;
    }

    cout << string(posisiNilai, ' ') 
         << "Nilai\n";
}

void analisisEkor(double skewness) {

    cout << "\n============================================\n";
    cout << "           ANALISIS EKOR DISTRIBUSI\n";
    cout << "============================================\n";

    cout << fixed << setprecision(4);
    cout << "Skewness = " << skewness << endl;

    if (skewness > 0.5) {

        cout << "Distribusi memiliki ekor kanan.\n";
        cout << "Data cenderung miring ke kanan (positive skew).\n";

    }
    else if (skewness < -0.5) {

        cout << "Distribusi memiliki ekor kiri.\n";
        cout << "Data cenderung miring ke kiri (negative skew).\n";

    }
    else {

        cout << "Distribusi relatif simetris.\n";
        cout << "Ekor kiri dan kanan relatif seimbang.\n";
    }
}

int main() {
    int n;

    cout << "============================================\n";
    cout << "       PROGRAM ANALISIS STATISTIKA\n";
    cout << "============================================\n";

    do {
        cout << "\nMasukkan jumlah data: ";
        cin >> n;

        if (n <= 0) {
            cout << "Jumlah data harus lebih dari 0!\n";
        }

    } while (n <= 0);

    double data[n];

    cout << "\nMasukkan " << n << " data:\n";

    for (int i = 0; i < n; i++) {

        cout << "Data ke-" << i + 1 << ": ";
        cin >> data[i];
    }

    sort(data, data + n); // menggunakan fungsi sort 

    Statistik hasil;

    hasil.mean = hitungMean(data, n);

    hasil.median = hitungMedian(data, n);

    hasil.modus = hitungModus(data, n);

    hasil.minimum = data[0];

    hasil.maksimum = data[n - 1];

    hasil.range = hasil.maksimum - hasil.minimum;

    hasil.q1 = hitungQ1(data, n);

    hasil.q3 = hitungQ3(data, n);

    hasil.iqr = hasil.q3 - hasil.q1;

    hasil.lowerFence =
        hasil.q1 - 1.5 * hasil.iqr;

    hasil.upperFence =
        hasil.q3 + 1.5 * hasil.iqr;

    hasil.varians =
        hitungVarians(data, n, hasil.mean);

    hasil.standarDeviasi =
        hitungStandarDeviasi(hasil.varians);

    hasil.skewness =
        hitungSkewness(
            data,
            n,
            hasil.mean,
            hasil.standarDeviasi
        );

    tampilkanData(data, n);

    cout << "\n============================================\n";
    cout << "             HASIL ANALISIS\n";
    cout << "============================================\n";

    cout << fixed << setprecision(2);

    cout << "\n=> Mean\n";
    cout << "   = " << hasil.mean << endl;

    cout << "\n=> Median\n";
    cout << "   = " << hasil.median << endl;

    cout << "\n=> Modus\n";

    if (isnan(hasil.modus)) { // isnan untuk memeriksa apakah number atau bukan
        cout << "   Tidak terdapat modus.\n";
    }
    else {
        cout << "   = " << hasil.modus << endl;
    }

    cout << "\n=> Outlier\n";

    cout << "   Q1          = "
         << hasil.q1 << endl;

    cout << "   Q3          = "
         << hasil.q3 << endl;

    cout << "   IQR         = "
         << hasil.iqr << endl;

    cout << "   Lower Fence = "
         << hasil.lowerFence << endl;

    cout << "   Upper Fence = "
         << hasil.upperFence << endl;

    cout << "   Z-Score menggunakan batas |Z| > 3.\n";

    analisisOutlier(
        data,
        n,
        hasil.mean,
        hasil.standarDeviasi,
        hasil.lowerFence,
        hasil.upperFence
    );

    cout << "\n=> Nilai Maximum & Minimum\n";

    cout << "   Maximum = "
         << hasil.maksimum << endl;

    cout << "   Minimum = "
         << hasil.minimum << endl;

    cout << "\n=> Range\n";

    cout << "   Range = Maximum - Minimum\n";
    cout << "         = "
         << hasil.range << endl;

    cout << "\n=> Varians\n";

    cout << "   = "
         << hasil.varians << endl;

    cout << "\n=> Standar Deviasi\n";

    cout << "   = "
         << hasil.standarDeviasi << endl;

    cout << "\n=> Ekor Kiri & Kanan\n";

    analisisEkor(hasil.skewness);

    cout << "\n10. => Kurva\n";

    tampilkanKurva(data, n);

    return 0;
}