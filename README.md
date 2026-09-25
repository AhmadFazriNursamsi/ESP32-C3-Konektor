# ESP32-C3 Konektor

Komunikasi nirkabel **peer-to-peer** antara dua board **ESP32-C3** menggunakan **ESP-NOW**, tanpa memerlukan router atau koneksi internet.

> **Status proyek:** dokumentasi dan rancangan awal. Repository saat ini berisi README dan belum menyertakan firmware/build configuration.

---

## Daftar Isi

- [Tentang Proyek](#tentang-proyek)
- [Tujuan](#tujuan)
- [Arsitektur Sistem](#arsitektur-sistem)
- [Hardware](#hardware)
- [Software](#software)
- [Cara Kerja ESP-NOW](#cara-kerja-esp-now)
- [Konsep Sender dan Receiver](#konsep-sender-dan-receiver)
- [Contoh Payload](#contoh-payload)
- [Persiapan Pengembangan](#persiapan-pengembangan)
- [Alur Implementasi](#alur-implementasi)
- [Debugging dan Troubleshooting](#debugging-dan-troubleshooting)
- [Rencana Pengembangan](#rencana-pengembangan)
- [Struktur Repository](#struktur-repository)
- [Kontribusi](#kontribusi)
- [Lisensi](#lisensi)

## Tentang Proyek

**ESP32-C3 Konektor** adalah proyek eksperimen untuk membangun komunikasi langsung antar dua ESP32-C3 menggunakan **ESP-NOW**.

ESP-NOW merupakan protokol komunikasi nirkabel yang tersedia pada ekosistem ESP32 dan dapat digunakan untuk pertukaran data antar perangkat tanpa harus membuat koneksi ke access point seperti pada komunikasi Wi-Fi biasa.

Proyek ini dirancang sebagai fondasi untuk aplikasi seperti:

- komunikasi antar node IoT;
- pertukaran data sensor;
- kendali relay atau aktuator;
- remote control;
- monitoring status perangkat;
- komunikasi dua arah antar mikrokontroler.

## Tujuan

Tujuan utama proyek:

1. Menghubungkan dua ESP32-C3 secara langsung.
2. Mengirim data dengan ESP-NOW.
3. Menerima dan memproses data pada perangkat tujuan.
4. Menyediakan fondasi yang mudah dikembangkan untuk aplikasi IoT.
5. Mendokumentasikan konfigurasi dan konsep komunikasi agar mudah direplikasi.

## Arsitektur Sistem

Arsitektur dasar proyek:

```text
┌─────────────────┐              ┌─────────────────┐
│    ESP32-C3 A   │              │    ESP32-C3 B   │
│     Sender      │              │    Receiver     │
└────────┬────────┘              └────────┬────────┘
         │                                │
         └──────────── ESP-NOW ───────────┘
                    Peer-to-Peer
```

Dalam implementasi berikutnya, komunikasi dapat dibuat **satu arah** maupun **dua arah**. Peran sender dan receiver juga dapat disesuaikan dengan kebutuhan aplikasi.

## Hardware

Komponen minimum:

| Komponen | Jumlah | Keterangan |
|---|---:|---|
| ESP32-C3 | 2 | Satu atau dua node komunikasi |
| Kabel USB | 2 | Pemrograman dan catu daya |
| Komputer | 1 | Arduino IDE, PlatformIO, atau toolchain lain |

Tidak diperlukan router Wi-Fi untuk komunikasi ESP-NOW antar peer.

> Pin GPIO yang digunakan belum ditentukan pada repository ini. Pinout sebaiknya ditambahkan setelah firmware dan rangkaian perangkat ditetapkan.

## Software

Toolchain yang dapat digunakan untuk pengembangan:

- **Arduino IDE** dengan dukungan ESP32;
- **PlatformIO**;
- ESP-IDF untuk implementasi yang lebih lanjut.

Library/API yang dibutuhkan bergantung pada framework yang dipilih. Untuk Arduino-ESP32, implementasi ESP-NOW menggunakan API ESP-NOW yang tersedia pada core ESP32.

## Cara Kerja ESP-NOW

Secara umum, proses komunikasi adalah:

1. ESP32-C3 melakukan inisialisasi sistem.
2. Wi-Fi diaktifkan pada konfigurasi yang diperlukan.
3. ESP-NOW diinisialisasi.
4. MAC address perangkat tujuan diketahui.
5. Perangkat tujuan ditambahkan sebagai **peer**.
6. Sender mengirim payload.
7. Receiver menerima payload melalui callback penerimaan.
8. Data divalidasi dan diproses oleh aplikasi.

Contoh alur:

```text
Start
  │
  ▼
Initialize Wi-Fi
  │
  ▼
Initialize ESP-NOW
  │
  ▼
Register Peer
  │
  ▼
Create Payload
  │
  ▼
Send Data ───────────────► Receive Data
  │                            │
  ▼                            ▼
Check Result              Validate Payload
                               │
                               ▼
                          Process Data
```

## Konsep Sender dan Receiver

### Sender

Sender bertugas membuat dan mengirim data ke perangkat tujuan.

Contoh data yang dapat dikirim:

- nilai sensor;
- status tombol;
- status relay;
- counter;
- command;
- struktur data gabungan.

### Receiver

Receiver menerima payload dan melakukan pemrosesan sesuai kebutuhan aplikasi.

Contoh:

- menampilkan data melalui Serial Monitor;
- mengubah status GPIO;
- mengaktifkan relay;
- menyimpan nilai sensor;
- mengirim respons kembali ke sender.

## Contoh Payload

Payload dapat direpresentasikan menggunakan struktur C/C++ sederhana:

```cpp
struct Data {
  int value;
  bool state;
};
```

Untuk proyek nyata, struktur payload sebaiknya dibuat eksplisit dan konsisten antara sender dan receiver.

Contoh pengembangan:

```cpp
struct Message {
  uint32_t id;
  int value;
  bool state;
};
```

Jika komunikasi berkembang menjadi sistem yang lebih kompleks, pertimbangkan menambahkan:

- nomor pesan;
- timestamp;
- tipe command;
- checksum/validasi;
- status/error code;
- versi format payload.

## Persiapan Pengembangan

### 1. Siapkan dua ESP32-C3

Hubungkan masing-masing board ke komputer melalui USB.

### 2. Identifikasi MAC address

MAC address setiap board diperlukan untuk konfigurasi peer ESP-NOW.

MAC address dapat dicetak melalui Serial Monitor menggunakan firmware pengujian yang sesuai.

### 3. Tentukan peran perangkat

Misalnya:

```text
ESP32-C3 #1 → Sender
ESP32-C3 #2 → Receiver
```

Untuk komunikasi dua arah, kedua perangkat dapat dikonfigurasi sebagai peer dan masing-masing dapat mengirim maupun menerima data.

### 4. Tentukan payload

Gunakan struktur data yang sama pada kedua perangkat.

### 5. Implementasikan callback

Gunakan callback pengiriman dan penerimaan untuk mengetahui status komunikasi dan memproses data yang masuk.

## Alur Implementasi

Implementasi firmware yang direkomendasikan:

### Tahap 1 — Tes board

Pastikan kedua ESP32-C3 dapat:

- diprogram;
- melakukan boot;
- terhubung ke Serial Monitor;
- menjalankan program sederhana.

### Tahap 2 — Tes identitas perangkat

Cetak MAC address masing-masing board dan catat hasilnya.

### Tahap 3 — Inisialisasi ESP-NOW

Tambahkan inisialisasi ESP-NOW dan konfigurasi peer.

### Tahap 4 — Kirim data sederhana

Mulai dengan payload sederhana seperti integer atau string sebelum menggunakan struktur data yang lebih kompleks.

### Tahap 5 — Tambahkan callback

Tambahkan callback untuk:

- status pengiriman;
- data diterima.

### Tahap 6 — Tambahkan validasi

Pastikan data yang diterima memiliki ukuran dan format yang sesuai sebelum digunakan.

### Tahap 7 — Integrasikan aplikasi

Setelah komunikasi stabil, integrasikan sensor, tombol, relay, aktuator, atau perangkat lain.

## Debugging dan Troubleshooting

### ESP32-C3 tidak menerima data

Periksa:

- MAC address peer;
- peer sudah ditambahkan;
- ESP-NOW berhasil diinisialisasi;
- kedua perangkat menggunakan konfigurasi channel yang kompatibel;
- perangkat tujuan sedang aktif.

### Pengiriman gagal

Periksa:

- alamat MAC tujuan;
- status peer;
- jarak antar perangkat;
- channel Wi-Fi;
- hasil callback pengiriman.

### Data diterima tetapi nilainya salah

Periksa:

- definisi struct pada kedua perangkat;
- ukuran payload;
- tipe data;
- urutan field;
- perubahan struktur antara sender dan receiver.

Hindari langsung meng-cast buffer menjadi struktur tanpa memastikan ukuran dan format datanya sesuai.

### Koneksi tidak konsisten

Untuk aplikasi yang membutuhkan keandalan lebih tinggi, pertimbangkan:

- nomor urut paket;
- acknowledgement (ACK);
- timeout;
- retry;
- validasi payload;
- mekanisme deteksi perangkat offline.

> ESP-NOW menyediakan mekanisme komunikasi nirkabel, tetapi keandalan aplikasi tetap perlu dirancang sesuai kebutuhan sistem.

## Rencana Pengembangan

Fitur yang dapat ditambahkan ke repository:

- [ ] Firmware sender
- [ ] Firmware receiver
- [ ] Contoh komunikasi dua arah
- [ ] Callback pengiriman dan penerimaan
- [ ] Konfigurasi MAC address melalui file konfigurasi
- [ ] Contoh payload berbasis struct
- [ ] ACK dan retry
- [ ] Sequence number
- [ ] Validasi payload
- [ ] Monitoring status komunikasi
- [ ] Contoh sensor
- [ ] Contoh kontrol relay/aktuator
- [ ] Dokumentasi wiring
- [ ] Konfigurasi Arduino IDE
- [ ] Konfigurasi PlatformIO
- [ ] Dokumentasi pengujian dan hasil

## Struktur Repository

Struktur repository saat ini masih minimal:

```text
ESP32-C3-Konektor/
└── README.md
```

Struktur yang dapat digunakan ketika firmware mulai dikembangkan:

```text
ESP32-C3-Konektor/
├── README.md
├── sender/
│   └── sender.ino
├── receiver/
│   └── receiver.ino
├── examples/
│   └── basic-esp-now/
└── docs/
    └── wiring.md
```

Struktur tersebut merupakan rekomendasi dan belum semuanya tersedia di repository.

## Catatan Teknis

- ESP-NOW tidak membutuhkan router untuk komunikasi langsung antar peer.
- MAC address harus dikonfigurasi dengan benar.
- Konfigurasi radio, termasuk channel, perlu diperhatikan ketika perangkat berkomunikasi.
- Payload harus memiliki format yang diketahui oleh kedua sisi.
- Untuk aplikasi produksi, tambahkan mekanisme validasi dan error handling yang sesuai.
- Detail API dapat berbeda tergantung versi Arduino-ESP32 atau ESP-IDF yang digunakan.

## Kontribusi

Kontribusi dan pengembangan lebih lanjut dipersilakan.

Alur kontribusi yang disarankan:

1. Fork repository.
2. Buat branch fitur.
3. Implementasikan perubahan.
4. Uji pada ESP32-C3.
5. Perbarui dokumentasi jika diperlukan.
6. Buat Pull Request dengan penjelasan perubahan.

## Lisensi

Repository saat ini **belum mencantumkan lisensi**.

Jika proyek akan didistribusikan atau digunakan oleh pihak lain, tambahkan file lisensi yang sesuai, misalnya MIT, Apache-2.0, atau lisensi lain sesuai kebutuhan pemilik proyek.

## Repository

urlESP32-C3-Konektor di GitHubhttps://github.com/AhmadFazriNursamsi/ESP32-C3-Konektor

---

**ESP32-C3 Konektor** — membangun komunikasi langsung antar ESP32-C3 dengan ESP-NOW.
