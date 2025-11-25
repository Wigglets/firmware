//
// Created by ub on 11/21/25.
//

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "BleRadio.h"

#include "config.h"
BLEScan* pBLEScan;
int scanTime = 1;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.getName() == BLE_DEVICE_NAME) {
            int rssi = advertisedDevice.getRSSI();
            String macAddress = advertisedDevice.getAddress().toString().c_str();
        }
    }
};

void BleRadio::begin(uint16_t selfId) {
    BLEDevice::init(BLE_DEVICE_NAME);

  // Advertising
  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(selfId));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  // Scanning
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // Actief scannen vindt apparaten sneller
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void BleRadio::loop() {
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    pBLEScan->clearResults();
    BLEDevice::startAdvertising();
}

void BleRadio::getSocialContext(SocialContext &ctx) const {
}