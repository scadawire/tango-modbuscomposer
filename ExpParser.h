/*
  File:        ExpParser.h
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

/* Supported gramar :                                                              */
/*                                                                                 */
/* <attdef> ::= <name> '=' <type>'(' <expr> [, <writefn>'('<integer>,<expr>')']')' */
/* <statedef> ::= <name> '=' <expr> [, <status_string>]                          */
/*                                                                               */
/* <expr>   ::= <factor1> [ <oper1> <factor1> ]                                  */
/* <factor1>::= <factor2> [ <oper2> <factor2> ]                                  */
/* <factor2>::= <factor3> [ <oper3> <factor3> ]                                  */
/* <factor3>::= <factor4> [ <oper4> <factor4> ]*                                 */
/* <factor4>::= <term> [ <oper5> <term> ]*                                       */
/*                                                                               */
/* <term>   ::= '(' <exp> ')'      |                                             */
/*                <double>         |                                             */
/*                <name>           |                                             */
/*               '-' <term>        |                                             */
/*               '~' <term>        |                                             */
/*               'not ' <term>     |                                             */
/*               'sin(' <exp> ')'  |                                             */
/*               'cos(' <exp> ')'  |                                             */
/*               'tan(' <exp> ')'  |                                             */
/*               'asin(' <exp> ')' |                                             */
/*               'acos(' <exp> ')' |                                             */
/*               'atan(' <exp> ')' |                                             */
/*               'abs(' <exp> ')'  |                                             */
/*               'sinh(' <exp> ')' |                                             */
/*               'cosh(' <exp> ')' |                                             */
/*               'tanh(' <exp> ')' |                                             */
/*               'exp(' <exp> ')'  |                                             */
/*               'ln(' <exp> ')'   |                                             */
/*               'log(' <exp> ')'  |                                             */
/*               'inv(' <exp> ')'  |                                             */
/*               'sqrt(' <exp> ')' |                                             */
/*               'fact(' <exp> ')'  |                                            */
/*               'pow(' <exp>,<exp> ')'  |                                       */
/*               'sum(' <variable>,<double>,<double> ')' |                       */
/*                                                                               */
/*               'xattr('' <full_qualified_att_name> '')' |   (Extern tango att)   */
/*                                                                               */
/*                                                      Scalar reading:          */
/*               'reg(' <integer> ')' |                 Read 16 bit signed       */
/*               'ureg(' <integer> ')' |                Read 16 bit unsigned     */
/*               'freg(' <integer> ')' |                Read 2*16bit float       */
/*               'dreg(' <integer> ')' |                Read 4*16bit double      */
/*               'flag(' <integer>,<integer> ')' |      Read bit n of reg        */
/*               'flag(' <integer>,<integer> ')' |      Read bit n of reg        */
/*                                                                               */
/*                                                      Vector reading:          */
/*               'regs(' <integer>,<integer> ')' |      Read n*16 bit signed     */
/*               'uregs(' <integer>,<integer> ')' |     Read n*16 bit unsigned   */
/*               'fregs(' <integer>,<integer> ')' |     Read n*2*16bit float     */
/*               'dregs(' <integer>,<integer> ')' |     Read n*4*16bit double    */
/*               'PI'                                                            */
/*                                                                               */
/* <oper1>    ::= '&' | '|' | '^'                          lowest precedence     */
/* <oper2>    ::= '<' | '>' | '>=' | '<=' | '==' | '!='                          */
/* <oper3>    ::= '<<' | '>>'                                                    */
/* <oper4>    ::= '+' | '-'                                                      */
/* <oper5>    ::= '*' | '/'                                highest precedence    */
/*                                                                               */
/* <writefn>  ::= 'WriteInt' |       Write 16 bit registers (signed)             */
/*                'WriteUInt' |      Write 16 bit registers (unsigned)           */
/*                'WriteLong' |      Write 2*16 bit registers (signed)           */
/*                'WriteULong' |     Write 2*16 bit registers (unsigned)         */
/*                'WriteLongLSB' |   Write 2*16 bit registers (signed LSB first) */
/*                'WriteULongLSB' |  Write 2*16 bit registers (unsigned LSB first)*/
/*                'WriteFloat' |     Write float value in 2 consecutive regs     */
/*                'WriteDouble' |    Write double value in 4 consecutive regs    */
/*                'WriteCoil'        Write single coil                           */
/*                                                                               */
/* <type>     :: 'DevBoolean'| 'DevShort' | 'DevDouble' | 'DevLong' |            */
/*               'DevVarDoubleArray'                                             */
/*                                                                               */
/* <double>   ::= <nb>* '.' <nb> <nb>* ['E' [-] <nb> <nb>*] | <integer>          */
/* <integer>  ::= ['0x']<nb>*                                                    */
/* <nb>       ::= '0'..'9'                                                       */
/* <name>     ::= <letter>[<letter> | <nb>]*                                     */
/* <letter>   ::= 'A'..'Z' | 'a'..'z' | '_'                                      */
/*                                                                               */
//================================================================================
//
// $Author: pons $
//
// $Revision: 1.2 $
// $Date: $
//
// $Log: $
//
//=============================================================================

#ifndef _EXPPARSERH_
#define _EXPPARSERH_

#include <ModbusComposer.h>

namespace ModbusComposer_ns
{

// Write function type
#define REG_INT      1     
#define REG_UINT     2     
#define REG_LONG     3     
#define REG_ULONG    4     
#define REG_FLOAT    5     
#define REG_DOUBLE   6
#define REG_COIL     7
#define REG_LONGLSB  8     
#define REG_ULONGLSB 9     

#define MAXLENGHT       64  // Maximun name length
#define MAXVALUELENGTH  64  // Maximum vector length

// value type
typedef struct _TVALUE {
  double   value[MAXVALUELENGTH];
  int      lgth;
} VALUE;

// reg info
typedef struct _REGINFO {  
  int idx;
  int lgth;  
} REGINFO;

// flag info
typedef struct _FLAGINFO {  
  int idx;
  int bit;
} FLAGINFO;

//Evaluation tree node info
typedef union {
  REGINFO  reginfo;
  FLAGINFO flaginfo;
  char     name[MAXLENGHT];
  double   constant;
} ETREE_NODE;

// Calculation function
class ExpParser;
typedef VALUE (*CALCFN)(ExpParser *obj,ETREE_NODE *info,VALUE *a,VALUE *b);

//Evaluation tree node
typedef struct _ETREE {
  CALCFN         calc_fn;
  ETREE_NODE     info;
  struct _ETREE *left;
  struct _ETREE *right;
} ETREE;

// Device factory
typedef struct {
  Tango::DeviceProxy *ds;
  string devName;
} DEV_ITEM;

class ExpParser {

public:

  // Construct an expression parser
  // modbusDS: Handle to the modbus device
  // selfDS: Handle to device proxy on self (for attribute reading)
  ExpParser(ModbusComposer *parent);
  ~ExpParser();

  // Formula name and type
  char *GetName();
  char *GetStatus();
  long  GetType();
  Tango::DevState GetState();
  bool  IsSpectrum();
  bool  GetBoolResult(VALUE r);

  // Expression management
  void  SetExpression(char *expr);   // Set expression
  char *GetExpression();             // Get the expression
  void  ParseAttribute();            // Compile attribute expression
  void  ParseCommand();              // Compile command expression
  void  ParseState();                // Compile state expression
  int   GetCurrentPos();             // Current parsing cursor position
  bool  HasWriteExpression();        // Return true if a write expression is specified

  // Evaluation
  void   EvaluateRead(VALUE *result);  // Evaluate the read expression
  void   EvaluateWrite(double wValue); // Evaluate the write expression and write register(s)
  double GetWriteValue();              // Return current write value

  
  vector<short> ReadModbusReg( int address , int length ); 
  short ReadModbusReg( int address );
  double ReadAttribute(char *attName);
  double ReadExternAttribute(char *attName);

  // Error function
  void   SetError(char *err,int p=-1);

  // Temporary value
  Tango::DevDouble  dValue;
  Tango::DevBoolean bValue;
  Tango::DevLong    lValue;
  Tango::DevShort   sValue;

private:

  VALUE  EvalTree(ETREE *t);
  void   ReadExpression(ETREE **node);
  void   ReadTerm(ETREE **node);
  void   ReadFactor1(ETREE **node);
  void   ReadFactor2(ETREE **node);
  void   ReadFactor3(ETREE **node);
  void   ReadFactor4(ETREE **node);

  void   ReadName( char *name );
  void   ReadAttName( char *name );
  void   ReadType();
  void   ReadState();
  void   ReadWriteFn();
  void   ReadDouble(double *R);
  void   ReadInteger(int *R);
  void   AddNode(CALCFN fn,ETREE_NODE info,ETREE **t,ETREE *left,ETREE *right);
  void   AV();
  void   AV(int n);
  bool   Match(string word);
  bool   IsLetter(char c);
  Tango::DeviceProxy *Import(string devName);

  void FloatToRegisters(float f,unsigned short *r1,unsigned short *r2);
  void DoubleToRegisters(double f,unsigned short *r1,unsigned short *r2,unsigned short *r3,unsigned short *r4);

  ModbusComposer *parent; // Parent
  char name[MAXLENGHT];   // Attribute or State Name
  long type;              // Attribute data type
  char status[512];       // status string
  Tango::DevState state;  // Inner state
  int  regWriteType;      // writefn index
  bool isSpectrum;        // Spectrum/Saclar
  char expr[512];         // Expression
  char EC;                // Current char
  int  current;           // Current char index
  int  writeAddress;      // Write address
  int  exprLgth;          // Expression length
  double writeVALUE;      // Value used if write expression

  ETREE *evalTree;
  ETREE *writeTree;
  vector<DEV_ITEM> devices;

};

} // namespace ModbusComposer_ns

#endif /* _EXPPARSERH_ */
