// ================= 引脚与参数定义 =================
#define TOUCH_PIN 4   // 触摸引脚 T0 (GPIO 4)
#define LED_PIN 5     // 【重要】LED 接在 GPIO 5 (避开 GPIO 2 的干扰)

// PWM 设置
const int freq = 5000;      // 频率 5kHz
const int resolution = 8;   // 分辨率 8位 (0-255)

// 触摸阈值 (根据之前的测试，可能需要微调，一般 < 20 为按下)
const int THRESHOLD = 20;   

// ================= 全局变量 =================
int speedLevel = 1;         // 速度档位: 1, 2, 3
bool lastTouchState = false; // 记录上一时刻是否被触摸

// 呼吸灯状态变量
int brightness = 0;         // 当前亮度 (0-255)
int fadeStep = 1;           // 亮度变化步长 (+1 或 -1)
unsigned long lastBreathTime = 0; // 上次更新亮度的时间戳

void setup() {
  Serial.begin(115200);
  
  // --- 初始化 LED PWM (新版写法 v3.0+) ---
  // ledcAttach(pin, freq, resolution)
  ledcAttach(LED_PIN, freq, resolution); 
  
  // 初始关闭 LED
  ledcWrite(LED_PIN, 0);
}

void loop() {
  // ================= 1. 触摸检测逻辑 =================
  int touchValue = touchRead(TOUCH_PIN);
  
  // 简单的防抖与边缘检测
  if (touchValue < THRESHOLD) {
    if (!lastTouchState) {
      // 检测到新的触摸动作
      lastTouchState = true;
      
      // 切换档位 1 -> 2 -> 3 -> 1
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1;
      
      Serial.print("Speed Level Changed to: ");
      Serial.println(speedLevel);
    }
  } else {
    lastTouchState = false; // 手松开，重置状态
  }

  // ================= 2. 呼吸灯逻辑 (非阻塞) =================
  unsigned long currentMillis = millis();
  
  // 根据档位决定呼吸速度 (延时越小，呼吸越快)
  unsigned long interval = 0;
  if (speedLevel == 1) interval = 30;   // 慢速呼吸
  else if (speedLevel == 2) interval = 15; // 中速呼吸
  else if (speedLevel == 3) interval = 5;  // 快速呼吸
  
  // 只有时间到了才更新亮度
  if (currentMillis - lastBreathTime >= interval) {
    lastBreathTime = currentMillis;
    
    // 更新亮度
    brightness += fadeStep;
    
    // 到达边界反转方向
    if (brightness <= 0 || brightness >= 255) {
      fadeStep = -fadeStep;
    }
    
    // 输出 PWM 波形 (新版写法直接写引脚号)
    ledcWrite(LED_PIN, brightness);
  }
}