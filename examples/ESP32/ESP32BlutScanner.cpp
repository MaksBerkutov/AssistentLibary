#include <BLEDevice.h>
#include <BLEScan.h>
#include <AssistentLibrary.h>
#include <Types/DataVariable.h>

String NameBoard = "PlatName";
String AesKey = "AppKey";
String TargetMac = "00:00:00:00:00:00";
const char *SSID = "SSID";
const char *PASSWORD = "PASSWORD";
AssistenWiFi assistent;
AssistentVariable::Variable variable;
const long interval = 9500;
bool deviceStatus = false;
BLEScan *pBLEScan;
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
  static unsigned long foundDevice;
  static float targetDistance;
  static int countScannig;

  float calculateDistance(int rssi)

  {
    float txPower = -59;
    float n = 2;
    return pow(10, (txPower - rssi) / (10 * n));
  }

  void onResult(BLEAdvertisedDevice advertisedDevice)
  {

    float distance = calculateDistance((int)advertisedDevice.getRSSI());

    if (advertisedDevice.getAddress().toString() == TargetMac.c_str() && distance <= targetDistance)
    {
      foundDevice = millis();
    }
  };
};
float MyAdvertisedDeviceCallbacks::targetDistance = 10.8f;
int MyAdvertisedDeviceCallbacks::countScannig = 0;
unsigned long MyAdvertisedDeviceCallbacks::foundDevice = 0;

AssistentVariable::Variable GetState(String arg)
{
  return variable;
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
  assistent.Begin(AesKey, NameBoard, nullptr, nullptr, 0, cmdRecName, cmdRec, 1, SSID, PASSWORD);
  variable["status"] = new AssistentVariable::Types::DataVariable("false");
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
    auto text = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataVariable>(variable["status"]);
    *(text) = deviceStatus ? "Found" : "Not Found";
    assistent.IoTMessage(variable);
  }
}
