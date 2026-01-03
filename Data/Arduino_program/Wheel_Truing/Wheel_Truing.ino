#define DETECTOR_0 2
#define DETECTOR_1 3
#define INDUCTIVE_SENSE A3
#define LED 13

#define BUFFER_SIZE 64

bool measuring = false;
char cmdBuf[BUFFER_SIZE];
uint8_t cmdPos = 0;

bool detector0 = false;
bool detector1 = false;
uint8_t spoke_count_total = 0;
uint8_t spoke_counter = 0;
int8_t last_spoke = -1;

void handleCommand(char *cmd)
{
  // convert to upper case
  for (char *p = cmd; *p; p++) *p = toupper(*p);

  if (!strcmp(cmd, "*IDN?"))
  {
    Serial.println("Piofciex,Wheel-Truing-Stand,0001,1.0");
  }
  else if (!strcmp(cmd, "MEAS:START"))
  {
    measuring = true;
  }
  else if (!strcmp(cmd, "MEAS:DATA?"))
  {
    Serial.print(analogRead(INDUCTIVE_SENSE));
    Serial.print(';');
    Serial.print(spoke_counter);
    Serial.print('\n');
  }
  else if (!strcmp(cmd, "MEAS:STOP"))
  {
    measuring = false;
  }
  else if (!strcmp(cmd, "SPOKES:ZERO"))
  {
    noInterrupts();
    spoke_counter = 0;
    detector0 = false;
    detector1 = false;
    interrupts();
    Serial.println("OK");
  }
  else if (!strcmp(cmd, "SPOKES?"))
  {
    Serial.println(spoke_count_total);
  }
  else if (!strncmp(cmd, "SPOKES ", 7))
  {
    int value = atoi(cmd + 7);
    if (value > 0 && value < 128)   // simple sanity check
    {
      spoke_count_total = value;
      Serial.println("OK");
    }
    else
    {
      Serial.println("Error, numer too large");
    }
  }
  else
  {
    Serial.print("Error (");
    Serial.print(cmd);
    Serial.println(");");
  }
}


void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  //Serial.println("OK");
  pinMode(DETECTOR_0, INPUT);
  pinMode(DETECTOR_1, INPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(DETECTOR_0), detectorIRQ0, FALLING);
  attachInterrupt(digitalPinToInterrupt(DETECTOR_1), detectorIRQ1, FALLING);

  digitalWrite(LED, LOW);
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      cmdBuf[cmdPos] = 0;
      handleCommand(cmdBuf);
      cmdPos = 0;
    }
    else if (c != '\r' && cmdPos < sizeof(cmdBuf) - 1) {
      cmdBuf[cmdPos++] = c;
    }
  }

  if ((measuring) && (last_spoke != spoke_counter))
  {
      Serial.print(analogRead(INDUCTIVE_SENSE));
      Serial.print(';');
      Serial.print(spoke_counter);
      Serial.print('\n');
      last_spoke = spoke_counter;
  }
  //delay(1);
}

void detectorIRQ0()
{
  detector0 = !detector0;
  if(detector0 && detector1)
  {
    spoke_counter--;
    spoke_counter = spoke_counter % spoke_count_total;
    detector0 = false;
    detector1 = false;
  }
}

void detectorIRQ1()
{
  detector1 = !detector1;
  if(detector0 && detector1)
  {
    spoke_counter++;
    spoke_counter = spoke_counter % spoke_count_total;
    detector0 = false;
    detector1 = false;
  }
}
