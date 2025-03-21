# M-ONE Controller v1

## 🎯 Deskripsi
M-ONE Controller v1 adalah firmware berbasis ESP yang digunakan untuk mengontrol motor stepper dengan fitur limit switch untuk memastikan pergerakan yang akurat dan aman. Firmware ini menggunakan library **AccelStepper** untuk kontrol stepper dan **Protothreads (PT)** untuk manajemen multitasking ringan.

## 🔥 Fitur Utama
- **Autohoming** otomatis saat startup.
- **Kontrol Motor Stepper** menggunakan perintah serial.
- **Dua Limit Switch** untuk membatasi pergerakan motor.
- **Protothreading** untuk mengelola komunikasi serial dengan lebih efisien.
- **Proteksi Watchdog Timer** untuk mencegah reset ESP saat eksekusi panjang.

## 🛠️ Hardware yang Dibutuhkan
- ESP32 atau ESP8266
- Driver Motor Stepper (A4988, DRV8825, atau sejenisnya)
- Motor Stepper
- 2x Limit Switch
- Power Supply yang sesuai dengan kebutuhan motor

## 🚀 Cara Menggunakan
1. **Upload kode ke ESP32/ESP8266** menggunakan Arduino IDE.
2. **Hubungkan hardware** sesuai dengan pin yang telah didefinisikan.
3. **Buka Serial Monitor** dengan baudrate `115200`.
4. **Kirimkan perintah berikut** melalui Serial Monitor:
   - `UP` → Motor bergerak ke atas.
   - `DOWN` → Motor bergerak ke bawah.
   - `STOP` → Motor berhenti.
   - `UP_MANUAL` → Motor naik sebanyak 1000 langkah.
   - `DOWN_MANUAL` → Motor turun sebanyak 1000 langkah.

## 🎥 Demo Video
Lihat demo pengoperasian M-ONE Controller v1 di video berikut:
[![Demo Video](https://img.youtube.com/vi/VIDEO_ID/0.jpg)](https://github.com/omenxuinsgd/M-ONE_Controller_v1/blob/main/2025-03-21_09h20_39.mp4)

## 📜 Lisensi
Proyek ini dilisensikan di bawah MajoreIT License - lihat file `LICENSE` untuk detail lebih lanjut.

## 💡 Kontribusi
Jika Anda ingin berkontribusi, silakan buat pull request atau ajukan issue di repositori ini.

---
🚀 **Happy Coding!** 🚀

