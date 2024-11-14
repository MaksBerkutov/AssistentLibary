#include <BLEDevice.h>
#include <BLEScan.h>
#include <AssistentLibrary.h>

String NameBoard = "esp32";
String AesKey = "30555C24496F7F124026365948694E0F";
AssistenWiFi assistent;
const long interval = 9500;
bool deviceStatus = false;
BLEScan *pBLEScan;
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
  static unsigned long foundDevice;
  static float targetDistance;
  static int countScannig;

  const char *targetMac = "44:40:73:b5:b5:e2";

  float calculateDistance(int rssi)

  {
    float txPower = -59;
    float n = 2;
    return pow(10, (txPower - rssi) / (10 * n));
  }

  void onResult(BLEAdvertisedDevice advertisedDevice)
  {

    float distance = calculateDistance((int)advertisedDevice.getRSSI());
    Serial.printf("Устройство =  MAC-адрес: %s, RSSI: %d, Имя: %s, Дистанция: %f\n",
                  advertisedDevice.getAddress().toString().c_str(),
                  advertisedDevice.getRSSI(),
                  advertisedDevice.getName().c_str(),
                  distance);

    if (advertisedDevice.getAddress().toString() == targetMac && distance <= targetDistance)
    {
      Serial.println("Целевое устройство найдено!");
      foundDevice = millis();
    }
  };
};
float MyAdvertisedDeviceCallbacks::targetDistance = 10.8f;
int MyAdvertisedDeviceCallbacks::countScannig = 0;
unsigned long MyAdvertisedDeviceCallbacks::foundDevice = 0;

AssistentVariable GetState()
{
  String keys[] = {
      "state",
      "devices",
  };
  String values[] = {String(MyAdvertisedDeviceCallbacks::targetDistance), deviceStatus ? "found" : "not found"};
  return AssistentVariable(keys, values, 2);
}

String cmdRecName[] = {"GetState"};
HandlerCMDRec cmdRec[] = {GetState};
void bleScanTask(void *pvParameters);
TaskHandle_t bleScanTaskHandle;

void setup()
{
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(1000);
  pBLEScan->setWindow(500);
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  assistent.Begin(AesKey, NameBoard, nullptr, nullptr, 0, cmdRecName, cmdRec, 1, (char *)"DOM", (char *)"07012004");

  xTaskCreatePinnedToCore(
      bleScanTask,
      "BLE Scan Task",
      4096,
      NULL,
      1,
      &bleScanTaskHandle,
      1);
}

void bleScanTask(void *pvParameters)
{
  while (true)
  {
    pBLEScan->start(3, false);
    MyAdvertisedDeviceCallbacks::countScannig++;

    delay(100);
  }
}

void loop()
{
  bool dumpDevice = deviceStatus;
  assistent.Handle();
  unsigned long currentMillis = millis();

  if (currentMillis - MyAdvertisedDeviceCallbacks::foundDevice >= interval && deviceStatus && MyAdvertisedDeviceCallbacks::countScannig >= 3)
  {
    deviceStatus = false;
  }
  else if (currentMillis - MyAdvertisedDeviceCallbacks::foundDevice < interval && !deviceStatus)
  {
    deviceStatus = true;
  }
  else if (MyAdvertisedDeviceCallbacks::countScannig >= 3)
  {
    MyAdvertisedDeviceCallbacks::countScannig = 0;
  }

  if (dumpDevice != deviceStatus)
  {
    Serial.println("#===========<DUMP>===========#");
    Serial.printf("Интервал сканирования: %lu секунд\n", interval / 1000);
    Serial.printf("Состояние устройства: %s\n", deviceStatus ? "активно" : "неактивно");
    Serial.printf("Последнее отображение сканирования: %lu секунд\n", MyAdvertisedDeviceCallbacks::foundDevice / 1000);
    Serial.printf("Ткущее время: %lu секунд\n", currentMillis / 1000);

    String keys[] = {"status"};
    String values[] = {deviceStatus ? "true" : "false"};
    assistent.IoTMessage(AssistentVariable(keys, values, 1));
  }
}
