/*
  File:        CalcFunc.h
  Description: A simple expression parser
  Author:      J-L PONS

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 */

//=============================================================================
//
// $Author: pons $
//
// $Revision: 1.2 $
// $Date: $
//
// $HeadURL: $
//
//=============================================================================

/* Calculation functions */

#define UI(x) ((unsigned int)(floor(x)))
#define US(x) ((unsigned short)(floor(x)))
#define S(x) ((short)(floor(x)))
#define I(x) ((int)(floor(x)))

//== Utils Function ===========================================================

const double LOG2 = log(2.0);

static double fact(double x) {

  unsigned int f = UI(x);
  double r = 1.0;
  for(unsigned int i=1;i<=f;i++) {
    r = (double)i * r;
  }
  return (double)r;

}

static float RegistersToFloat(short reg1,short reg2) {

  float ret;

  unsigned char *ptr = (unsigned char *)&ret;
  unsigned short r1 = (unsigned short)reg1;
  unsigned short r2 = (unsigned short)reg2;

  ptr[0] = r1 & 0xFF;
  ptr[1] = (r1 >> 8) & 0xFF;
  ptr[2] = r2 & 0xFF;
  ptr[3] = (r2 >> 8) & 0xFF;
  return ret;

}

static double RegistersToDouble(short reg1,short reg2,short reg3,short reg4) {

  double ret;

  unsigned char *ptr = (unsigned char *)&ret;
  unsigned short r1 = (unsigned short)reg1;
  unsigned short r2 = (unsigned short)reg2;
  unsigned short r3 = (unsigned short)reg3;
  unsigned short r4 = (unsigned short)reg4;

  ptr[0] = r1 & 0xFF;
  ptr[1] = (r1 >> 8) & 0xFF;
  ptr[2] = r2 & 0xFF;
  ptr[3] = (r2 >> 8) & 0xFF;
  ptr[4] = r3 & 0xFF;
  ptr[5] = (r3 >> 8) & 0xFF;
  ptr[6] = r4 & 0xFF;
  ptr[7] = (r4 >> 8) & 0xFF;
  return ret;

}


//== Calculation Function =====================================================

VALUE OPER_PLUS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1) {
    for (int i = 0; i < b->lgth; i++) r.value[i] = b->value[i] + a->value[0];
    r.lgth = b->lgth;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] + b->value[0];
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] + b->value[i];
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_MINUS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] - b->value[0];
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] - b->value[i];
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_MUL(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1) {
    for (int i = 0; i < b->lgth; i++) r.value[i] = b->value[i] * a->value[0];
    r.lgth = b->lgth;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] * b->value[0];
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] * b->value[i];
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_DIV(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = a->value[0] / b->value[0];
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] / b->value[0];
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = a->value[i] / b->value[i];
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_OR(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = UI(a->value[0]) | UI(b->value[0]);
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) | UI(b->value[0]);
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) | UI(b->value[i]);
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_AND(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = UI(a->value[0]) & UI(b->value[0]);
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) & UI(b->value[0]);
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) & UI(b->value[i]);
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_XOR(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = UI(a->value[0]) ^ UI(b->value[0]);
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) ^ UI(b->value[0]);
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) ^ UI(b->value[i]);
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_LW(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] < b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_LWEQ(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] <= b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_GT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] > b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_GTEQ(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] >= b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_EQ(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] == b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_NEQ(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = (a->value[0] != b->value[0]);
    r.lgth = 1;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_LSHIFT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = UI(a->value[0]) << UI(b->value[0]);
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) << UI(b->value[0]);
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) << UI(b->value[i]);
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_RSHIFT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (a->lgth == 1 && b->lgth == 1) {
    r.value[0] = UI(a->value[0]) >> UI(b->value[0]);
    r.lgth = 1;
  } else if (b->lgth == 1) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) >> UI(b->value[0]);
    r.lgth = a->lgth;
  } else if (a->lgth == b->lgth) {
    for (int i = 0; i < a->lgth; i++) r.value[i] = UI(a->value[i]) >> UI(b->value[i]);
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_COS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = cos(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_POW(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (b->lgth == 1) {
    for (int i = 0; i < a->lgth && errno==0; i++) {
      r.value[i] = pow(a->value[i], b->value[0]);
    }
    if( errno!=0 ) obj->SetError((char *)strerror(errno));
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_MOD(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (b->lgth == 1) {
    for (int i = 0; i < a->lgth && errno==0; i++) {
      r.value[i] = fmod(a->value[i], b->value[0]);
    }
    if( errno!=0 ) obj->SetError((char *)strerror(errno));
    r.lgth = a->lgth;
  } else {
    obj->SetError((char *)"Incompatible value size");
  }
  return r;
}

VALUE OPER_FACT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth; i++) {
    r.value[i] = fact(a->value[i]);
  }
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_ACOS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = acos(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_SIN(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = sin(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_ASIN(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = asin(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_COSH(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = cosh(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_SINH(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = sinh(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_EXP(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = exp(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_LN(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = log(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_LOG10(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = log10(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_LOG2(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = log(a->value[i]) / LOG2;
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_INV(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth; i++) {
    r.value[i] = 1.0 / a->value[i];
  }
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_SQRT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = sqrt(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_TAN(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = tan(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_ATAN(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = atan(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_ABS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = fabs(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_TANH(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth && errno==0; i++) {
    r.value[i] = tanh(a->value[i]);
  }
  if( errno!=0 ) obj->SetError((char *)strerror(errno));
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_MINUS1(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth; i++) {
    r.value[i] = -a->value[i];
  }
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_NEG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth; i++) {
    r.value[i] = ~UI(a->value[i]);
  }
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_NOT(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  for (int i = 0; i < a->lgth; i++) {
    r.value[i] = !UI(a->value[i]);
  }
  r.lgth = a->lgth;
  return r;
}

VALUE OPER_REG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  short reg = obj->ReadModbusReg(info->reginfo.idx);
  r.value[0] = reg;
  r.lgth = 1;
  return r;
}

VALUE OPER_COIL(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  short coil = obj->ReadModbusCoil(info->reginfo.idx);
  r.value[0] = coil;
  r.lgth = 1;
  return r;
}

VALUE OPER_UREG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  short reg = obj->ReadModbusReg(info->reginfo.idx);
  r.value[0] = (unsigned short)(reg);
  r.lgth = 1;
  return r;
}

VALUE OPER_FREG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, 2);
  r.value[0] = (double)RegistersToFloat(regs[0], regs[1]);
  r.lgth = 1;
  return r;
}

VALUE OPER_DREG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, 4);
  r.value[0] = RegistersToDouble(regs[0], regs[1], regs[2], regs[3]);
  r.lgth = 1;
  return r;
}

VALUE OPER_REGS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  int l = info->reginfo.lgth;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, l);
  for (int i = 0; i < l; i++) r.value[i] = regs[i];
  r.lgth = l;
  return r;
}

VALUE OPER_COILS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  int l = info->reginfo.lgth;
  vector<short> coils = obj->ReadModbusCoil(info->reginfo.idx, l);
  for (int i = 0; i < l; i++) r.value[i] = coils[i];
  r.lgth = l;
  return r;
}

VALUE OPER_UREGS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  int l = info->reginfo.lgth;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, l);
  for (int i = 0; i < l; i++) r.value[i] = (unsigned short) regs[i];
  r.lgth = l;
  return r;
}

VALUE OPER_FREGS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  int l = info->reginfo.lgth;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, l * 2);
  for (int i = 0; i < l; i++) r.value[i] = (double)RegistersToFloat(regs[2 * i], regs[2 * i + 1]);
  r.lgth = l;
  return r;
}

VALUE OPER_DREGS(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  int l = info->reginfo.lgth;
  vector<short> regs = obj->ReadModbusReg(info->reginfo.idx, l * 4);
  for (int i = 0; i < l; i++) r.value[i] = RegistersToDouble(regs[2 * i], regs[2 * i + 1], regs[2 * i + 2], regs[2 * i + 3]);
  r.lgth = l;
  return r;
}

VALUE OPER_FLAG(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  vector<short> regs = obj->ReadModbusReg(info->flaginfo.idx, 1);
  unsigned short mask = 1 << (info->flaginfo.bit);
  unsigned short reg = (unsigned short) (regs[0]);
  if (reg & mask) {
    r.value[0] = 1.0;
  } else {
    r.value[0] = 0.0;
  }
  r.lgth = 1;
  return r;
}

VALUE OPER_XATTR(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  r.value[0] = obj->ReadExternAttribute(info->name);
  r.lgth = 1;
  return r;
}

VALUE OPER_DOUBLE(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  r.value[0] = (info->constant);
  r.lgth = 1;
  return r;
}

VALUE OPER_NAME(ExpParser *obj, ETREE_NODE *info, VALUE *a, VALUE *b) {
  VALUE r;
  if (strcasecmp(info->name, "value") == 0) {
    // Write value
    r.value[0] = obj->GetWriteValue();
    r.lgth = 1;
  } else {
    // Read external attribute
    r.value[0] = obj->ReadAttribute(info->name);
    r.lgth = 1;
  }
  return r;
}

