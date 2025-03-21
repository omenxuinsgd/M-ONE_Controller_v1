#define LIMIT_SWITCH_PIN 5  // Pin limit switch
#define LIMIT_SWITCH_PIN2 0  // Pin limit switch
// #define LED_BUILTIN D4       // LED internal Wemos D1 Mini

void setup() {
    pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Menggunakan internal pull-up resistor
    pinMode(LIMIT_SWITCH_PIN2, INPUT_PULLUP);  // Menggunakan internal pull-up resistor
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    int switchState = digitalRead(LIMIT_SWITCH_PIN);
    int switchState2 = digitalRead(LIMIT_SWITCH_PIN2);

    if (switchState == LOW) {  // Jika limit switch ditekan (terhubung ke GND)
        digitalWrite(LED_BUILTIN, LOW); // LED menyala (Wemos D1 LED aktif LOW)
        Serial.println("Limit Switch 1 DITEKAN!");
    } else {
        digitalWrite(LED_BUILTIN, HIGH); // LED mati
        Serial.println("Limit Switch 1 TIDAK ditekan.");
    }

    if (switchState2 == LOW) {  // Jika limit switch ditekan (terhubung ke GND)
        digitalWrite(LED_BUILTIN, LOW); // LED menyala (Wemos D1 LED aktif LOW)
        Serial.println("Limit Switch 2 DITEKAN!");
    } else {
        digitalWrite(LED_BUILTIN, HIGH); // LED mati
        Serial.println("Limit Switch 2 TIDAK ditekan.");
    }
    
    delay(100); // Debounce sederhana
}
