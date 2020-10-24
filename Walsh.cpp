#include "SC_PlugIn.h"
#include <iostream>
using namespace std;

static InterfaceTable *ft;

struct Walsh : public Unit {};

extern "C" {
  static void load(InterfaceTable *inTable);
  static void Walsh_Ctor(Walsh *unit);
  static void Walsh_next(Walsh *unit, int inNumSamples);
  static void Walsh_Dtor(Walsh *unit);
}

void Walsh_Ctor(Walsh *unit) {
  int32 maxindex = unit->mNumInputs;
  if(!ISPOWEROFTWO(maxindex)){
    Print("Walsh UGEN: The number of Inputs (%i) is not a power of Two\n", maxindex);
    return;
  }

  SETCALC(Walsh_next);
  Walsh_next(unit,1);
}

float * process(float a[], size_t n) {
  int endVal = n;
  int h = 1;
  while (h < endVal) {
    for (int i = 0; i < endVal; i+=h*2) {
      for (int j = i; j< i+h; j++) {
        float x = a[j];
        float y = a[j+h];
        a[j] = x+y;
        a[j+h] = x-y;
      }
    }
    h *= 2;
  }
  for (int val = 0; val < n; val++){
    a[val] = a[val]*(1/pow(2,log2f(n)-1));
  }
  return a;
}

void Walsh_next(Walsh* unit, int inNumSamples) {
  float *in;
  float *out;
  int n = unit->mNumInputs;
  out = OUT(0);
  float a[n];
  for (int ch = 0; ch < n; ch++) {
    in = IN(ch);
    a[ch] = *in;
  }

  float *result = process(a, n);

  for(int i=0; i<inNumSamples; i++){
    out = &result[i];
  }
}

void Walsh_Dtor(Walsh * unit) {}

PluginLoad(WalshUgens) {
  ft = inTable;
  DefineSimpleUnit(Walsh);
}
