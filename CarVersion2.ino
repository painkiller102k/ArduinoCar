#include <Servo.h>

// Пины ультразвукового датчика
const int trigPin = 3;
const int echoPin = 2;

// Пины моторов
const int mot1f = 6;
const int mot1b = 5;
const int mot2f = 11;
const int mot2b = 10;

int mot_speed = 100; // Скорость моторов
int k = 0;

// Пороговые значения
const int dist_stop = 30; // Расстояние остановки
int errorLED = 13;        // Светодиод ошибки

// Серво
Servo myServo;
const int servoPin = 12;

// Переменные расстояния
long duration, distance;
int dist_check1, dist_check2, dist_check3;
int dist_result;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(errorLED, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(90); // Стартовое положение — прямо
  delay(3000); // Время на запуск
}

void loop() {
  int result = ping(); // Проверка спереди

  if (result <= dist_stop) {
    digitalWrite(errorLED, 1);
    motors_stop();

    // Проверка слева (0°)
    int left_dist = check_side(0);
    delay(200);

    // Проверка справа (180°)
    int right_dist = check_side(180);
    delay(200);

    // Возвращаем серво в центр
    myServo.write(90);
    delay(300);

    if (left_dist > dist_stop) {
      motors_left();
      delay(300);
      motors_forward();
      delay(500);
    } else if (right_dist > dist_stop) {
      motors_right();
      delay(300);
      motors_forward();
      delay(500);
    } else {
      motors_back();
      delay(500);
      motors_stop();
      delay(200);
    }
  } else {
    digitalWrite(errorLED, 0);
    motors_forward();
    delay(50);
  }
}

// Функция для измерения расстояния (усреднённое значение)
int ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist_check1 = duration / 58;

  delay(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist_check2 = duration / 58;

  delay(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist_check3 = duration / 58;

  dist_result = (dist_check1 + dist_check2 + dist_check3) / 3;
  return dist_result;
}

// Проверка стороны с серво
int check_side(int angle) {
  myServo.write(angle);
  delay(500); // Ждём стабилизацию
  return ping();
}

// Управление моторами
void motors_forward() {
  analogWrite(mot1f, mot_speed);
  analogWrite(mot2f, mot_speed);
  digitalWrite(mot1b, LOW);
  digitalWrite(mot2b, LOW);
}

void motors_back() {
  digitalWrite(mot1f, LOW);
  digitalWrite(mot2f, LOW);
  analogWrite(mot1b, mot_speed);
  analogWrite(mot2b, mot_speed);
}

void motors_stop() {
  digitalWrite(mot1f, HIGH);
  digitalWrite(mot2f, HIGH);
  digitalWrite(mot1b, HIGH);
  digitalWrite(mot2b, HIGH);
}

void motors_left() {
  analogWrite(mot1f, mot_speed);
  digitalWrite(mot2f, LOW);
  digitalWrite(mot1b, LOW);
  analogWrite(mot2b, mot_speed);
}

void motors_right() {
  digitalWrite(mot1f, LOW);
  analogWrite(mot2f, mot_speed);
  analogWrite(mot1b, mot_speed);
  digitalWrite(mot2b, LOW);
}

void motors_foward_left() {
  k = mot_speed * 0.8;
  analogWrite(mot1f, mot_speed);
  analogWrite(mot2f, k);
  digitalWrite(mot1b, LOW);
  digitalWrite(mot2b, LOW);
}

void motors_foward_right() {
  k = mot_speed * 0.8;
  analogWrite(mot1f, k);
  analogWrite(mot2f, mot_speed);
  digitalWrite(mot1b, LOW);
  digitalWrite(mot2b, LOW);
}

void motors_back_left() {
  k = mot_speed * 0.8;
  digitalWrite(mot1f, LOW);
  digitalWrite(mot2f, LOW);
  analogWrite(mot1b, k);
  analogWrite(mot2b, mot_speed);
}

void motors_back_right() {
  k = mot_speed * 0.8;
  digitalWrite(mot1f, LOW);
  digitalWrite(mot2f, LOW);
  analogWrite(mot1b, mot_speed);
  analogWrite(mot2b, k);
}
