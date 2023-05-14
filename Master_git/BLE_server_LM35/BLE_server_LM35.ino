
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#define PIN_LM35       35


#define SERVICE_UUID        "b93bf421-f499-4e4d-994e-462ce5e85078"
//#define CHARACTERISTIC_UUID "d2d8bd48-8380-4fbc-ba3c-303af1851b64"

BLECharacteristic LMTempCHARACTERISTIC ("2af5685c-3d9e-4d2b-a1ba-26534e9cbf01",BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor LMTempsDescriptor(BLEUUID((uint16_t)0x2902));



bool deviceConnected = false;

float temp;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
///CHARACTERISTIC_UUID,/////
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("LM35 sensor");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *LMService = pServer->createService(SERVICE_UUID);
  LMService->addCharacteristic(&LMTempCHARACTERISTIC);
  LMTempsDescriptor.setValue("LM temperature");
  LMTempCHARACTERISTIC.addDescriptor(&LMTempsDescriptor);

  //pCharacteristic->setValue("Hello World says Neil");
  LMService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
      temp = analogRead(PIN_LM35);
      temp = temp*500/1023;
      static char temperatureCTemp[10];
      dtostrf(temp, 4,  2, temperatureCTemp);
      //Set temperature Characteristic value and notify connected client
      LMTempCHARACTERISTIC.setValue(temperatureCTemp);
      LMTempCHARACTERISTIC.notify();
      Serial.println(temperatureCTemp);
      delay(3000);

}