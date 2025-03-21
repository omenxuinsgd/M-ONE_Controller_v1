#include <AccelStepper.h>
#include <pt.h> // Include protothread library

#define DIR_PIN 4  // Pin untuk arah
#define STEP_PIN 2 // Pin untuk langkah

// Inisialisasi objek AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Definisikan kecepatan dan akselerasi
const int maxSpeed = 1000;      // Kecepatan maksimum (steps per second)
const int acceleration = 100;    // Akselerasi (steps per second^2)

static struct pt pt1; // Protothread variable
bool isRunning = false; // Flag untuk menandakan apakah motor sedang berjalan
bool lastDirectionUp = false; // Flag untuk menyimpan arah terakhir

void setup() {
    Serial.begin(115200);
    stepper.setMaxSpeed(maxSpeed);        // Set kecepatan maksimum
    stepper.setAcceleration(acceleration); // Set akselerasi
    Serial.println("Motor Controller Ready");
    
    PT_INIT(&pt1); // Initialize the protothread
}

void loop() {
    // Jalankan protothread
    checkSerial(&pt1);
    
    // Jalankan motor jika sedang berjalan
    if (isRunning) {
        stepper.runSpeed(); // Panggil fungsi runSpeed untuk memperbarui posisi motor
    }
}

static int checkSerial(struct pt *pt) {
    PT_BEGIN(pt);
    
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();  // Hapus karakter kosong

        if (command == "UP") {
            if (!isRunning || !lastDirectionUp) { // Jika motor tidak berjalan atau arah terakhir bukan UP
                stepper.stop(); // Hentikan motor
                delay(100); // Tunggu sejenak sebelum mengubah arah
                stepper.setCurrentPosition(0); // Reset posisi
                isRunning = true; // Set flag untuk mulai bergerak
                stepper.setSpeed(maxSpeed); // Set kecepatan positif untuk bergerak ke atas
                Serial.println("Moving UP");
                lastDirectionUp = true; // Update arah terakhir
            }
        }
        else if (command == "DOWN") {
            if (!isRunning || lastDirectionUp) { // Jika motor tidak berjalan atau arah terakhir adalah UP
                stepper.stop(); // Hentikan motor
                delay(100); // Tunggu sejenak sebelum mengubah arah
                stepper.setCurrentPosition(0); // Reset posisi
                isRunning = true; // Set flag untuk mulai bergerak
                stepper.setSpeed(-maxSpeed); // Set kecepatan negatif untuk bergerak ke bawah
                Serial.println("Moving DOWN");
                lastDirectionUp = false; // Update arah terakhir
            }
        }
        else if (command == "STOP") {
            isRunning = false; // Set flag untuk berhenti
            stepper.stop(); // Hentikan motor
            Serial.println("STOP");
        }
    }

    PT_END(pt);
}