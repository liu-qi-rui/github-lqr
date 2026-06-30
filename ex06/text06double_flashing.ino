// 定义两个LED连接的引脚
// 请根据实际面包板连接修改引脚号，确保是支持PWM的GPIO
const int ledPinA = 2; 
const int ledPinB = 4; 

// PWM 属性设置
const int freq = 5000;      // 频率 5kHz，人眼无闪烁
const int resolution = 8;   // 8位分辨率，对应数值范围 0~255

void setup() {
  // 初始化两个独立的 PWM 通道
  // 新版 API 直接绑定引脚、频率和分辨率
  ledcAttach(ledPinA, freq, resolution);
  ledcAttach(ledPinB, freq, resolution);
}

void loop() {
  // --- 阶段 1: 从 0 逐渐增加到 255 (渐亮过程) ---
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // 灯A：变亮
    ledcWrite(ledPinA, dutyCycle);
    
    // 灯B：变暗 (反相逻辑)
    // 当 dutyCycle 为 0 时，B 为 255；当 dutyCycle 为 255 时，B 为 0
    ledcWrite(ledPinB, 255 - dutyCycle);
    
    delay(5); // 延时 5ms，保证视觉上的平滑过渡
  }

  // --- 阶段 2: 从 255 逐渐减小到 0 (渐暗过程) ---
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // 灯A：变暗
    ledcWrite(ledPinA, dutyCycle);
    
    // 灯B：变亮 (反相逻辑)
    // 当 dutyCycle 为 255 时，B 为 0；当 dutyCycle 为 0 时，B 为 255
    ledcWrite(ledPinB, 255 - dutyCycle);
    
    delay(5); // 保持相同的延时，确保呼吸节奏一致
  }
}