/*
  File:        ExpParser.cpp
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
//
// $Log: $
//
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <errno.h>

namespace ModbusComposer_ns
{
class ExpParser;
}

#include "ExpParser.h"

namespace ModbusComposer_ns
{

#include "CalcFunc.h"

// -------------------------------------------------------
// Utils functions
// -------------------------------------------------------

// Add a character to a string
static void stradd(char *s,char c)
{
  int l=(int)strlen(s);
  s[l]=c;
  s[l+1]=0;
}

// free a tree
static void free_tree(ETREE *t) {
  if(t->left!=NULL)  free_tree(t->left);
  if(t->right!=NULL) free_tree(t->right);
  free(t);
}

static void safe_free_tree(ETREE **t)
{
  if(*t!=NULL) {
    free_tree(*t);
    *t=NULL;
  }
}

// -------------------------------------------------------
// ExpParser
// -------------------------------------------------------

ExpParser::ExpParser(ModbusComposer *parent) {

  this->parent = parent;
  evalTree=NULL;
  writeTree=NULL;
  strValue = NULL;
  memset(expr,0,sizeof(expr));  
  strcpy(name,"");
  strcpy(status,"");
  state = Tango::UNKNOWN;
  devices.clear();
  isMEM = 0;
}

// -------------------------------------------------------

ExpParser::~ExpParser() {

  safe_free_tree(&evalTree);
  safe_free_tree(&writeTree);
  for(int i=0;i<(int)devices.size();i++)
    delete devices[i].ds;
  devices.clear();
  if(strValue) free(strValue);

}

// -------------------------------------------------------

char *ExpParser::GetName() {
  return name;
}

// -------------------------------------------------------

long ExpParser::GetType() {
  return type;
}

// -------------------------------------------------------

void ExpParser::SetExpression(char *expr) {
  if(strlen(expr)>sizeof(this->expr)) 
    SetError((char *)"Expression too long");
  strcpy(this->expr,expr);
}

// -------------------------------------------------------

char *ExpParser::GetExpression() {
  return expr;
}

// -------------------------------------------------------
// Return true if next string match given string 

bool ExpParser::Match(string word)
{
  char s[128];
  int i;
  int lg = word.length();
  s[0]=0;
  for(i=current;i<current+lg;i++)
    stradd(s,expr[i]);

  return strcasecmp(s,word.c_str())==0;
}

// -------------------------------------------------------

void ExpParser::AV()
{
  // Return fisrt significative char in the string to analyse 
  do {
    current++;
    EC=expr[current];
  } while (current<exprLgth && (EC==' ' || EC=='\t'));
}

void ExpParser::AV(int n) {
  for(int i=0;i<n;i++)
    AV();
}

// -------------------------------------------------------

void ExpParser::SetError(char *err,int p) {

  char errMsg[512];

  if(p>=0)
    sprintf(errMsg,"%s at %d",err,p);
  else
    strcpy(errMsg,err);

  Tango::Except::throw_exception(	    
      (const char *)"ExpParser::error",
      (const char *)errMsg,
      (const char *)"ExpParser::SetError"); 

}


// -------------------------------------------------------

void ExpParser::AddNode(CALCFN fn , ETREE_NODE info ,
                        ETREE **t,ETREE *left,ETREE *right) {

  // Add node into the evaluation tree
  ETREE *elem;
  elem=(ETREE *)malloc(sizeof(ETREE));
  elem->calc_fn=fn;
  elem->info=info;
  elem->left=left;
  elem->right=right;

  *t=elem;

}

// -------------------------------------------------------
// Gramar functions
// -------------------------------------------------------

void ExpParser::ReadDouble(double *R)
{
  char S[128];
  char ex[128];
  int  c;
  int  p;
  int  nega;
  int  n;

  S[0]=0;
  ex[0]=0;
  p=current;

  if( Match("0x") ) {
    // We have an hexadecimal number
    ReadInteger(&n);
    *R = (double)n;
    return;
  }

  do {
    stradd(S,EC);
    AV();
  } while ( (EC>='0' && EC<='9') || EC=='.' );

  if (EC=='E' || EC=='e') {
    AV();
    nega=false;

    if (EC=='-') {
      nega=true;
      AV();
    }

    if (EC=='+') {
      AV();
    }

    while (EC>='0' && EC<='9')
    {
      stradd(ex,EC);
      AV();
    }
    c=sscanf(ex,"%d",&n);
    if (c==0) { SetError((char *)"Incorrect exposant in number",p);return; }
    if (nega) { strcat(S,"e-");strcat(S,ex);}
    else      { strcat(S,"e");strcat(S,ex); }
  }

  c=sscanf(S,"%lf",R);  
  if (c==0) SetError((char *)"Incorrect number",p);

}

// -------------------------------------------------------

bool ExpParser::IsLetter(char c) {

  return (EC>='A' && EC<='Z') || (EC>='a' && EC<='z') || (EC=='_');

}

// -------------------------------------------------------

void ExpParser::ReadInteger(int *R)
{
  char S[128];
  char ex[128];
  int  c;
  int  p;
  int  isHexa;

  S[0]=0;
  ex[0]=0;
  p=current;

  isHexa = Match("0x");
  if(isHexa) {

    stradd(S,EC);AV();
    stradd(S,EC);AV();

    do {
      stradd(S,EC);
      AV();
    } while ( (EC>='0' && EC<='9') || (EC>='A' && EC<='F') || (EC>='a' && EC<='f') );

  } else {

    do {
      stradd(S,EC);
      AV();
    } while ( (EC>='0' && EC<='9') );
  
  }
  
  if( strlen(S)==0 ) SetError((char *)"Incorrect integer number",p);

  if( isHexa ) {
    errno=0;
    *R = strtol(S,NULL,16);
    if(errno!=0) {
      cerr << "Invalid hexadecimal number:" << S << endl;
      SetError((char *)"Incorrect hexadecimal number",p);
    }
  } else {
    c=sscanf(S,"%d",R);
    if (c==0) {
      cerr << "Invalid number:" << S << endl;
      SetError((char *)"Incorrect integer number",p);
    }
  }
  
}

// -------------------------------------------------------

void ExpParser::ReadName( char *name ) {

  int i=0;
  int p;
  p=current;
  
  /*
  if( !IsLetter(EC) ) {
    error = true;
    SetError((char *)"Name expected",p);
    return;
  }
  */

  name[0]=0;
  do {

    stradd(name,EC);

    current++;
    EC=expr[current];

    i++;
    if(i>=MAXLENGHT) SetError((char *)"Variable name too long",p);

  } while (
     (EC>='A' && EC<='Z') || 
     (EC>='a' && EC<='z') ||
     (EC>='0' && EC<='9') ||
     (EC=='_'));

  // Jump to the next significative char
  while(current<exprLgth && (EC==' ' || EC=='\t')) {
    current++;
    EC=expr[current];
  }

}

// -------------------------------------------------------

void ExpParser::ReadAttName( char *name ) {

  int i=0;
  int p;
  p=current;
  
  /*
  if( !IsLetter(EC) ) {
    error = true;
    SetError((char *)"Name expected",p);
    return;

  }
  */

  name[0]=0;
  while( EC!='\'' ) {

    stradd(name,EC);

    current++;
    EC=expr[current];

    i++;
    if(i>=MAXLENGHT) SetError((char *)"Attribute name too long",p);

  }

}

// -------------------------------------------------------

void ExpParser::ReadTerm(ETREE **node) {

  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  switch(EC) {

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':  ReadDouble(&(elem.constant));
      AddNode( OPER_DOUBLE , elem , node , NULL , NULL);
      break;

    case '(' : AV();
      ReadExpression(node);
      if (EC!=')') SetError((char *)") expected",current);
      AV();
      break;

    case '{' : {
      AV();

      // Construct a vector
      ETREE *V[MAXVALUELENGTH];
      int nbValue = 0;
      ReadExpression(&V[nbValue++]);
      while (EC == ',' && nbValue < MAXVALUELENGTH) {
        AV();
        ReadExpression(&V[nbValue++]);
      }
      if (EC != '}') SetError((char *) "} expected", current);

      int i = 0;
      for (; i < nbValue - 1 ; i++) {
        AddNode(OPER_CONCAT, elem, node, V[i], NULL);
        node = &((*node)->right);
      }
      *node = V[i];

      AV();
      }
      break;

      // -------------------------------------------------------
      // unary operator
      // -------------------------------------------------------

    case '-' :AV();
      ReadTerm(&l_t);
      AddNode( OPER_MINUS1 , elem , node , l_t , NULL);
      break;

    case '~' :AV();
      ReadTerm(&l_t);
      AddNode( OPER_NEG , elem , node , l_t , NULL);
      break;

    case 'N':
    case 'n': if( Match("not") ) {
        AV(3);
        ReadTerm(&l_t);
        AddNode( OPER_NOT , elem , node , l_t , NULL);
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

      // -------------------------------------------------------
      // Math functions
      // -------------------------------------------------------

    case 'a':
    case 'A': if ( Match("abs(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_ABS , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("asin(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_ASIN , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("acos(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_ACOS , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else  if ( Match("atan(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_ATAN , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'S':
    case 's': if ( Match("sin(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_SIN , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("sqrt(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_SQRT , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("sinh(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_SINH , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("sum(") ) {
        AV(4);
        if( (EC>='A' && EC<='Z') ||
            (EC>='a' && EC<='z') ||
            (EC=='_'))
        {
          char tmpVName[MAXLENGHT];
          double d1,d2;
          int i,i1,i2;

          ReadName((char *)tmpVName);
          if (EC!=',') SetError((char *)", expected",current);AV();
          ReadDouble(&d1);
          if (EC!=',') SetError((char *)", expected",current);AV();
          ReadDouble(&d2);
          if (EC!=')') SetError((char *)") expected",current);AV();

          // Add all variables
          i1 = I(d1);
          i2 = I(d2);

          // 1st
          sprintf(elem.name,"%s%d",tmpVName,i1);
          AddNode( OPER_NAME , elem , &l_t , NULL , NULL);

          for(i=i1+1;i<=i2;i++) {
            sprintf(elem.name,"%s%d",tmpVName,i);
            AddNode( OPER_NAME , elem , &r_t , NULL , NULL);
            AddNode( OPER_PLUS , elem , &l_t , l_t , r_t );
          }
          *node = l_t;

        } else {
          SetError((char *)"variable prefix name expected",current);
        }
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'C':
    case 'c': if ( Match("cos(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_COS , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("cosh(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_COSH , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("coil(") ) {
        int idx;
        AV(5);
        ReadInteger(&idx);
        elem.reginfo.cmd = 0;
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_COIL , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("coils(") ) {
        int idx,lgth;
        AV(6);
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&lgth);
        elem.reginfo.cmd = 0;
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_COILS , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'E':
    case 'e': if ( Match("exp(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_EXP , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'D':
    case 'd':  if ( Match("dreg(") ) {
        int idx;
        AV(5);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_DREG , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("dregbe(") ) {
        int idx;
        AV(7);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_DREGBE , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("dregs(") ) {
        int idx,lgth;
        AV(6);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&lgth);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_DREGS , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("dregsbe(") ) {
        int idx,lgth;
        AV(8);
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&lgth);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_DREGSBE , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'F':
    case 'f': if ( Match("fact(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_FACT , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("flag(") ) {
        int idx,bit;
        AV(5);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&bit);
        elem.flaginfo.idx = idx;
        elem.flaginfo.bit = bit;
        AddNode( OPER_FLAG , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("freg(") ) {
        int idx;
        AV(5);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_FREG , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("fregbe(") ) {
        int idx;
        AV(7);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_FREGBE , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("fregs(") ) {
        int idx,lgth;
        AV(6);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&lgth);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_FREGS , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("fregsbe(") ) {
        int idx,lgth;
        AV(8);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);AV();
        ReadInteger(&lgth);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_FREGSBE , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'I':
    case 'i': if ( Match("inv(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_INV , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'L':
    case 'l': if ( Match("ln(") ) {
        AV(3);
        ReadExpression(&l_t);
        AddNode( OPER_LN , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("log2(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_LOG2 , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("log10(") ) {
        AV(6);
        ReadExpression(&l_t);
        AddNode( OPER_LOG10 , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'T':
    case 't': if ( Match("tan(") ) {
        AV(4);
        ReadExpression(&l_t);
        AddNode( OPER_TAN , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("tanh(") ) {
        AV(5);
        ReadExpression(&l_t);
        AddNode( OPER_TANH , elem , node , l_t , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'r':
    case 'R': if ( Match("reg(") ) {
        int idx;
        AV(4);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_REG , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("regs(") ) {
        int idx,lgth;
        AV(5);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);
        AV();
        ReadInteger(&lgth);
        if(lgth>MAXVALUELENGTH) SetError((char *)"regs(): too many registers",current);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_REGS , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'u':
    case 'U': if ( Match("ureg(") ) {
        int idx;
        AV(5);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = 1;
        AddNode( OPER_UREG , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else if ( Match("uregs(") ) {
        int idx,lgth;
        AV(6);
        elem.reginfo.cmd = ReadCommandName();
        ReadInteger(&idx);
        if (EC!=',') SetError((char *)", expected",current);
        AV();
        ReadInteger(&lgth);
        if(lgth>MAXVALUELENGTH) SetError((char *)"uregs(): too many registers",current);
        elem.reginfo.idx = idx;
        elem.reginfo.lgth = lgth;
        AddNode( OPER_UREGS , elem , node , NULL , NULL);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'x':
    case 'X': if ( Match("xattr(") ) {
        AV(6);
        if (EC!='\'') SetError((char *)"' expected",current);
        AV();
        ReadAttName((char *)elem.name);
        AddNode( OPER_XATTR , elem , node , NULL , NULL);
        if (EC!='\'') SetError((char *)"' expected",current);
        AV();
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

      // -------------------------------------------------------
      // Constants
      // -------------------------------------------------------

    case 'P':
    case 'p': if ( Match("pi") ) {
        AV(2);
        elem.constant=3.14159265358979323846;
        AddNode( OPER_DOUBLE , elem , node , NULL , NULL);
      } else if ( Match("pow(") ) {
        AV(4);
        ReadExpression(&l_t);
        if (EC!=',') SetError((char *)", expected",current);
        AV();
        ReadExpression(&r_t);
        AddNode( OPER_POW , elem , node , l_t , r_t);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    case 'M':
    case 'm': if ( Match("mod(") ) {
        AV(4);
        ReadExpression(&l_t);
        if (EC!=',') SetError((char *)", expected",current);
        AV();
        ReadExpression(&r_t);
        AddNode( OPER_MOD , elem , node , l_t , r_t);
        if (EC!=')') SetError((char *)") expected",current);
        AV();
      } else {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      }
      break;

    default: if( (EC>='A' && EC<='Z') ||
                 (EC>='a' && EC<='z') ||
                 (EC=='_'))
      {
        ReadName((char *)elem.name);
        AddNode( OPER_NAME , elem , node , NULL , NULL);
      } else
        SetError((char *)"Syntax error",current);
  }

}

// -------------------------------------------------------

void ExpParser::ReadExpression(ETREE **node) {

  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  ReadFactor1(&l_t);
  if (EC=='&')
  {
    AV();
    ReadFactor1(&r_t);
    AddNode( OPER_AND , elem , node , l_t , r_t );
  } else if (EC=='|') {
    AV();
    ReadFactor1(&r_t);
    AddNode( OPER_OR , elem , node , l_t , r_t );
  } else if (EC=='^') {
    AV();
    ReadFactor1(&r_t);
    AddNode( OPER_XOR , elem , node , l_t , r_t );
  } else {
    *node=l_t;
  }

}

// -------------------------------------------------------

void ExpParser::ReadFactor1(ETREE **node) {

  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  ReadFactor2(&l_t);
  if ((EC=='<') && Match("<=") )
  {
    AV(2);
    ReadFactor2(&r_t);
    AddNode( OPER_LWEQ , elem , node , l_t , r_t );
  } else if ((EC=='<') && !Match("<=") && !Match("<<") ) {
    AV();
    ReadFactor2(&r_t);
    AddNode( OPER_LW , elem , node , l_t , r_t );
  } else if ((EC=='>') && Match(">=") ) {
    AV(2);
    ReadFactor2(&r_t);
    AddNode( OPER_GTEQ , elem , node , l_t , r_t );
  } else if ((EC=='>') && !Match(">=") && !Match(">>") ) {
    AV();
    ReadFactor2(&r_t);
    AddNode( OPER_GT , elem , node , l_t , r_t );
 } else if ((EC=='=') && Match("==") ) {
    AV(2);
    ReadFactor2(&r_t);
    AddNode( OPER_EQ , elem , node , l_t , r_t );
  } else if ((EC=='!') && Match("!=") ) {
    AV(2);
    ReadFactor2(&r_t);
    AddNode( OPER_NEQ , elem , node , l_t , r_t );
  } else {
    *node=l_t;
  }

}

// -------------------------------------------------------

void ExpParser::ReadFactor2(ETREE **node) {

  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  ReadFactor3(&l_t);
  if ((EC=='<') && Match("<<"))
  {
    AV(2);
    ReadFactor3(&r_t);
    AddNode( OPER_LSHIFT , elem , node , l_t , r_t );
  } else if ( (EC=='>') && Match(">>") ) {
    AV(2);
    ReadFactor3(&r_t);
    AddNode( OPER_RSHIFT , elem , node , l_t , r_t );
  } else {
    *node=l_t;
  }

}

// -------------------------------------------------------

void ExpParser::ReadFactor3(ETREE **node)
{
  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  ReadFactor4(&l_t);

  while(EC=='+' || EC=='-')
  {
    switch(EC) {
    case '+': AV();
              ReadFactor4(&r_t);
              AddNode( OPER_PLUS , elem , &l_t , l_t , r_t );
              break;

    case '-': AV();
              ReadFactor4(&r_t);
              AddNode( OPER_MINUS , elem , &l_t , l_t , r_t );
              break;
    }
  }
  *node=l_t;

}

// -------------------------------------------------------

void ExpParser::ReadFactor4(ETREE **node)
{
  ETREE *l_t=NULL;
  ETREE *r_t=NULL;
  ETREE_NODE elem;
  memset(&elem,0,sizeof(ETREE_NODE));

  ReadTerm(&l_t);

  while(EC=='*' || EC=='/')
  {
    switch(EC) {
    case '*': AV();
              ReadTerm(&r_t);
              AddNode( OPER_MUL , elem , &l_t , l_t , r_t );
              break;

    case '/': AV();
              ReadTerm(&r_t);
              AddNode( OPER_DIV , elem , &l_t , l_t , r_t );
              break;
    }
  }
  *node=l_t;

}

// -------------------------------------------------------

void ExpParser::ReadType() {

  char tName[256];
  ReadName(tName);

  if( strcasecmp(tName,"DevBoolean")==0 ) {	    	  	
    isSpectrum = false;
    type = Tango::DEV_BOOLEAN;
  } else if ( strcasecmp(tName,"DevShort")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_SHORT;
  } else if ( strcasecmp(tName,"DevUShort")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_USHORT;
  } else if ( strcasecmp(tName,"DevDouble")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_DOUBLE;
  } else if ( strcasecmp(tName,"DevFloat")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_FLOAT;
  } else if ( strcasecmp(tName,"DevLong")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_LONG;
  } else if ( strcasecmp(tName,"DevULong")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_ULONG;
  } else if ( strcasecmp(tName,"DevVarDoubleArray")==0 ) {
    isSpectrum = true;
    type = Tango::DEV_DOUBLE;
  } else if ( strcasecmp(tName,"DevString")==0 ) {
    isSpectrum = false;
    type = Tango::DEV_STRING;
  } else {
    char tmpErr[128];
    sprintf(tmpErr,"Type %s not suported",tName);
    SetError(tmpErr);
  }


}

// -------------------------------------------------------

int ExpParser::ReadCommandName() {

  int cmd = 0; // 0=Default, 1=Holding, 2=Input

  if(Match("HOLDING")) {
    AV(7);
    cmd=1;
    if (EC!=',') SetError((char *)", expected",current);
    AV();
  } else if(Match("INPUT")) {
    AV(5);
    cmd=2;
    if (EC!=',') SetError((char *)", expected",current);
    AV();
  }

  return cmd;

}

// -------------------------------------------------------

const string TG_STATE[] = { "ON", "OFF", "CLOSE", "OPEN", "INSERT", "EXTRACT", "MOVING", "STANDBY", "FAULT",
                            "INIT", "RUNNING", "ALARM", "DISABLE", "UNKNOWN" };

void ExpParser::ReadState() {

  char tName[256];
  ReadName(tName);

  bool found = false;
  int nbState = (int)( std::end(TG_STATE) - std::begin(TG_STATE) );
  int i = 0;
  
  while( !found && i<nbState ) {
    found = strcasecmp(TG_STATE[i].c_str(),tName)==0;
    if(!found) i++;
  }

  if(!found) {
    state = Tango::UNKNOWN;
    char tmpErr[128];
    sprintf(tmpErr,"Unknown Tango state %s",tName);
    SetError(tmpErr);
  } else {
    state = (Tango::DevState)i;
  }

}

// -------------------------------------------------------

void ExpParser::ReadWriteFn() {

  char fName[256];
  ReadName(fName);

  if( strcasecmp(fName,"WriteInt")==0 ) {
    regWriteType = REG_INT;
  } else if ( strcasecmp(fName,"WriteUInt")==0 ) {
    regWriteType = REG_UINT; 	  	
  } else if ( strcasecmp(fName,"WriteLong")==0 ) {
    regWriteType = REG_LONG; 	  	
  } else if ( strcasecmp(fName,"WriteLongLSB")==0 ) {
    regWriteType = REG_LONGLSB; 	  	
  } else if ( strcasecmp(fName,"WriteULong")==0 ) {
    regWriteType = REG_ULONG; 	  	
  } else if ( strcasecmp(fName,"WriteULongLSB")==0 ) {
    regWriteType = REG_ULONGLSB; 	  	
  } else if ( strcasecmp(fName,"WriteFloat")==0 ) {
    regWriteType = REG_FLOAT; 	  	
  } else if ( strcasecmp(fName,"WriteDouble")==0 ) {
    regWriteType = REG_DOUBLE; 	  	
  } else if ( strcasecmp(fName,"WriteCoil")==0 ) {
    regWriteType = REG_COIL; 	  	
  } else if ( strcasecmp(fName,"WriteCoils")==0 ) {
    regWriteType = REG_COILS;
  } else if ( strcasecmp(fName,"WriteBit")==0 ) {
    regWriteType = REG_BIT; 	  	
  } else {
    char tmpErr[128];
    sprintf(tmpErr,"Write function %s not suported",fName);
    SetError(tmpErr);
  }

}

// -------------------------------------------------------

void ExpParser::ParseState()
{

  exprLgth = strlen(expr);
  if( exprLgth==0 )
    SetError((char *)"Empty expression");

  current=0;
  EC=expr[0];

  safe_free_tree(&evalTree);
  safe_free_tree(&writeTree);
  
  ReadState();
  if(EC!='=') SetError((char *)"= expected",current);AV();
  ReadExpression(&evalTree);

  if(EC==',') {
    // We have a status message
    AV();
    while(current<exprLgth) {
      stradd(status,expr[current]);
      current++;
    }
  }
  
  if(current!=exprLgth)
    SetError((char *)"Syntax error",current);

}

// -------------------------------------------------------

void ExpParser::ParseStatus() {

  exprLgth = strlen(expr);
  if (exprLgth == 0)
    SetError((char *) "Empty expression");

  current = 0;
  EC = expr[0];

  safe_free_tree(&evalTree);
  safe_free_tree(&writeTree);

  ReadExpression(&evalTree);

  if (EC != ',') SetError((char *) ", expected", current);
  AV();

  // Status message
  while (current < exprLgth) {
    stradd(status, expr[current]);
    current++;
  }

  if (current != exprLgth)
    SetError((char *) "Syntax error", current);

}

// -------------------------------------------------------
void ExpParser::ReadWriteDefinition() {

  ReadWriteFn();
  if(EC!='(') SetError((char *)"( expected",current);AV();
  if( regWriteType==REG_COILS ) {

    if(EC!='{') SetError((char *)"{ expected",current);AV();
    do {
      if(EC==',') AV();
      int c;
      ReadInteger(&c);
      coilList.push_back(c);
    } while(EC==',');
    if(EC!='}') SetError((char *)"{ expected",current);AV();
    if(EC!=',') SetError((char *) ", expected", current);AV();

  } else {

    ReadInteger(&writeAddress);
    if (EC != ',') SetError((char *) ", expected", current);
    AV();
    if (regWriteType == REG_BIT) {
      ReadInteger(&writeBitIndex);
      if (EC != ',') SetError((char *) ", expected", current);
      AV();
    }

  }
  ReadExpression(&writeTree);
  if(EC!=')') SetError((char *)") expected",current);AV();

}

// -------------------------------------------------------

void ExpParser::ParseCommand()
{

  exprLgth = strlen(expr);
  if( exprLgth==0 )
    SetError((char*)"Empty expression");

  current=0;
  EC=expr[0];

  safe_free_tree(&evalTree);
  safe_free_tree(&writeTree);
  coilList.clear();

  ReadName(name);
  if(EC!='=') SetError((char *)"= expected",current);AV();
  ReadWriteDefinition();
  if(current!=exprLgth)
    SetError((char *)"Syntax error",current);

}

// -------------------------------------------------------

void ExpParser::ParseAttribute()
{

  exprLgth = strlen(expr);
  if( exprLgth==0 )
    SetError((char*)"Empty expression");

  current=0;
  EC=expr[0];

  safe_free_tree(&evalTree);
  safe_free_tree(&writeTree);
  
  ReadName(name);
  if(EC!='=') SetError((char *)"= expected",current);AV();
  ReadType();
  if(EC!='(') SetError((char *)"( expected",current);AV();

  if( type==Tango::DEV_STRING ) {

    // String attribute
    ReadInteger(&stringStartReg);
    if(EC!=',') SetError((char *)", expected",current);AV();
    ReadInteger(&stringLength);
    strValue = (char *)malloc(stringLength+1);

  } else {

    // Numerical attribute
    ReadExpression(&evalTree);

    if (EC == ',') {
      // We have a write definition
      AV();
      ReadWriteDefinition();
    }

    if (EC == ',') {
      // We have a memorized definition
      AV();
      if(Match("MEM")) {
        AV(3);
        isMEM = 1;
      } else {
        SetError((char *)"MEM expected",current);
      }
    }

  }

  if (EC!=')') SetError((char *)") expected",current);AV();
  
  if(current!=exprLgth)
    SetError((char *)"Syntax error",current);

}

/*
void printBin(double *add) {

  unsigned char *p = (unsigned char *)add;
  printf("0x");
  for(int i=7;i>=0;i--)
    printf("%02X",*(p+i));

  unsigned int p7 = (int)(*(p+7));
  unsigned int p6 = (int)(*(p+6));
  unsigned int ph = (p7<<8) + p6;

  int e = ((ph & 0x7FF0)>>4) - 1023;
  printf("(e=%d)",e);

}
*/

// -------------------------------------------------------

void ExpParser::FloatToRegisters(float f,unsigned short *r1,unsigned short *r2) {

  unsigned char *ptr = (unsigned char *)&f;
  *r1  = ptr[0] & 0xFF;
  *r1 |= ptr[1] << 8;
  *r2  = ptr[2] & 0xFF;
  *r2 |= ptr[3] << 8;

}

// -------------------------------------------------------

void ExpParser::DoubleToRegisters(double f,unsigned short *r1,unsigned short *r2,unsigned short *r3,unsigned short *r4) {

  unsigned char *ptr = (unsigned char *)&f;
  *r1  = ptr[0] & 0xFF;
  *r1 |= ptr[1] << 8;
  *r2  = ptr[2] & 0xFF;
  *r2 |= ptr[3] << 8;
  *r3  = ptr[4] & 0xFF;
  *r3 |= ptr[5] << 8;
  *r4  = ptr[6] & 0xFF;
  *r4 |= ptr[7] << 8;

}

// -------------------------------------------------------

VALUE ExpParser::EvalTree(ETREE *t) {

  VALUE a,b;
  if(t->left)  a=EvalTree(t->left);
  if(t->right) b=EvalTree(t->right);
  return t->calc_fn(this,&(t->info),&a,&b);

}

// -------------------------------------------------------

vector<short> ExpParser::ReadModbusReg(int cmd, int address , int length ) {

  return parent->regs(cmd, address,length);

}

// -------------------------------------------------------

short ExpParser::ReadModbusReg(int cmd, int address ) {

  return parent->reg(cmd,address);

}

// -------------------------------------------------------

vector<short> ExpParser::ReadModbusCoil( int address , int length ) {

  return parent->coils(address,length);

}

// -------------------------------------------------------

short ExpParser::ReadModbusCoil( int address ) {

  return parent->coil(address);

}

// -------------------------------------------------------

double ExpParser::ReadAttribute(char *attName) {
  
  return parent->read_self_attribute(attName);

}

// -------------------------------------------------------

Tango::DeviceProxy *ExpParser::Import(string devName) {

  bool found = false;
  int i = 0;

  while( !found && i<(int)devices.size() ) {
    found = devName.compare(devices[i].devName)==0;
    if(!found) i++;
  }

  if(!found) {
    DEV_ITEM it;
    it.ds = new Tango::DeviceProxy(devName);
    it.devName = devName;
    devices.push_back(it);
    return it.ds;    
  } else {
    return devices[i].ds;
  }

}

// -------------------------------------------------------

Tango::DevULong ExpParser::GetIntegerValue(double value) {

  Tango::DevULong v = UI(value);
  return v;

}

// -------------------------------------------------------

double ExpParser::ReadExternAttribute(char *fullAttName) {

  char devNameStr[128];
  strcpy(devNameStr,fullAttName);
  char *attNameStr = strrchr(devNameStr,'/');
  if( attNameStr==NULL ) SetError((char *)"Malformed device name");
  *attNameStr=0;
  attNameStr++;

  string devName = string(devNameStr);
  string attName = string(attNameStr);

  Tango::DeviceProxy *ds = Import(devName);
  Tango::DeviceAttribute da = ds->read_attribute(attName);

  switch(da.get_type()) {

    case Tango::DEV_BOOLEAN:
    {
      Tango::DevBoolean v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_SHORT:
    {
      Tango::DevShort v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_USHORT:
    {
      Tango::DevUShort v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_LONG:
    {
      Tango::DevLong v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_ULONG:
    {
      Tango::DevULong v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_DOUBLE:
    {
      Tango::DevDouble v;
      da >> v;
      return v;
    }
    break;   
    case Tango::DEV_FLOAT:
    {
      Tango::DevFloat v;
      da >> v;
      return (double)v;
    }
    break;   
    default:
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_read",
         (const char *)"Cannot read attribute (type not supported)",
         (const char *)"ExpParser::ReadExternAttribute");

  }

  return 0.0;

}

// -------------------------------------------------------

int ExpParser::GetCurrentPos() {
  return current;
}

// -------------------------------------------------------

double ExpParser::GetWriteValue() {

  return writeVALUE;

}
// -------------------------------------------------------


void ExpParser::EvaluateString() {

  int nbReg = stringLength / 2 + stringLength % 2;
  int idx = 0;
  vector<short> regs = ReadModbusReg(0,stringStartReg, nbReg);

  for(int i=0;i<nbReg;i++) {
    if(idx<stringLength) strValue[idx++] = (regs[i] >> 8) & 0xFF;
    if(idx<stringLength) strValue[idx++] = (regs[i] & 0xFF );
  }
  strValue[idx] = 0;

}

// -------------------------------------------------------

void ExpParser::EvaluateWrite(double wValue) {

  writeVALUE = wValue;
  VALUE result;
  errno=0;

  /* Evaluate expression */
  
  if(writeTree==NULL) SetError((char *)"Write evaluation expression not initialised");
  result=EvalTree(writeTree);

  /* Now write */
  vector<short> input;
  unsigned short r1,r2,r3,r4;
  int v;
  unsigned int uv;

  switch(regWriteType) {

    case REG_INT:
      parent->write_reg(writeAddress,S(result.value[0]));
      break;

    case REG_UINT:
      uv = UI(result.value[0]);
      r1 = (unsigned short)uv;
      parent->write_reg(writeAddress,r1);      
      break;

    case REG_LONG:
      v = I(result.value[0]);
      uv = (unsigned int)v;
      r1 = (unsigned short)( uv >> 16 );
      r2 = (unsigned short)( uv & 0xFFFF );
      input.push_back(r1);
      input.push_back(r2);
      parent->write_regs(writeAddress,input);
      break;

    case REG_LONGLSB:
      v = I(result.value[0]);
      uv = (unsigned int)v;
      r1 = (unsigned short)( uv >> 16 );
      r2 = (unsigned short)( uv & 0xFFFF );
      input.push_back(r2);
      input.push_back(r1);
      parent->write_regs(writeAddress,input);
      break;

    case REG_ULONG:
      uv = UI(result.value[0]);
      r1 = (unsigned short)( uv >> 16 );
      r2 = (unsigned short)( uv & 0xFFFF );
      input.push_back(r1);
      input.push_back(r2);
      parent->write_regs(writeAddress,input);
      break;

    case REG_ULONGLSB:
      uv = UI(result.value[0]);
      r1 = (unsigned short)( uv >> 16 );
      r2 = (unsigned short)( uv & 0xFFFF );
      input.push_back(r2);
      input.push_back(r1);
      parent->write_regs(writeAddress,input);
      break;

    case REG_FLOAT:
      FloatToRegisters((float)result.value[0],&r1,&r2);
      input.push_back(r1);
      input.push_back(r2);
      parent->write_regs(writeAddress,input);
      break;

    case REG_DOUBLE:
      DoubleToRegisters(result.value[0],&r1,&r2,&r3,&r4);
      input.push_back(r1);
      input.push_back(r2);
      input.push_back(r3);
      input.push_back(r4);
      parent->write_regs(writeAddress,input);
      break;

    case REG_COIL:
      parent->write_coil(writeAddress,US(result.value[0]));
      break;

    case REG_COILS: {
        if(result.lgth==1) {
          for (int i = 0; i < (int) coilList.size(); i++)
            parent->write_coil(coilList[i], US(result.value[0]));
        } else if((int)coilList.size()==result.lgth) {
          for (int i = 0; i < (int) coilList.size(); i++)
            parent->write_coil(coilList[i], US(result.value[i]));
        } else {
          SetError((char *)"WriteCoils: invalid value size");
        }
      }
      break;

    case REG_BIT:
      parent->write_bit(writeAddress,writeBitIndex,US(result.value[0]));
      break;
  }

}

// -------------------------------------------------------

bool ExpParser::GetBoolResult(VALUE r) {

  unsigned int n = UI(r.value[0]);
  return n!=0;

}

// -------------------------------------------------------

bool ExpParser::HasWriteExpression() {
  return writeTree!=NULL;
}

// -------------------------------------------------------
bool ExpParser::IsMemorized() {
  return isMEM!=0;
}

// -------------------------------------------------------

bool ExpParser::IsSpectrum() {
  return isSpectrum;
}

// -------------------------------------------------------

Tango::DevState ExpParser::GetState() {
  return state;
}

// -------------------------------------------------------

char *ExpParser::GetStatus() {
  return status;
}

// -------------------------------------------------------

void ExpParser::EvaluateRead(VALUE *result) {
  errno=0;
  if(evalTree==NULL) SetError((char *)"Read evaluation expression not initialised");
  *result=EvalTree(evalTree);
}

} // namespace ModbusComposer_ns

