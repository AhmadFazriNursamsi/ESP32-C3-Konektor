# ESP32-C3 Konektor

Komunikasi nirkabel langsung antara dua **ESP32-C3 menggunakan ESP-NOW**.

## Tujuan

Proyek ini menjadi dasar untuk membuat dua ESP32-C3 saling bertukar data tanpa router atau koneksi internet.

## Arsitektur

```
ESP32-C3 A  <====== ESP-NOW ======>  ESP32-C3 B
 Sender                              Receiver
```

ESP-NOW memungkinkan komunikasi peer-to-peer antar perangkat ESP dengan latensi rendah tanpa harus terhubung ke access point.

## Hardware

- 2 × ESP32-C3
- 2 × kabel USB untuk pemrograman
- Komputer untuk Arduino IDE atau PlatformIO

## Konsep komunikasi

1. Dapatkan MAC address kedua ESP32-C3.
2. Tentukan salah satu board sebagai sender dan board lainnya sebagai receiver.
3. Tambahkan MAC address receiver sebagai peer ESP-NOW pada sender.
4. Inisialisasi Wi-Fi dalam mode yang diperlukan oleh ESP-NOW.
5. Kirim payload dari sender.
6. Terima dan proses payload pada receiver.

## Contoh payload

Payload dapat dikembangkan menggunakan struktur data, misalnya:

```cpp
struct Data {
  int value;
  bool state;
};
```

## Pengembangan

Repository ini dapat dikembangkan menjadi:

- contoh kode sender dan receiver;
- callback ESP-NOW untuk pengiriman dan penerimaan;
- pertukaran sensor data;
- kontrol relay/aktuator;
- monitoring status koneksi;
- retry dan error handling.

## Catatan

Repository dibuat pada branch `main`. Saat ini proyek belum memiliki firmware atau konfigurasi build yang dapat didokumentasikan secara spesifik.

## Repository

[ESP32-C3-Konektor](https://github.com/AhmadFazriNursamsi/ESP32-C3-Konektor)
