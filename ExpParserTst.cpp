#include <ExpParser.h>
#include <iostream>

using namespace std;

Tango::DeviceProxy *modbusDS;

double NaN = sqrt(-1); 

short Reg(int i) {

  if(modbusDS==NULL)
    return -1;

  Tango::DeviceData argout;
  Tango::DeviceData argin;
  vector<short> input;
  vector<short> output;
          
  input.push_back(i);
  input.push_back(1);
  argin << input;
  argout = modbusDS->command_inout("ReadHoldingRegisters",argin);
  argout >> output;
  return output[0];

}

int Flag(int i,int k) {
  unsigned int mask = 1;
  mask = mask << k;
  return ( ((unsigned int)Reg(i)&mask)!=0 );
}

void Test(ExpParser *ep,string EXP,double expected) {

  VALUE r;

  char *E = (char *)EXP.c_str();
  ep->SetExpression(E);
  try {

    ep->Parse();
    ep->EvaluateRead(&r);
    bool test;
    int n;
    switch(ep->GetType()) {
      case Tango::DEV_BOOLEAN:
        test = ep->GetBoolResult(r);
        cout << "\"" << E << "\"=" << test << " (expected: " << expected << ")" << endl;
        break;
      case Tango::DEV_SHORT:
      case Tango::DEV_LONG:
        n = (int)( r.value[0]+0.5 );
        cout << "\"" << E << "\"=" << n << " (expected: " << expected << ")" << endl;
        break;
      case Tango::DEV_DOUBLE:
        for(int i=0;i<r.lgth;i++)
        cout << "\"" << E << "\"=" << r.value[i] << " (expected: " << expected << ")" << endl;
        break;
    }

  } catch(Tango::DevFailed &e) {

     cerr << "\"" << E << "\": Error: " << e.errors[0].desc << endl;

  }

}

void TestState(ExpParser *ep,string EXP) {

  VALUE r;

  char *E = (char *)EXP.c_str();
  ep->SetExpression(E);

  try {

    ep->ParseState();
    ep->EvaluateRead(&r);
    bool test;
    test = ep->GetBoolResult(r);
    cout << "\"" << E << "\"=" << test << " : " << ep->GetStatus() << endl;

  } catch(Tango::DevFailed &e) {

     cerr << "\"" << E << "\": Error: " << e.errors[0].desc << endl;

  }

}

int main(int argc,char* argv[]) {

  try {
    modbusDS = new Tango::DeviceProxy("sr/ps-qf7hg/test-modbus");
  } catch( Tango::DevFailed &e ) {
    cerr << "Import failed:" << e.errors[0].desc << endl;
  }

  ExpParser *ep = new ExpParser(modbusDS,NULL);

  Test(ep,"toto=DevDouble(1.6*2.0/5.0)",1.6*2.0/5.0);
  Test(ep,"toto=DevDouble((4.5+6.7)*(4.5+2)/(37+4))",(4.5+6.7)*(4.5+2)/(37+4));
  Test(ep,"toto=DevShort(freg(15))",NaN);
  Test(ep,"toto=DevDouble(freg(20))",NaN);
  Test(ep,"toto=DevDouble(fregs(7,2))",NaN);
  Test(ep,"toto=DevShort(Flag(15,15) << 1 | Flag(15,14) )",Flag(15,15) << 1 | Flag(15,14));
  Test(ep,"toto=DevLong(2+2-4)",2+2-4);
  Test(ep,"toto=DevLong(2+2*3)",2+2*3);
  Test(ep,"toto=DevLong(2 << 2)",2 << 2);
  Test(ep,"toto=DevLong(2+2 << 2)",2+2 << 2);
  Test(ep,"toto=DevLong(0x0110 & 0x0100 >> 8)",0x0110 & 0x0100 >> 8);
  Test(ep,"toto=DevLong((0x0110 & 0x0100) >> 8)",(0x0110 & 0x0100) >> 8);
  Test(ep,"toto=DevLong(1 << 2+2)",1 << 2+2);
  Test(ep,"toto=DevLong(0x1234 | 0x0FF0)",0x1234 | 0x0FF0);
  Test(ep,"toto=DevLong(0x1234 & 0x00F0 >> 4)",0x1234 & 0x00F0 >> 4);
  Test(ep,"toto=DevBoolean(1.23>2.5)",1.23>2.5);
  Test(ep,"toto=DevBoolean(1.23<2.5)",1.23<2.5);
  Test(ep,"toto=DevBoolean((0x1050>>4)==0x105)",(0x1050>>4)==0x105);
  Test(ep,"toto=DevBoolean((2+2-4)==0)",(2+2-4)==0);
  Test(ep,"toto=DevBoolean((2+3-6)==-1)",(2+3-4)==1);
  Test(ep,"toto=DevBoolean(not Flag(15,15))",0);
  Test(ep,"toto=DevBoolean(Flag(15,15)!=1)",0);
  Test(ep,"toto=DevBoolean(1.5!=1.5)",0);
  Test(ep,"toto=DevBoolean(1.5!=1.4)",0);
  Test(ep,"toto=DevDouble(freg(20),WriteFloat(12,10.0*VALUE))",NaN);
  TestState(ep,"ON=2>1");
  TestState(ep,"ALARM=freg(20)>1.5");
  TestState(ep,"FAULT=Flag(15,15),Fault on reg 15");
  TestState(ep,"STT=Flag(15,15)");

  return 0;

}
