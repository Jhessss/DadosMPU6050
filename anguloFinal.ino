#include<Wire.h>
#include<Math.h>

// Endereco I2C do sensor MPU-6050
const int MPU = 0x68;
int cont = 0;

// Variaveis para armazenar valores do sensor
double AccX, AccY, AccZ, AccXf, AccYf, AccZf, theta_pitch;
double GyroX, GyroY, GyroZ, GyroXf, GyroYf, GyroZf, theta_pitch_gyro;

// Variáveis para integração do giroscópio
unsigned long intervalo = 0; // Inicialize a variável intervalo
double angulo_anterior = 0;

void setup() {
  // Inicializa Serial
  Serial.begin(9600);

  // Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configura Acelerometro para fundo de escala desejado
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();
}

void loop() {
  // Comandos para iniciar transmissão de dados
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // Solicita os dados ao sensor

  // Armazena o valor dos sensores nas variaveis correspondentes
  AccX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  GyroX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyroY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyroZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  AccXf = ((AccX / 16384) - 0.01645);
  AccYf = ((AccY / 16384) + 0.0011);
  AccZf = ((AccZ / 16384) + 0.04805);

  GyroXf = GyroX / 131.0; // Sensibilidade do giroscópio para o range de ±250 degrees/sec
  GyroYf = GyroY / 131.0;
  GyroZf = GyroZ / 131.0;

  // Cálculo do ângulo utilizando o acelerômetro
  theta_pitch = atan((AccYf / sqrt(square(AccXf) + square(AccZf)))) * 180.0 / M_PI;

  // Integração do giroscópio para obter o ângulo
  unsigned long agora = millis();
  double intervalo_segundos = (agora - intervalo) / 1000.0;
  theta_pitch_gyro += GyroYf * intervalo_segundos;
  intervalo = agora;

  if (cont < 100) {
    // Imprime na Serial os valores obtidos
    Serial.print("Acelerômetro - ANGULO: ");
    Serial.print(theta_pitch);

    Serial.print(" Giroscópio - ANGULO: ");
    Serial.println(theta_pitch_gyro);

    cont++;
  }

  // Atraso de 100ms
  delay(100);
}
