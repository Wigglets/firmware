/*
 * ESP32 Universele Node (Scanner + Advertiser)
 * Flash deze code op ALLE ESP32's.
 * Elke ESP zendt zijn aanwezigheid uit EN luistert naar anderen.
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// De "familienaam" van onze apparaten.
// Alleen apparaten met deze naam worden gerapporteerd.
#define DEVICE_NAME "ESP32_NODE"

// Variabelen voor scannen
BLEScan* pBLEScan;
int scanTime = 1; // Scan tijd in seconden (kort houden voor responsiviteit)

// --- CALLBACK: Wat te doen als we een apparaat vinden ---
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      // Check of het gevonden apparaat "een van ons" is
      if (advertisedDevice.getName() == DEVICE_NAME) {

        int rssi = advertisedDevice.getRSSI();
        String macAddress = advertisedDevice.getAddress().toString().c_str();

        Serial.print("Node gevonden! [");
        Serial.print(macAddress); // Uniek ID van de andere ESP
        Serial.print("] RSSI: ");
        Serial.print(rssi);

        // Afstand schatten
        if (rssi > -50) {
           Serial.println(" -> STATUS: HEEL DICHTBIJ (IMMEDIATE)");
        } else if (rssi > -75) {
           Serial.println(" -> STATUS: IN DE BUURT (NEAR)");
        } else {
           Serial.println(" -> STATUS: VER WEG (FAR)");
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Start Universele Node...");

  // 1. Initialiseer BLE met de gedeelde naam
  BLEDevice::init(DEVICE_NAME);

  // --- DEEL A: ADVERTEREN (ZENDEN) ---
  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x1800));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Adverteren gestart...");

  // --- DEEL B: SCANNEN (ONTVANGEN) ---
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // Actief scannen vindt apparaten sneller
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  // 1. Scan naar anderen gedurende 'scanTime' seconden
  // 'false' betekent dat we niet stoppen na de eerste vondst, maar doorgaan tot de tijd om is
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  // 2. Resultaten opruimen voor geheugenbeheer
  pBLEScan->clearResults();

  // 3. BELANGRIJK: Adverteren herstarten
  // Sommige BLE stacks stoppen met zenden tijdens een actieve scan.
  // Voor de zekerheid roepen we startAdvertising() opnieuw aan.
  BLEDevice::startAdvertising();

  // 4. Willekeurige vertraging
  // Dit voorkomt dat twee apparaten in een "lock-step" komen waarbij
  // ze tegelijk scannen en tegelijk zwijgen, waardoor ze elkaar missen.
  delay(10);
}