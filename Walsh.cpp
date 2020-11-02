#include "SC_PlugIn.h"
#include <iostream>
using namespace std;

static InterfaceTable *ft;

struct Walsh : public Unit {};

// extern "C" {
  // static void load(InterfaceTable *inTable);
  static void Walsh_Ctor(Walsh *unit);
  static void Walsh_next(Walsh *unit, int inNumSamples);
  static void Walsh_Dtor(Walsh *unit);
// }

void Walsh_Ctor(Walsh *unit) {
  SETCALC(Walsh_next);
  Walsh_next(unit,1);
}

// Fast Walsh-Hadamard Transform
void fwht(float *a, size_t n) {
  for (int h=1; h < n; h *=2){
    for (int i = 0; i < n; i+=h*2) {
      for (int j = i; j< i+h; j++) {
        float x = a[j];
        float y = a[j+h];
        a[j] = x+y;
        a[j+h] = x-y;
      }
    }
  }
  for (int i = 0; i < n; i++){
    a[i] = a[i]*(1/pow(2,log2f(n)-1));
  }
}

void Walsh_next(Walsh* unit, int inNumSamples) {
  int n = unit->mNumInputs;
  float arr[n];


  // Block Size loop
  for (int i = 0; i < inNumSamples; i++) {
    // get the i sample value from each input and put it in an array
    for (int ch = 0; ch < n; ch++) {
      float *in = IN(ch);
        arr[ch] = in[i]; // array of n sample from channel [IN(0)[i],IN(1)[i],IN(2)[i]..IN(n)[i]]
    }
    fwht(arr, n);
    // route array to the output
    for(int ch=0; ch<n; ch++){
      float *out = OUT(ch);
      out[i] = arr[ch];
    }
  }
}

// not sure if it is needed
void Walsh_Dtor(Walsh * unit) {}

PluginLoad(WalshUgens) {
  ft = inTable;
  DefineSimpleUnit(Walsh);
}
