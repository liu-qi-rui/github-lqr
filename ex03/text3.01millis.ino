
// 定义LED引脚
const int LED_PIN = 2;  // ESP32开发板上的内置LED通常连接GPIO2

// 使用 unsigned long 类型存储时间变量（millis()返回值的类型）
unsigned long previousMillis = 0;  // 上一次LED状态改变的时间
const unsigned long interval = 500; // 闪烁间隔：500毫秒（1Hz = 亮500ms + 灭500ms）

// LED当前状态
bool ledState = false;

void setup() {
  // 初始化LED引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  
  // 可选：初始化串口用于调试（非必需）
  // Serial.begin(115200);
  // Serial.println("LED 1Hz 闪烁开始...");
}

void loop() {
  // 获取当前系统运行时间（毫秒）
  unsigned long currentMillis = millis();
  
  // 检查是否到达切换LED状态的时间
  if (currentMillis - previousMillis >= interval) {
    // 保存本次切换的时间，作为下一次切换的基准
    previousMillis = currentMillis;
    
    // 切换LED状态
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    
  }
  
  
}