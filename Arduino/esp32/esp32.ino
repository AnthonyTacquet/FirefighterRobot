#define WATCHDOG
#define RX1 16
#define TX1 17

void port_init()
{
  Serial1.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX1, TX1);
}

void setup() 
{
  // put your setup code here, to run once:
  port_init();
}

void loop() 
{
  Serial.print("Data Recieved: ");
  Serial.println(Serial2.readString());

  delay(200);
}
