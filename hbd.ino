#define RELAY_PIN 3  
#define BUZZER_PIN 8 

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_B4, NOTE_B4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

int noteDurations[] = {
  300, 300, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 1200
};

const int totalNotes = sizeof(melody) / sizeof(int);
int currentNote = 0;
unsigned long previousMillis = 0;
unsigned long interval = 0;

unsigned long lastBlinkMillis = 0;
const unsigned long blinkInterval = 3000; 
bool relayState = false;
bool playMusic = false;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  
  Serial.begin(9600);
  Serial.println("SIYAAPPP");
}

void loop() {
  checkSerialCommands();
  handleMusicPlayback();
  handleBlinking();
}

void checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "ON_OFF") {
      relayState = !relayState;
      digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
      Serial.println(relayState ? "Relay ON" : "Relay OFF");
    } 
    else if (command == "PLAY") {
      playMusic = true;
      currentNote = 0;
      previousMillis = millis();
      interval = 0;
      lastBlinkMillis = millis(); 
      Serial.println("putar lagu");
    }
  }
}

void handleMusicPlayback() {
  if (playMusic) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (currentNote < totalNotes) {
        tone(BUZZER_PIN, melody[currentNote], noteDurations[currentNote]);
        interval = noteDurations[currentNote] + 30;
        currentNote++;
      } else {
        noTone(BUZZER_PIN);
        digitalWrite(RELAY_PIN, LOW); 
        interval = 2000;
        playMusic = false;
        Serial.println("lagu selesai");
      }
    }
  }
}

void handleBlinking() {
  if (playMusic) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkMillis >= blinkInterval) {
      lastBlinkMillis = currentMillis;
      relayState = !relayState; 
      digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
    }
  }
}
