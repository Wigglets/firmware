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
BLEScanResults foundDevices;

unsigned long lastScan;

NeighborInfo neighbors[10];


int scanTime = 1;
int scanInterval = 5000;

static void onScanComplete(BLEScanResults results) {

    int neighbourIndex = 0;
    for (int i = 0; i < results.getCount(); i++) {
        BLEAdvertisedDevice device = results.getDevice(i);
        if (device.getName() == BLE_DEVICE_NAME) {
            neighbors[neighbourIndex].id = String(device.getAddress().toString().c_str());
            neighbors[neighbourIndex].avg_rssi = device.getRSSI();
            neighbors[neighbourIndex].last_seen_ms = millis();

            neighbourIndex++;

            Serial.print("Found device "); Serial.print(i); Serial.print(": "); Serial.println(device.getAddress().toString().c_str());
            Serial.println("RSSI: "); Serial.println(device.getRSSI());
        }
    }

    pBLEScan->clearResults();
    pBLEScan->start(1, onScanComplete, false);
    lastScan = millis();
}

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
      pBLEScan->setActiveScan(true);
      pBLEScan->setInterval(100);
      pBLEScan->setWindow(99);

      pBLEScan->start(1, onScanComplete, false);
}

void BleRadio::loop() {

}

void BleRadio::getSocialContext(SocialContext &ctx) const {
    int totalNeighbors = 0;
    int closeNeighbors = 0;

    for (const auto neighbor : neighbors) {
        if (neighbor.avg_rssi < 0) {
            totalNeighbors++;
        }

        if (neighbor.avg_rssi > -50 && neighbor.avg_rssi < 0) {
            closeNeighbors++;
        }
    }

    ctx.total_neighbors = totalNeighbors;
    ctx.close_neighbors = closeNeighbors;
}