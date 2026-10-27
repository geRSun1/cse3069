#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11

#define TOTAL_SAMPLES 30

DHT dht(DHTPIN, DHTTYPE);

int sampleCount = 0;
bool finished = false;

void setup() {
  Serial.begin(115200);
  dht.begin();

  delay(1000);

  Serial.println("timestamp,temperature,humidity,valid");
}

void loop() {

  if (finished) {
    return;      // dừng hoàn toàn
  }

  unsigned long ts = millis();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.print(ts);
    Serial.println(",0,0,0");
  } else {
    Serial.print(ts);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(h);
    Serial.println(",1");
  }

  sampleCount++;

  if (sampleCount >= TOTAL_SAMPLES) {
    Serial.println("END_OF_DATA");
    finished = true;

    // hoặc ngủ vô hạn
    while (1) {
      delay(1000);
    }
  }

  delay(2000);
}