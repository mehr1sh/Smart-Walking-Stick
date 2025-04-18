// What to Expect:
//Open Serial Monitor at 115200 baud

//Type AT and hit Enter

//You should get OK from the module

#define MODEM_RST 5
#define MODEM_PWRKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 17
#define MODEM_RX 16
#define SerialMon Serial
#define SerialAT Serial2

void setup() {
  SerialMon.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);

  SerialMon.println("Starting SIM800L test...");

  delay(3000); // wait for SIM800L to initialize

  SerialAT.println("AT");
}

void loop() {
  if (SerialAT.available()) {
    SerialMon.write(SerialAT.read());
  }
  if (SerialMon.available()) {
    SerialAT.write(SerialMon.read());
  }
}
