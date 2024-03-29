#include "../ControllerQueue/C015_queue_element.h"

#include "../DataStructs/ESPEasy_EventStruct.h"

#ifdef USES_C015

C015_queue_element::C015_queue_element() {}

C015_queue_element::C015_queue_element(const struct EventStruct *event, byte value_count) :
  idx(event->idx),
  TaskIndex(event->TaskIndex),
  controller_idx(event->ControllerIndex),
  valuesSent(0),
  valueCount(value_count) {}

bool C015_queue_element::checkDone(bool succesfull) const {
  if (succesfull) { ++valuesSent; }
  return valuesSent >= valueCount || valuesSent >= VARS_PER_TASK;
}

size_t C015_queue_element::getSize() const {
  size_t total = sizeof(*this);

  for (int i = 0; i < VARS_PER_TASK; ++i) {
    total += txt[i].length();
  }
  return total;
}

bool C015_queue_element::isDuplicate(const C015_queue_element& other) const {
  if (other.controller_idx != controller_idx || 
      other.TaskIndex != TaskIndex ||
      other.sensorType != sensorType ||
      other.valueCount != valueCount ||
      other.idx != idx) {
    return false;
  }
  for (byte i = 0; i < VARS_PER_TASK; ++i) {
    if (other.txt[i] != txt[i]) {
      return false;
    }
    if (other.vPin[i] != vPin[i]) {
      return false;
    }
  }
  return true;
}


#endif
