# Final-Project-KWHAF
### Projek ini berfungsi agar data detak jantung yang diukur oleh wearable dapat langsung dikirim setiap detik ke server, tanpa harus diunduh secara manual. Dengan demikian, server dapat segera melakukan analisis untuk mendeteksi kondisi abnormal seperti bradikardia (detak lambat) atau takikardia (detak cepat). Jika terdeteksi anomali, data akan disimpan dalam bentuk file biner sebagai buffer, dan diekspor ke file JSON sebagai laporan medis yang dapat ditinjau lebih lanjut.

### Output dari sistem ini mencakup file log biner dan JSON yang berisi data detak abnormal lengkap dengan waktu kejadian dan statusnya, serta dapat digunakan untuk memberikan peringatan langsung kepada pengguna melalui tampilan aplikasi.

### Cara run program:
### 1. Pada VScode, buka terminal baru, lalu split terminal menjadi dua
### 2. Pada salah satu terminal, ketik .\server\server.exe , lalu tekan enter
### 3. Akan muncul [SERVER] waiting for connncetion...
### 4. Pada terminal yang lain, ketik .\client\client.exe , lalu tekan enter
### 5. Server dan client akan terhubung
### 6. Pada terminal client, akan muncul data detak jantung yang akan dibaca
### 7. Pada terminal server, akan muncul hasil pembacaan dan pengklasifikasian detak jantung dengan keadaannya, apakah normal, takikardia, atau bradikardia
### 8. File anomaly_log.bin, daily_report.jason, dan report.json akan muncul dari folder \data. daily_report.json akan menampilkan timestamp, bpm, serta status dari seluruh detak jantung, report.json akan menampilkan timestamp, bpm, serta status khusus untuk detak jantung yang tidak normal, serta anomaly_log.bin akan menunjukkan data mana saja yang termasuk detak jantung tidak normal