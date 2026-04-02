# AssistentLibrary

Библиотека для ESP8266/ESP32, которая связывает прошивку устройства с [Assistent Web Server](https://github.com/MaksBerkutov/PhpAssistent).

Сейчас у библиотеки есть два уровня API:

- `AssistentEasy` — максимально простой слой для обычных скетчей
- `AssistentLibrary` — низкоуровневый API, если нужен полный контроль над командами и обработчиками

Подробный список последних изменений лежит в [CHANGELOG.md](./CHANGELOG.md).

## Что выбрать

Используйте `AssistentEasy`, если хотите писать короткий скетч без массивов команд, ручного `new` и упаковки данных.

Используйте `AssistentLibrary`, если вам нужен старый стиль работы через `CMD`, `HandlerCMD`, `CMDRec`, `HandlerCMDRec` и прямой доступ к `AssisenWiFi`.

## Возможности

- Wi-Fi для ESP8266 и ESP32
- Прием команд от сервера
- Команды с ответом и без ответа
- Отправка телеметрии на сервер
- AES-шифрование сообщений
- OTA-обновление через макрос `ASSISTENT_OTA`
- Упрощенный фасад `AssistentEasy` для коротких скетчей

## Зависимости

В `PlatformIO` используются такие зависимости:

- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [AESLib](https://github.com/DavyLandman/AESLib)
- [base64_arduino](https://github.com/Densaugeo/base64_arduino)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

Актуальный пример конфигурации лежит в [platformio.ini](/C:/OSPanel/home/AssistentLibary/AssistentLibary/platformio.ini).

## Макросы

```cpp
#define ASSISTENT_DEBUG
// #define ASSISTENT_OTA
```

`ASSISTENT_DEBUG` включает сообщения в `Serial`.

`ASSISTENT_OTA` включает endpoint для OTA-обновлений.

## Быстрый старт

### Реле

Это самый короткий вариант скетча для управления реле:

```cpp
#include <AssistentEasy.h>

AssistentRelayDevice device(
    "RoomLight",
    "00112233445566778899AABBCCDDEEFF",
    "SSID",
    "PASSWORD");

void setup()
{
  device.addRelay("led", 2);
  device.addRelay("pump", 5);
  device.begin();
}

void loop()
{
  device.run();
}
```

Готовый файл: [EasyRelay.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/examples/Easy/EasyRelay.cpp).

### Климат

Пример с DHT22:

```cpp
#include <AssistentEasy.h>

AssistentClimateDevice device(
    "ClimateNode",
    "00112233445566778899AABBCCDDEEFF",
    "SSID",
    "PASSWORD");

void setup()
{
  device.addDht22("room", 4);
  device.begin();
}

void loop()
{
  device.run();
}
```

Готовый файл: [EasyClimate.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/examples/Easy/EasyClimate.cpp).

## AssistentEasy API

Подключение:

```cpp
#include <AssistentEasy.h>
```

Основные классы:

- `AssistentRelayDevice`
- `AssistentClimateDevice`
- `AssistentEasyDevice`

### Конструктор

```cpp
AssistentRelayDevice device(name, aesKey, ssid, password, baudRate);
```

Параметры:

- `name` — имя устройства
- `aesKey` — AES-ключ в hex-формате, 16 байт
- `ssid` — имя Wi-Fi сети
- `password` — пароль Wi-Fi
- `baudRate` — скорость `Serial`, по умолчанию `9600`

### Методы AssistentEasyDevice

- `begin()` — инициализация Wi-Fi, сервера и внутренних обработчиков
- `run()` — вызывать в `loop()`
- `sendState()` — отправить текущее состояние на сервер
- `set(key, value)` — записать строковое значение в состояние устройства
- `values()` — получить ссылку на внутренний `AssistentVariable::Variable`
- `on(command, handler)` — зарегистрировать команду без ответа
- `onRequest(command, handler)` — зарегистрировать команду с ответом
- `relay(name)` — получить объект-обертку для управления реле по имени

### Методы AssistentRelayDevice

- `addRelay(name, pin)` — добавить реле

Автоматически регистрируются команды:

- `ON`
- `OFF`
- `TOGGLE`
- `STATE`

Примеры:

```cpp
device.addRelay("fan", 14);
device.set("mode", "auto");
device.sendState();
```

```cpp
device.relay("fan").on();
device.relay("fan").toggle();
```

### Методы AssistentClimateDevice

- `addDht22(name, pin)` — добавить датчик DHT22

Автоматически регистрируется команда:

- `STATE`

### Кастомные команды в AssistentEasy

Если нужен простой пользовательский хендлер без перехода на старый API:

```cpp
#include <AssistentEasy.h>

AssistentRelayDevice device(
    "Node",
    "00112233445566778899AABBCCDDEEFF",
    "SSID",
    "PASSWORD");

void Ping(String arg)
{
  device.set("last_ping", arg);
}

AssistentVariable::Variable ReadState(String arg)
{
  return device.values();
}

void setup()
{
  device.addRelay("led", 2);
  device.on("PING", Ping);
  device.onRequest("GET_ALL", ReadState);
  device.begin();
}

void loop()
{
  device.run();
}
```

### Ограничения AssistentEasy

- Один `AssistentEasy`-девайс на один скетч
- Максимум `12` команд без ответа
- Максимум `12` команд с ответом
- Максимум `12` зарегистрированных реле

Эти ограничения заданы в [AssistentEasy.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentEasy.h).

## Низкоуровневый API

Если нужен старый стиль, используйте:

```cpp
#include <AssistentLibrary.h>
```

Основная точка входа — класс `AssisenWiFi`.

Сигнатуры обработчиков:

- Команды без ответа: `void Handler(String Arg)`
- Команды с ответом: `AssistentVariable::Variable Handler(String Arg)`

Минимальный пример:

```cpp
#include <AssistentLibrary.h>
#include <Types/DataRele.h>

String CMD[] = {"ON", "OFF"};
String CMDRec[] = {"GetState"};

AssisenWiFi assistant;
AssistentVariable::Variable variable;

void ON(String arg)
{
  auto rele = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataRele>(variable[arg]);
  if (rele != nullptr)
    rele->ON();
}

void OFF(String arg)
{
  auto rele = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataRele>(variable[arg]);
  if (rele != nullptr)
    rele->OFF();
}

AssistentVariable::Variable GetState(String arg)
{
  return variable;
}

HandlerCMD HCmd[] = {ON, OFF};
HandlerCMDRec HCmdRec[] = {GetState};

void setup()
{
  assistant.Begin(
      "00112233445566778899AABBCCDDEEFF",
      "DeviceName",
      CMD,
      HCmd,
      2,
      CMDRec,
      HCmdRec,
      1,
      "your_wifi_ssid",
      "your_wifi_password");

  variable["led"] = new AssistentVariable::Types::DataRele(2);
}

void loop()
{
  assistant.Handle();
}
```

Файл API: [AssistentLibrary.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentLibrary.h).

### Параметры Begin

```cpp
Begin(
  aesKey,
  name,
  CMD,
  HandlerCMDS,
  sizeCMD,
  CMDRec,
  HandlerCMDSRec,
  sizeCMDRec,
  ssid,
  password,
  cfg,
  baudRate,
  handler)
```

Параметры:

- `aesKey` — AES-ключ
- `name` — имя устройства
- `CMD` — массив команд без ответа
- `HandlerCMDS` — массив обработчиков команд без ответа
- `sizeCMD` — размер массива `CMD`
- `CMDRec` — массив команд с ответом
- `HandlerCMDSRec` — массив обработчиков команд с ответом
- `sizeCMDRec` — размер массива `CMDRec`
- `ssid` — Wi-Fi SSID
- `password` — Wi-Fi пароль
- `cfg` — объект конфигурации, опционально
- `baudRate` — скорость `Serial`
- `handler` — кастомный обработчик входящих сообщений

## Встроенные команды протокола

- `SERV_GAI` — получить информацию об устройстве и доступных командах
- `SERV_GCFG` — получить конфигурацию
- `SERV_SCFG` — обновить конфигурацию

## Что изменилось

Кратко:

- появился новый фасад `AssistentEasy`
- добавлены готовые easy-примеры
- исправлены баги в контейнерах и маршрутизации команд
- улучшена переносимость `PlatformIO`
- переписаны тесты и документация

Полный список изменений смотрите в [CHANGELOG.md](./CHANGELOG.md).

## Тестирование

Запуск тестов:

```bash
pio test
```

## Контакты

**Maks Berkutov**

- [Telegram](https://t.me/i37Holodok73i)
- [GitHub](https://github.com/MaksBerkutov)
- [Email](mailto:0701200maks@gmail.com)

## Лицензия

MIT
