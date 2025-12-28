#define DETECTOR_0 8
#define DETECTOR_1 9
#define INDUCTIVE_SENSE A3
#define LED 13

bool measuring = false;
String inputLine = "";

void setup()
{
  Serial.begin(115200);
  pinMode(DETECTOR_0, INPUT);
  pinMode(DETECTOR_1, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void handleCommand(String cmd)
{
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "*IDN?")
  {
    Serial.println("Piofciex,Wheel-Truing-Stand,0001,1.0");
  }
  else if (cmd == "MEAS:START")
  {
    digitalWrite(LED, HIGH);
    measuring = true;
  }
  else if (cmd == "MEAS:STOP")
  {
    digitalWrite(LED, LOW);
    measuring = false;
  }
  else
  {
    // Optional: SCPI-style error
    Serial.println("ERROR:UNKNOWN COMMAND");
  }
}

void loop()
{
  /* ---- SCPI command reception ---- */
  while (Serial.available())
  {
    char c = Serial.read();

    if (c == '\n')
    {
      handleCommand(inputLine);
      inputLine = "";
    }
    else if (c != '\r')
    {
      inputLine += c;
    }
  }

  /* ---- Measurement output ---- */
  if (measuring)
  {
    Serial.print(analogRead(INDUCTIVE_SENSE));
    Serial.print(';');
    Serial.print(digitalRead(DETECTOR_0));
    Serial.print(';');
    Serial.print(digitalRead(DETECTOR_1));
    Serial.print('\n');
    delay(1);
  }
}
