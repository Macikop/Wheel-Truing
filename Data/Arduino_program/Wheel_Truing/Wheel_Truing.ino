#define DETECTOR_0 8
#define DETECTOR_1 9
#define INDUCTIVE_SENSE A3

void setup() 
{
  Serial.begin(115200);
  pinMode(DETECTOR_0, INPUT);
  pinMode(DETECTOR_1, INPUT);
}

void loop() 
{
  Serial.print(analogRead(INDUCTIVE_SENSE));
  Serial.print(';');
  Serial.print(digitalRead(DETECTOR_0));
  Serial.print(';');
  Serial.print(digitalRead(DETECTOR_1));
  Serial.print('\n');

}
