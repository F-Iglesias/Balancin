#ifndef utils_h
#define utils_h

inline float clamp(float min, float val, float max) {
    if (val<min) return min;
    if (val>max) return max;
    return val;
}

#endif

inline float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}