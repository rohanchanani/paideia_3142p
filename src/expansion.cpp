#include "vex.h"
#include "global.h"
#include <cmath>

using namespace vex;

int extensionVal = 0;
bool extended = true;
void updateExtension() {
  if (extensionVal < 4) {
    extensionVal++;
  } else {
    extension.set(true);
  }
}