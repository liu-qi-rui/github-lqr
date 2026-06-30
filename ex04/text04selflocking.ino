// 定义引脚
#define TOUCH_PIN 4   // 触摸引脚 T0 (GPIO 4)
#define LED_PIN 2     // 板载 LED (GPIO 2)

// 定义阈值，需根据实际串口监视器数值调整
// 通常未触摸时数值较大(如60+)，触摸时会变小(如20以下)
int threshold = 30;

// 状态变量
bool ledState = false;        // LED当前状态：false为灭，true为亮
bool isTouching = false;      // 当前是否处于被触摸状态（用于边缘检测）
unsigned long lastDebounceTime = 0; // 上次状态改变的时间（用于防抖）
unsigned long debounceDelay = 50;   // 防抖延迟时间（毫秒）

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始化LED为灭
}

void loop() {
  // 1. 读取触摸值
  int touchValue = touchRead(TOUCH_PIN);

  // 打印数值以便调试（可选）
  // Serial.print("Touch Value: ");
  // Serial.println(touchValue);

  // 2. 判断当前物理状态 (触摸值小于阈值视为按下)
  bool currentTouchStatus = (touchValue < threshold);

  // 3. 边缘检测与防抖逻辑
  // 只有当“当前是按下”且“上一刻是松开”时（上升沿），才触发动作
  if (currentTouchStatus == true && isTouching == false) {

    // 简单的防抖检查：距离上次动作必须超过 debounceDelay 毫秒
    if (millis() - lastDebounceTime > debounceDelay) {

      // --- 执行自锁动作：翻转 LED 状态 ---
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      // 更新防抖时间戳
      lastDebounceTime = millis();

      Serial.println(ledState ? "LED ON" : "LED OFF");
    }
  }

  // 4. 更新上一刻的状态记录
  // 注意：这里直接更新，但动作只在上面的 if 中触发
  isTouching = currentTouchStatus;

  delay(10); // 短暂延时，降低CPU占用
}