#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "driver/uart.h"   // ESP-IDF UART APIs (available in Arduino)  [3](https://github.com/espressif/arduino-esp32/issues/6185)

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr int SCREEN_ADDRESS = 0x3C;

constexpr int OLED_SDA = 21;
constexpr int OLED_SCL = 22;
constexpr int TRIG_PIN = 26;
constexpr int ECHO_PIN = 25;

/* UART settings */
constexpr uart_port_t UARTX = UART_NUM_2;
// pick pins according to your board layout (any GPIO via matrix)  [4](https://esp32.com/viewtopic.php?t=28318)
constexpr int TX_PIN  = 17;
constexpr int RX_PIN  = 16;
constexpr int RTS_PIN = 4;   // ESP32 RTS output to converter CTS (7th pin from bottom-right)
constexpr int CTS_PIN = 2;   // ESP32 CTS input from converter RTS (5th pin from bottom-right)
constexpr int BAUD    = 9600;

#define SERVICE_UUID "a05fde7e-bacb-40b9-9856-efb85cdb8f66"
#define CHAR_TX_NF_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f07"
#define CHAR_RX_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f08"


BLEServer *pServer = nullptr;
BLEService *pService = nullptr;
BLECharacteristic *pTXNFCharacteristic = nullptr;
BLECharacteristic *pRXCharacteristic = nullptr;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float duration_us, distance_cm;


class RxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    // Data from BLE central -> forward to UART

    String s = c->getValue();
    Serial.println("Received data: "+s);

    if (!s.isEmpty()) {
      // Write in chunks to respect CTS if needed; driver handles pause on CTS deassert
      uart_write_bytes(UARTX, s.c_str(), s.length());
      uint8_t buf[256];
      size_t avail = 0;
      uart_get_buffered_data_len(UARTX, &avail);
      if (avail > 0) {
        int n = uart_read_bytes(UARTX, buf, std::min(avail, sizeof(buf)), 10);
        if (n > 0) {
          // Chunk to negotiated MTU to avoid oversize notifications (default payload ~20B)
          pTXNFCharacteristic->setValue(buf, n);
          pTXNFCharacteristic->notify();
        }
      }
    }
  }
};

void setupUart() {
  // Install driver with RX/TX buffers
  const int BUF = 2048;
  uart_driver_install(UARTX, BUF, BUF, 0, nullptr, 0);

  // Configure parameters
  uart_config_t cfg = {
    .baud_rate = BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
    .rx_flow_ctrl_thresh = 0,
    .source_clk = UART_SCLK_DEFAULT
  };
  uart_param_config(UARTX, &cfg);

  // Route pins: TX, RX, RTS, CTS  [2](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/uart.html)
  uart_set_pin(UARTX, TX_PIN, RX_PIN, RTS_PIN, CTS_PIN);          // enable pins  [3](https://github.com/espressif/arduino-esp32/issues/6185)
  uart_set_hw_flow_ctrl(UARTX, UART_HW_FLOWCTRL_CTS_RTS, 64);     // CTS+RTS, RX threshold  [3](https://github.com/espressif/arduino-esp32/issues/6185)
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA,OLED_SCL);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setupUart();
  
  BLEDevice::init("ESP32_BLE_UART");
  BLEDevice::setMTU(512);
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  
  pTXNFCharacteristic = pService->createCharacteristic(
    CHAR_TX_NF_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY /*| BLECharacteristic::PROPERTY_WRITE*/
  );
  //pTXNFCharacteristic->setCallbacks(new RxCallbacks());
  pTXNFCharacteristic->addDescriptor(new BLE2902());
  
  pRXCharacteristic = pService->createCharacteristic(
    CHAR_RX_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRXCharacteristic->addDescriptor(new BLE2902());
  // Rx handler: BLE → UART
  pRXCharacteristic->setCallbacks(new RxCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  display.begin(0x3C, true);
  //display.begin(SH1106_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
}

void loop() {

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  String distanceString = String(distance_cm);

  pTXNFCharacteristic->setValue(distanceString.c_str());
  pTXNFCharacteristic->notify();

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("Distance: ");
  display.print(distance_cm);
  display.println(" cm");
  display.display();

  delay(100);
  display.clearDisplay();

  sleep(5);

}
