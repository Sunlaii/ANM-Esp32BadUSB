#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "recon.h"
#include "payloads.h"

USBHIDKeyboard Keyboard;

// Nút bấm trên S2 Mini là chân số 0
const int BUTTON_PIN = 0;

const uint32_t DEBOUNCE_MS = 40;
const uint32_t DOUBLE_CLICK_MS = 800;

void onSingleClick() {
  Serial.println("Single click -> CMD");

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();

  //chờ hộp thoại hiện ra
  delay(500);

  Keyboard.println("cmd");

  delay(1000);

  Keyboard.println("Chao ban, S2 Mini dang thuc thi lenh!");
  Keyboard.releaseAll();

  Serial.println("Da go xong.");

  // Chờ bạn thả nút ra để tránh việc nó gõ liên tục
  while(digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }
}


void doubleClick(){ 
  Keyboard.releaseAll();
  Serial.println("Double click -> runRecon");
  runRecon();
  Keyboard.releaseAll();
}

void tripleClick(){ 
  Keyboard.releaseAll();
  delay(100); // Đợi hệ thống phản hồi việc nhả phím
  Serial.println("Triple click -> runSystemAudit");
  runSystemAudit();
  Keyboard.releaseAll();
}


//THAO TAC NHAN
int rawLevel = HIGH;
int stableLevel =HIGH;
int lastStableLevel = HIGH;
uint32_t lastBounceTime = 0;

//FSM
enum ClickState {
  IDLE,
  WAITING_SECOND_CLICK,
  WAITING_THIRD_CLICK
};

ClickState state = IDLE;

uint32_t firstClickTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Đợi 2-3 giây để máy tính nhận diện driver xong xuôi
  delay(3000); 

  Keyboard.begin();

  // Khởi động USB 
  USB.begin();

  // Cổng Serial ảo qua USB để bạn xem thông báo
  Serial.begin(115200);

}

//phat hien canh da nhan debounce
bool isPressedEdgeDebounced() {

  int currentRaw = digitalRead(BUTTON_PIN);
  uint32_t now = millis();

  if (currentRaw != rawLevel) {
    rawLevel = currentRaw;
    lastBounceTime = now;
  }

  if ((now - lastBounceTime) > DEBOUNCE_MS && stableLevel != rawLevel) {
    stableLevel = rawLevel;
    bool pressedEdge = (lastStableLevel == HIGH && stableLevel == LOW);

    lastStableLevel = stableLevel;

    return pressedEdge;
  }
  return false;
}

void loop() {

  uint32_t now = millis();
  bool pressedEdge = isPressedEdgeDebounced();

  switch(state) {

    case IDLE:
      if (pressedEdge) {
        firstClickTime = now;
        state = WAITING_SECOND_CLICK;
      }

      break;

      case WAITING_SECOND_CLICK:
        if (pressedEdge && (now - firstClickTime <= DOUBLE_CLICK_MS)) {
          Serial.println("-> Chuyển sang WAITING_THIRD_CLICK");
          state = WAITING_THIRD_CLICK;
        }

        else if (now - firstClickTime > DOUBLE_CLICK_MS) {
          Serial.println("-> Timeout, gọi Single Click");
          onSingleClick();
          state = IDLE;
        }

        break;

      case WAITING_THIRD_CLICK:
        if (pressedEdge && (now - firstClickTime <= DOUBLE_CLICK_MS)) {
          Serial.println("-> Đã nhận Triple Click!");
          tripleClick();
          state = IDLE;
        }
        else if (now - firstClickTime > DOUBLE_CLICK_MS) {
          //het thoi gian cho coi nhu la double click
          Serial.println("-> Timeout, gọi Double Click");
          doubleClick();
          state = IDLE;
        }
        break;
  }

}

