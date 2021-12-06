#define inlineTimer(milliSeconds, func) ({\
    static int nextMillis =0; \
    if (nextMillis <= millis()){ \
      func; \
      nextMillis = millis() + milliSeconds; \
    } \
  } )