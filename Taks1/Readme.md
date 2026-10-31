
# DP-Week1 Task 1 - Balatro-Like Core Loop

## Nama Kelompok

1. Gea Ismawianda Aulia Firdausy - 5225600117
2. Rizky Eka Aji Santoso - 5225600119
3. Shelen ahmad kahfi - 5225600104
4. Kalvin Luna Sinuraya - 5225600120
5. I Putu Satwika Werdi Widagda Karang - 5225600112
6. Ignatius Davin Keane Souisa - 5225600092
7. Moch Salman Al Fariz - 5225600107

---

## Deskripsi Proyek

Proyek ini merupakan implementasi sederhana sistem permainan yang terinspirasi dari Balatro menggunakan C++.

Program memiliki core loop sebanyak tiga ronde. Setiap ronde menjalankan enam fase utama dengan urutan yang tetap.

### Core Loop

1. Generate Input
2. Compute Base Score
3. Compute Reward
4. Update Money
5. Shop Phase
6. Advance Round

### Class Utama

- `RunSession`: Mengatur jalannya permainan dan urutan fase.
- `IInputGenerator`: Interface untuk menghasilkan input.
- `FixedInputGenerator`: Menghasilkan angka tetap, yaitu 3.
- `RandomInputGenerator`: Menghasilkan angka acak dari 1 sampai 5.
- `IScoringRule`: Interface untuk perhitungan skor.
- `SimpleScoringRule`: Menggunakan nilai input sebagai skor dasar.
- `IRewardRule`: Interface untuk menghitung reward.
- `SimpleRewardRule`: Menghasilkan reward sebesar skor dasar.
- `BonusRewardRule`: Menghasilkan reward sebesar skor dasar ditambah 2.
- `ShopSystem`: Menampilkan penawaran toko pada setiap ronde.

---

## Tugas Tambahan - Poker Hand Checker

Pada pengembangan berikutnya, program ditambahkan sistem Poker Hand Checker berdasarkan class diagram yang diberikan.

Implementasi menggunakan konsep **Chain of Responsibility Pattern**, yaitu pemeriksaan dilakukan secara berantai dari satu checker ke checker berikutnya.

### Class yang Ditambahkan

- `PokerHandChecker`: Base class untuk seluruh checker.
- `HandGenerator`: Menghasilkan objek Hand.
- `HandPlayer`: Menentukan jenis hand yang akan diuji.
- `ScoringRule`: Mengatur rantai checker dan menghubungkan hasil pemeriksaan.
- `BlindRule`: Menangani pemeriksaan target skor.
- `RewardRule`: Menangani perhitungan reward.
- `GameManager`: Mengatur interaksi antara komponen-komponen tersebut.

### 13 Poker Hand Checker

Implementasi terdiri dari 13 kelas checker:

1. High Card Checker
2. Pair Checker
3. Two Pair Checker
4. Three of a Kind Checker
5. Straight Checker
6. Flush Checker
7. Full House Checker
8. Four of a Kind Checker
9. Straight Flush Checker
10. Royal Flush Checker
11. Five of a Kind Checker
12. Flush House Checker
13. Flush Five Checker

Setiap checker memiliki fungsi `check()` untuk memeriksa jenis hand. Fungsi `setNext()` digunakan untuk menghubungkan checker dengan checker berikutnya.

Sesuai instruksi tugas, implementasi ini berfokus pada struktur kelas dan pencetakan jenis hand, tanpa implementasi detail algoritma kombinasi kartu.

### Hasil Pengujian

Seluruh 13 checker diuji melalui program secara otomatis. Hasil pengujian menampilkan nama masing-masing jenis hand.

---

## Cara Menjalankan Program

Gunakan compiler C++ yang mendukung C++11.

Compile:

```bash
g++ -std=c++11 main.cpp -o main
```

Jalankan:

```bash
./main
```

Untuk Windows PowerShell:

```powershell
g++ -std=c++11 main.cpp -o main.exe
.\main.exe
```

---

## Reflection

### 1. What is the invariant structure in your program?

Struktur invariant dalam program ini adalah alur eksekusi fase permainan di dalam kelas `RunSession`. Urutan enam fase tersebut mencakup pembuatan input, perhitungan skor dasar, perhitungan reward, pembaruan jumlah uang, fase toko, dan perpindahan ronde. Urutan ini bersifat tetap dan tidak boleh diubah karena setiap fase bergantung pada hasil fase sebelumnya.

### 2. Which parts are mutable?

Bagian yang bersifat mutable adalah implementasi spesifik dari setiap modul pendukung. Hal ini mencakup algoritma pembuatan input pada `IInputGenerator`, aturan kalkulasi poin pada `IScoringRule`, formula perolehan uang pada `IRewardRule`, serta item penawaran pada `ShopSystem`. Implementasi tersebut dapat diubah atau diganti tanpa memengaruhi urutan fase utama.

Pada tugas tambahan, jenis dan implementasi `PokerHandChecker` juga dapat dikembangkan tanpa harus mengubah keseluruhan alur permainan.

### 3. When you replaced the InputGenerator, why didn't RunSession change?

Kelas `RunSession` tidak mengalami perubahan karena menggunakan abstraksi melalui interface `IInputGenerator`. `RunSession` hanya bergantung pada kontrak metode `generate()`, bukan pada kelas konkret tertentu. Ketika `FixedInputGenerator` diganti dengan `RandomInputGenerator`, `RunSession` tetap memanggil metode yang sama tanpa perlu mengetahui bagaimana input acak dihasilkan di dalamnya.

### 4. What would happen if scoring logic was placed inside RunSession?

Jika logika perhitungan skor diletakkan langsung di dalam `RunSession`, maka kelas tersebut akan memiliki terlalu banyak tanggung jawab. Setiap perubahan cara menghitung skor akan mengharuskan perubahan pada kelas pengatur alur permainan. Hal ini meningkatkan ketergantungan antarbagian kode dan berisiko merusak urutan fase yang seharusnya tetap. Oleh karena itu, pemisahan scoring ke dalam `IScoringRule` dan implementasinya membuat program lebih mudah dikembangkan dan dipelihara.

---

## Teknologi

- Programming Language: C++
- Compiler Standard: C++11
- Version Control: Git
