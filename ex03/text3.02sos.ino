
const int LED_PIN = 2;  

// 时间参数（毫秒）
const int SHORT_ON = 200;   // 短闪亮灯时间
const int SHORT_OFF = 200;  // 短闪灭灯时间
const int LONG_ON = 600;    // 长闪亮灯时间
const int LONG_OFF = 200;   // 长闪灭灯时间
const int PAUSE = 1200;     // SOS结束后的停顿时间

// SOS信号序列：1=短闪, 2=长闪, 0=停顿
const int SOS_SEQUENCE[] = {1, 1, 1, 2, 2, 2, 1, 1, 1, 0};
const int SEQUENCE_LEN = 10;

// 状态变量
unsigned long previousMillis = 0;
int stepIndex = 0;          // 当前执行到序列的第几步
bool ledState = false;
bool isPause = false;       // 是否处于停顿状态
int currentStepDuration = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // 初始化第一步
  updateStep();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 检查是否到达切换时间
  if (currentMillis - previousMillis >= currentStepDuration) {
    previousMillis = currentMillis;
    
    if (isPause) {
      // 停顿结束，重新开始序列
      isPause = false;
      stepIndex = 0;
      updateStep();
    } else {
      // 切换LED状态（亮/灭交替）
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      
      // 如果当前步骤是灭灯状态，进入下一步
      if (!ledState) {
        stepIndex++;
        if (stepIndex >= SEQUENCE_LEN) {
          // SOS播放完毕，进入停顿
          isPause = true;
          currentStepDuration = PAUSE;
          digitalWrite(LED_PIN, LOW);
        } else {
          updateStep();
        }
      }
    }
  }
}

// 更新当前步骤的时间和状态
void updateStep() {
  int signal = SOS_SEQUENCE[stepIndex];
  
  if (signal == 1) {  // 短闪
    currentStepDuration = ledState ? SHORT_OFF : SHORT_ON;
  } else if (signal == 2) {  // 长闪
    currentStepDuration = ledState ? LONG_OFF : LONG_ON;
  } else {  // 停顿
    isPause = true;
    currentStepDuration = PAUSE;
    digitalWrite(LED_PIN, LOW);
  }
}