#include <Wire.h>
#include <Math.h>

// Endereço I2C do sensor MPU-6050
const int MPU = 0x68;  // Endereço I2C do MPU-6050

int cont = 0;

// Variáveis para armazenar valores do sensor
double AccX, AccY, AccZ, AccXf, AccYf, AccZf, theta_pitch;
double GyroX, GyroY, GyroZ, GyroXf, GyroYf, GyroZf, theta_pitch_gyro;

// Variáveis para integração do giroscópio
unsigned long intervalo = 0; // Inicialize a variável intervalo
double angulo_anterior = 0;

void setup() {
  // Inicializa a comunicação Serial
  Serial.begin(9600);

  // Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // Registrador PWR_MGMT_1
  Wire.write(0);     // Coloca 0 para ativar o MPU-6050 (desabilita o modo de economia de energia)
  Wire.endTransmission(true);

  // Configura o acelerômetro para fundo de escala desejado
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);  
  Wire.write(0b00000000);  // Configuração padrão para fundo de escala ±2g
  Wire.endTransmission();
}

void loop() {
  // Comandos para iniciar a transmissão de dados
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // Solicita os dados ao sensor

  // Armazena o valor dos sensores nas variáveis correspondentes
  AccX = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do acelerômetro em X
  AccY = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do acelerômetro em Y
  AccZ = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do acelerômetro em Z

  GyroX = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do giroscópio em X
  GyroY = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do giroscópio em Y
  GyroZ = Wire.read() << 8 | Wire.read();  // Combina os bytes de alta e baixa ordem para obter o valor do giroscópio em Z

  AccXf = ((AccX / 16384) - 0.01645);  // Converte o valor do acelerômetro em X para a unidade desejada
  AccYf = ((AccY / 16384) + 0.0011);   // Converte o valor do acelerômetro em Y para a unidade desejada
  AccZf = ((AccZ / 16384) + 0.04805);  // Converte o valor do acelerômetro em Z para a unidade desejada

  GyroXf = GyroX / 131.0;  // Converte o valor do giroscópio em X para a unidade desejada (±250 degrees/sec)
  GyroYf = GyroY / 131.0;  // Converte o valor do giroscópio em Y para a unidade desejada (±250 degrees/sec)
  GyroZf = GyroZ / 131.0;  // Converte o valor do giroscópio em Z para a unidade desejada (±250 degrees/sec)

  // Cálculo do ângulo utilizando o acelerômetro
  theta_pitch = atan((AccYf / sqrt(square(AccXf) + square(AccZf)))) * 180.0 / M_PI;

  // Integração do giroscópio para obter o ângulo
  unsigned long agora = millis();  // Obtém o tempo atual em milissegundos
  double intervalo_segundos = (agora - intervalo) / 1000.0;  // Calcula o intervalo de tempo em segundos
  theta_pitch_gyro += GyroYf * intervalo_segundos;  // Atualiza o ângulo integrando o valor do giroscópio
  intervalo = agora;  // Atualiza o tempo de referência

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
