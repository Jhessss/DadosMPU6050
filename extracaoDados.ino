#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize(); // Inicializa a MPU-6050

  // Configuração do acelerômetro e do giroscópio
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

  // Inicia a comunicação serial
  Serial.println("MPU-6050 Inicializado!");
}

void loop() {
  // Lê os dados brutos do acelerômetro e do giroscópio
  int16_t acX, acY, acZ, temp, gyroX, gyroY, gyroZ;
  mpu.getMotion6(&acX, &acY, &acZ, &gyroX, &gyroY, &gyroZ);

  // Converte os dados em valores em fração de gravidade (g)
  float acX_g = (float)acX / 16384.0;
  float acY_g = (float)acY / 16384.0;
  float acZ_g = (float)acZ / 16384.0;

  // Converte os dados em valores em graus por segundo (°/s)
  float gyroX_deg_per_s = (float)gyroX / 131.0;
  float gyroY_deg_per_s = (float)gyroY / 131.0;
  float gyroZ_deg_per_s = (float)gyroZ / 131.0;

  // Exibe os valores no monitor serial
  Serial.print("Acelerômetro (g): ");
  Serial.print(acX_g);
  Serial.print(" , ");
  Serial.print(acY_g);
  Serial.print(" , ");
  Serial.println(acZ_g);

  Serial.print("Giroscópio (°/s): ");
  Serial.print(gyroX_deg_per_s);
  Serial.print(" , ");
  Serial.print(gyroY_deg_per_s);
  Serial.print(" , ");
  Serial.println(gyroZ_deg_per_s);

  delay(1000); // Aguarda um segundo antes de ler novamente
}
