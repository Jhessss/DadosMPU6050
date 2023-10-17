#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize(); // Inicializa a MPU-6050

  // Configuração do acelerômetro e do giroscópio
  mpu.setFullScaleGyroRange(0);  // ±250 °/s
  mpu.setFullScaleAccelRange(0); // ±2g

  // Inicia a comunicação serial
  Serial.println("MPU-6050 Inicializado!");
}

void loop() {
  // Lê os dados brutos do acelerômetro e do giroscópio
  int16_t acX, acY, acZ, temp, gyroX, gyroY, gyroZ;
  mpu.getMotion6(&acX, &acY, &acZ, &gyroX, &gyroY, &gyroZ);

  // Exibe os valores brutos no monitor serial
  Serial.print("Acelerômetro (raw): ");
  Serial.print(acX);
  Serial.print(" , ");
  Serial.print(acY);
  Serial.print(" , ");
  Serial.print(acZ);
  Serial.println();

  Serial.print("Giroscópio (raw): ");
  Serial.print(gyroX);
  Serial.print(" , ");
  Serial.print(gyroY);
  Serial.print(" , ");
  Serial.print(gyroZ);
  Serial.println();

  delay(1000); // Aguarda um segundo antes de ler novamente
}
