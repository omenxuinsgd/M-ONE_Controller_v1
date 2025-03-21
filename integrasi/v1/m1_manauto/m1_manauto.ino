#include <AccelStepper.h>
#include <pt.h> // Include protothread library
#include <ESP.h>

#define DIR_PIN 4  // Pin untuk arah
#define STEP_PIN 2 // Pin untuk langkah
#define LIMIT_SWITCH_PIN 5  // Pin limit switch
#define LIMIT_SWITCH_PIN2 0  // Pin limit switch

#define BACKSTEP 100

// Inisialisasi objek AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Definisikan kecepatan dan akselerasi
const int maxSpeed = 1000;      // Kecepatan maksimum (steps per second)
const int acceleration = 100;    // Akselerasi (steps per second^2)

static struct pt pt1; // Protothread variable
bool isRunning = false; // Flag untuk menandakan apakah motor sedang berjalan
bool lastDirectionUp = false; // Flag untuk menyimpan arah terakhir
bool isRunning2 = false;

void setup() {
    pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Menggunakan internal pull-up resistor
    pinMode(LIMIT_SWITCH_PIN2, INPUT_PULLUP);  // Menggunakan internal pull-up resistor
    // pinMode(DIR_PIN, OUTPUT);  // Menggunakan internal pull-up resistor
    // pinMode(STEP_PIN, OUTPUT);  // Menggunakan internal pull-up resistor
    
    Serial.begin(115200);

    // ESP.wdtDisable();

    // Autohoming
    Serial.println("Melakukan autohoming...");
    digitalWrite(DIR_PIN, LOW);
    while (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(10);
      Serial.println("Homing...");
    }
    digitalWrite(STEP_PIN, LOW); // Motor berhenti saat menyentuh limit switch  
    // Mundur sedikit
    delay(500);
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(STEP_PIN, HIGH);
    delay(200);
    Serial.println("Autohoming selesai!");

    stepper.setMaxSpeed(maxSpeed);        // Set kecepatan maksimum
    stepper.setAcceleration(acceleration); // Set akselerasi
    Serial.println("Motor Controller Ready");
    PT_INIT(&pt1); // Initialize the protothread
}

void loop() {
    // Jalankan protothread
    checkSerial(&pt1);
    
    // Jika motor sedang berjalan, jalankan dengan pengecekan limit switch
    if (isRunning) {
        if (digitalRead(LIMIT_SWITCH_PIN) == LOW) {  // Jika limit switch tertekan
            isRunning = false;
            stepper.stop();
            Serial.println("LIMIT SWITCH ATAS PRESSED! Motor stopped.");
            
            // Berbalik arah beberapa langkah
            delay(100);  // Tunggu sebentar sebelum bergerak mundur
            Serial.println("Reversing motor...");
            
            stepper.move(-BACKSTEP);  // Mundur beberapa langkah
            unsigned long reverseStart = millis();
            
            while (stepper.distanceToGo() != 0) {
                stepper.run();  // Jalankan motor mundur
                
                // Cegah reset ESP dengan memberikan sinyal ke Watchdog Timer
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined(ESP32)
                esp_task_wdt_reset();
                #endif
                
                // Hindari loop terlalu lama
                if (millis() - reverseStart > 5000) {  // Timeout 5 detik
                    Serial.println("Reverse timeout! Stopping motor.");
                    stepper.stop();
                    break;
                }
            }

            Serial.println("Reverse complete.");
            stepper.setCurrentPosition(0);  // Reset posisi agar tetap sinkron
        }else if (digitalRead(LIMIT_SWITCH_PIN2) == LOW) {  // Jika limit switch tertekan
            isRunning = false;
            stepper.stop();
            Serial.println("LIMIT SWITCH BAWAH PRESSED! Motor stopped.");
            
            // Berbalik arah beberapa langkah
            delay(100);  // Tunggu sebentar sebelum bergerak mundur
            Serial.println("Reversing motor...");
            
            stepper.move(BACKSTEP);  // Mundur beberapa langkah
            unsigned long reverseStart = millis();
            
            while (stepper.distanceToGo() != 0) {
                stepper.run();  // Jalankan motor mundur
                
                // Cegah reset ESP dengan memberikan sinyal ke Watchdog Timer
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined(ESP32)
                esp_task_wdt_reset();
                #endif
                
                // Hindari loop terlalu lama
                if (millis() - reverseStart > 5000) {  // Timeout 5 detik
                    Serial.println("Reverse timeout! Stopping motor.");
                    stepper.stop();
                    break;
                }
            }

            Serial.println("Reverse complete.");
            stepper.setCurrentPosition(0);  // Reset posisi agar tetap sinkron
        } else {
            stepper.runSpeed(); // Perbarui posisi motor jika tidak ada batasan
        }
    }
}

static int checkSerial(struct pt *pt) {
    PT_BEGIN(pt);
    
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();  // Hapus karakter kosong di awal/akhir

        if (command == "UP") {
            if (!isRunning || !lastDirectionUp) { // Jika motor tidak berjalan atau arah terakhir bukan UP
                stepper.stop(); // Hentikan motor
                delay(100); // Tunggu sebelum mengubah arah
                stepper.setCurrentPosition(0); // Reset posisi
                isRunning = true;
                stepper.setSpeed(maxSpeed); // Set kecepatan positif untuk naik
                Serial.println("Moving UP");
                lastDirectionUp = true;
            }
        }
        else if (command == "DOWN") {
            if (!isRunning || lastDirectionUp) { // Jika motor tidak berjalan atau arah terakhir adalah UP
                stepper.stop();
                delay(100);
                stepper.setCurrentPosition(0);
                isRunning = true;
                stepper.setSpeed(-maxSpeed); // Set kecepatan negatif untuk turun
                Serial.println("Moving DOWN");
                lastDirectionUp = false;
            }
        }
        else if (command == "STOP") {
            isRunning = false;
            stepper.stop();
            Serial.println("STOP");
        }
        else if (command == "UP_MANUAL") {
            if (!isRunning) { // Jika motor tidak berjalan
                stepper.stop(); // Hentikan motor jika sedang berjalan
                delay(100); // Tunggu sebelum mengubah arah
                stepper.setCurrentPosition(stepper.currentPosition() + 1000); // Tambahkan 1000 langkah
                isRunning = true;
                stepper.setSpeed(maxSpeed); // Set kecepatan positif untuk naik
                Serial.println("Moving UP_MANUAL");
            }
        }
        else if (command == "DOWN_MANUAL") {
            if (!isRunning) { // Jika motor tidak berjalan
                stepper.stop(); // Hentikan motor jika sedang berjalan
                delay(100); // Tunggu sebelum mengubah arah
                stepper.setCurrentPosition(stepper.currentPosition() - 1000); // Kurangi 1000 langkah
                isRunning = true;
                stepper.setSpeed(-maxSpeed); // Set kecepatan negatif untuk turun
                Serial.println("Moving DOWN_MANUAL");
            }
        }
        else {
            Serial.println("Unknown command");
        }
    }
    PT_END(pt);
}
