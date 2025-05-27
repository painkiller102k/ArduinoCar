// Пины для ультразвукового датчика
const int trigPin = 3;
const int echoPin = 2;
int dist_check1, dist_check2, dist_check3;
long duration, distance, distance_all;
int dist_result;

// Пины для моторов
const int mot1f = 6;
const int mot1b = 5;
const int mot2f = 11;
const int mot2b = 10;
int mot_speed = 100; // Скорость моторов
int k = 0; // Тормоз

// Логические переменные
const int dist_stop = 30; // Расстояние для остановки (уменьшено)
const int max_range = 100; // Максимальное расстояние
const int min_range = 0;   // Минимальное расстояние
int errorLED = 13; // Индикатор ошибки

// Инициализация
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(errorLED, OUTPUT);
  
  // Добавляем паузу перед запуском (3 секунды)
  delay(3000); // Задержка 3 секунды
}

// Основной цикл программы
void loop() {
  int result = ping(); // Проверка расстояния

  // Если объект слишком близко, включаем LED ошибку и начинаем двигаться назад и поворачивать
  if (result <= min_range) { 
    digitalWrite(errorLED, 1); // Включаем светодиод ошибки
    delay(100); // Уменьшена задержка
  }
  
  // Если расстояние превышает максимальный предел или слишком велико, считаем ошибкой
  if (result == max_range || result > max_range) { 
    digitalWrite(errorLED, 1); 
    delay(100); // Уменьшена задержка
  }

  // Когда объект слишком близко (в пределах dist_stop), реагируем быстрее
  if (result <= dist_stop) { 
    digitalWrite(errorLED, 0); // Выключаем светодиод ошибки
    motors_back();
    delay(300); // Уменьшено время движения назад
    motors_stop();
    delay(100); // Уменьшена задержка перед поворотом
    motors_left(); // Поворот влево
    delay(200); // Уменьшено время поворота
    motors_stop();
    delay(100); // Уменьшена задержка перед продолжением движения
  } 
  // Если расстояние нормально, просто двигаемся вперёд
  else if (result > dist_stop) { 
    motors_forward();
    delay(50); // Уменьшена задержка
  }
}

// Функция для измерения расстояния
int ping() {
  // 1-й измерение
  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, 1);
  distance = duration / 58;
  dist_check1 = distance;

  // 2-й измерение
  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, 1);
  distance = duration / 58;
  dist_check2 = distance;

  // 3-й измерение
  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, 1);
  distance = duration / 58;
  dist_check3 = distance;

  // Суммируем и возвращаем среднее значение
  int dist_check_sum = dist_check1 + dist_check2 + dist_check3;
  dist_result = dist_check_sum / 3;
  return dist_result;
}

// Функции для управления моторами
void motors_forward() { // Двигаемся вперёд
  analogWrite(mot1f, mot_speed);
  analogWrite(mot2f, mot_speed);
  digitalWrite(mot1b, 0);
  digitalWrite(mot2b, 0);
}

void motors_back() { // Двигаемся назад
  digitalWrite(mot1f, 0);
  digitalWrite(mot2f, 0);
  analogWrite(mot1b, mot_speed);
  analogWrite(mot2b, mot_speed);
}

void motors_stop() { // Останавливаем моторы
  digitalWrite(mot1f, 1);
  digitalWrite(mot2f, 1);
  digitalWrite(mot1b, 1);
  digitalWrite(mot2b, 1);
}

void motors_left() { // Поворот влево
  analogWrite(mot1f, mot_speed);
  digitalWrite(mot2f, 0);
  digitalWrite(mot1b, 0);
  analogWrite(mot2b, mot_speed);
}

void motors_right() { // Поворот вправо
  digitalWrite(mot1f, 0);
  analogWrite(mot2f, mot_speed);
  analogWrite(mot1b, mot_speed);
  digitalWrite(mot2b, 0);
}

void motors_foward_left() { // Вперёд влево
  k = mot_speed * 0.8;
  analogWrite(mot1f, mot_speed);
  analogWrite(mot2f, k);
  digitalWrite(mot1b, 0);
  digitalWrite(mot2b, 0);
}

void motors_foward_right() { // Вперёд вправо
  k = mot_speed * 0.8;
  analogWrite(mot1f, k);
  analogWrite(mot2f, mot_speed);
  analogWrite(mot1b, 0);
  analogWrite(mot2b, 0);
}

void motors_back_left() { // Назад влево
  k = mot_speed * 0.8;
  digitalWrite(mot1f, 0);
  digitalWrite(mot2f, 0);
  analogWrite(mot1b, k);
  analogWrite(mot2b, mot_speed);
}

void motors_back_right() { // Назад вправо
  k = mot_speed * 0.8;
  digitalWrite(mot1f, 0);
  digitalWrite(mot2f, 0);
  analogWrite(mot1b, mot_speed);
  analogWrite(mot2b, k);
}
