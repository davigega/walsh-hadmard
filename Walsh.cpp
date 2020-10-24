#include "SC_PlugIn.h"
#include <iostream>
using namespace std;

static InterfaceTable *ft;

struct Walsh : public Unit {};

// extern "C" {
//   static void load(InterfaceTable *inTable);
//   static void Walsh_Ctor(Walsh *unit);
//   static void Walsh_next(Walsh *unit, int inNumSamples);
//   static void Walsh_Dtor(Walsh *unit);
// }

void Walsh_Ctor(Walsh *unit) {
  int32 maxindex = unit->mNumInputs;
  // if num of inputs is not a power of two stop the program
  if(!ISPOWEROFTWO(maxindex)){
    Print("Walsh UGEN: The number of Inputs (%i) is not a power of Two\n", maxindex);
    return;
  }

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
  float *in;
  float *out;
  int n = unit->mNumInputs;
  out = OUT(0);
  float a[n];
  // get saple value from each input e put it in array
  for (int ch = 0; ch < n; ch++) {
    in = IN(ch);
    a[ch] = *in;
  }

  float *result = fwht(a, n); // returns an array

  // send the array to the output
  for(int i=0; i<inNumSamples; i++){
    out = &result[i];
  }
}

// not sure if it is needed
void Walsh_Dtor(Walsh * unit) {}

PluginLoad(WalshUgens) {
  ft = inTable;
  DefineSimpleUnit(Walsh);
}
