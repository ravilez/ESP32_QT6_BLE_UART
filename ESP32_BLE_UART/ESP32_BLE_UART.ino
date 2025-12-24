#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define ECHO_TASK_STACK_SIZE 4096  // 4096 words (~16 KB on ESP32)

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
constexpr int RTS_PIN = 4;   // ESP32 RTS output to converter CTS (5th pin from bottom-right)
constexpr int CTS_PIN = 2;   // ESP32 CTS input from converter RTS (4th pin from bottom-right)
constexpr int BAUD    = 9600;



#define SERVICE_UUID "a05fde7e-bacb-40b9-9856-efb85cdb8f66"
#define CHAR_TX_NF_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f07"
#define CHAR_RX_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f08"
#define FS_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f09"

BLEServer *pServer = nullptr;
BLEService *pService = nullptr;
BLECharacteristic *pTXNFCharacteristic = nullptr;
BLECharacteristic *pRXCharacteristic = nullptr;
BLECharacteristic *pFSCharacteristic = nullptr;

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
    }
  }
};

class FSReadCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    // Data from BLE central -> sdcard

    String s = c->getValue();
    Serial.println("Received data: "+s);

    if (!s.isEmpty()) {
    }
  }
};

static void echo_task(void *arg) {

  // Install driver with RX/TX buffers
  const int BUF_SIZE = 2048;


  // Configure parameters
  uart_config_t uart_config = {
    .baud_rate = BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
    .rx_flow_ctrl_thresh = 0,
    .source_clk = UART_SCLK_DEFAULT
  };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(UARTX, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UARTX, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UARTX, TX_PIN, RX_PIN, RTS_PIN, CTS_PIN));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UARTX, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        // Write data back to the UART
        //uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
        if (len) {
            data[len] = '\0';
            Serial.print("Reading:");
            Serial.println((char *) data);
            // Chunk to negotiated MTU to avoid oversize notifications (default payload ~20B)
            pTXNFCharacteristic->setValue(data, len);
            pTXNFCharacteristic->notify();

        }
    }

}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA,OLED_SCL);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  BLEDevice::init("ESP32_BLE_UART");
  BLEDevice::setMTU(512);
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  
  pTXNFCharacteristic = pService->createCharacteristic(
    CHAR_TX_NF_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY /*| BLECharacteristic::PROPERTY_WRITE*/
  );
  pTXNFCharacteristic->addDescriptor(new BLE2902());
  
  pRXCharacteristic = pService->createCharacteristic(
    CHAR_RX_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRXCharacteristic->addDescriptor(new BLE2902());
  // Rx handler: BLE → UART
  pRXCharacteristic->setCallbacks(new RxCallbacks());

  pFSCharacteristic = pService->createCharacteristic(
    FS_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
  );
  pFSCharacteristic->addDescriptor(new BLE2902());
  // FS READ handler: BLE → ESP32 sdcard
  pFSCharacteristic->setCallbacks(new FSReadCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
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


  xTaskCreate(echo_task, "uart_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);

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
  pFSCharacteristic->setValue("Not SD-Card detected");
  pFSCharacteristic->notify();

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
