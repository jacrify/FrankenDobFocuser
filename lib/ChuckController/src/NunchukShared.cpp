
#include "NunchukShared.h"
#include <Math.h>
// #define HOLD_DOWN_TIME_MILLIS 3000
// void HistoricalChuckState::storeState(wii_i2c_nunchuk_state s) { state = s; }

// HistoricalChuckState last;


// void HistoricalChuckState::processChuckState(wii_i2c_nunchuk_state s) {
// 
//   last.storeState(state);
//   //   delete lastProcessedState;
//   lastProcessedState = &last;
//   state = s;

//   if (isCNewlyPushed()) {
//     cPushTime = s.millis;
//   }

//   if (isZNewlyPushed()) {
//     zPushTime = s.millis;
//   }

//   if (isBothNewlyPushed()) {
//     bothPushTime = s.millis;
//   }
// }

// bool HistoricalChuckState::isCNewlyPushed() {
  
// }
// bool HistoricalChuckState::isZNewlyPushed() {
//   return isZPushed() and lastProcessedState->isZReleased();
// }
// bool HistoricalChuckState::isBothNewlyPushed() {
//   return isBothPushed() and !(lastProcessedState->isBothPushed());
// }

// bool HistoricalChuckState::isCReleased() {
//   return (lastProcessedState->isCPushed() and !isCPushed());
// }
// bool HistoricalChuckState::isZReleased() {
//   return (lastProcessedState->isZPushed() and !isZPushed());
// };
// bool HistoricalChuckState::isBothReleased() {
//   return (lastProcessedState->isBothPushed() and !isBothPushed());
// }
// bool HistoricalChuckState::isCHeld() {
//   return (isCPushed() and ((state.millis - cPushTime) > HOLD_DOWN_TIME_MILLIS));
// }

// bool ChuckState::isZHeld();
// bool ChuckState::isBothHeld();
// bool ChuckState::isCReleasedAfterHeld();
// bool ChuckState::isZReleasedAfterHeld();
// bool ChuckState::isBothReleasedAfterHeld();
