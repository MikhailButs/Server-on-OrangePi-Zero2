#define MINVOLTAGE 0 //4.5V
#define MAXVOLTAGE 1024 //5.5V
#define CHECKTIME 120000
#define VOLTAGEPIN 3
#define RELEYPIN 4
#define OFFCONTROLPIN 2
#define OFFSIGNALPIN 1
#define BTNPIN 0

bool board_enabled = false;
bool pwr_enabled = false;
unsigned short voltage = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(VOLTAGEPIN, INPUT);
  pinMode(RELEYPIN, OUTPUT);
  digitalWrite(RELEYPIN, HIGH);
  pinMode(OFFCONTROLPIN, INPUT);
  pinMode(OFFSIGNALPIN, OUTPUT);
  digitalWrite(OFFSIGNALPIN, LOW); //команда выключения платы не дается
  pinMode(BTNPIN, INPUT);
  digitalWrite(BTNPIN, HIGH); //подтягиваюзий резистор кнопки

  while (voltage < MINVOLTAGE || voltage > MAXVOLTAGE) { //контроль входного напряжения
    delay(500);
    voltage = analogRead(VOLTAGEPIN);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(BTNPIN) == LOW) { //нажали и откустили кнопку
    while (analogRead(BTNPIN) == LOW) {
      delay(100);
    }
    if (pwr_enabled == false) { //полностью включаемся
      digitalWrite(RELEYPIN, LOW);
      pwr_enabled = true;
      while (OFFCONTROLPIN == LOW) {
        delay(100);
      }
      board_enabled = true;
    }
    else { //полностью выключаемся
      digitalWrite(OFFSIGNALPIN, HIGH);
      while (digitalRead(OFFCONTROLPIN) == HIGH) {
        delay(100);
      }
      board_enabled = false;
      digitalWrite(OFFSIGNALPIN, LOW);
      digitalWrite(RELEYPIN, HIGH);
      pwr_enabled = false;
    }
  }

  board_enabled = bool(digitalRead(OFFCONTROLPIN)); //проверяем плату

  if (pwr_enabled == true && board_enabled == false) { //если выключилась плата, ждем, перепроверяем и выключаем питание
    delay(CHECKTIME);
    if (digitalRead(OFFCONTROLPIN) == LOW) {
      digitalWrite(RELEYPIN, HIGH);
      pwr_enabled = false;
    }
  }
}
