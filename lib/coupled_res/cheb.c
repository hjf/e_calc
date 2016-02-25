#include <math.h>



function calculateC()
{
  var FcC=parseFloat(document.getElementById("FREQC").value);
  var BWC=parseFloat(document.getElementById("BWC").value);
  var Z0C=parseFloat(document.getElementById("Z0C").value);
  var LC=parseFloat(document.getElementById("LC").value);
  var RC=parseFloat(document.getElementById("RC").value);
  var A = new Array(0,0,0,0,0,0,0,0,0,0);
  var B = new Array(0,0,0,0,0,0,0,0,0,0);
  LC=LC*0.000000001;
  FcC=FcC*1e6;
  BWC=BWC*1e6;
  var KC = new Array(0,0,0,0,0,0,0,0,0,0);
  var CsC = new Array(0,0,0,0,0,0,0,0,0,0,0);
  var CpC = new Array(0,0,0,0,0,0,0,0,0,0,0);
  LrkC = (2*Math.PI*FcC*LC)/Z0C;
  //If to calculate L, then lets calculate to a value of Lrk = unity
  if (document.getElementById("calcLC").checked)
  {
    LrkC = 1;
    LC = (LrkC*Z0C)/(2*Math.PI*FcC);
    LC = LC*1e9;
    LC = LC.toPrecision(4);
    document.getElementById("LC").value=LC;
  }
   
  w0C = 1.0;
  w1C = w0C-(BWC/FcC);
  w2C = w0C+(BWC/FcC);
  f1C = w1C/(2*Math.PI);
  f2C = w2C/(2*Math.PI);
  f1dC = 1/(2*Math.PI);
  f0C = f1C+f2C-Math.sqrt(Math.pow(f2C-f1C,2)+(f1C*f2C));
  wdC = ((f0C/f1C)-(f0C/f2C))*(f0C/f1dC);
  CrkC = 1/(Math.pow(w0C,2)*LrkC);
  var CuC = new Array();
  var LuC = new Array();
  STRC='';
  for (BUTorderC=2;BUTorderC<=7;BUTorderC++)
  {
    e = Math.pow(1/(Math.pow(10,(-1*RC)/20)),2)-1;
    b = Math.log(  (((Math.exp(RC/17.37) + Math.exp((-1*RC)/17.37))/2)/((Math.exp(RC/17.37) - Math.exp((-1*RC)/17.37))/2))  );
    b = Math.log(   (Math.exp(2*(RC/17.37))+1)/(Math.exp(2*(RC/17.37))-1)  );
    Y = (Math.exp(b/(2*BUTorderC))-Math.exp((-1*b)/(2*BUTorderC)))/2;
    for (iC=1;iC<=BUTorderC;iC++)
    {
      A[iC]=Math.sin(((2*iC-1)/(2*BUTorderC))*Math.PI);
      B[iC]=(Y*Y)+Math.pow(Math.sin((iC*Math.PI)/BUTorderC),2);
      if (iC==1) KC[1] = (2*A[1])/Y;
      else KC[iC] = (4*A[iC-1]*A[iC])/(B[iC-1]*KC[iC-1]);
    }
    for (iC=1;iC<=BUTorderC;iC++)
    {
      if (iC==1) CsC[1] = (1/w0C)*Math.sqrt((wdC*CrkC/KC[1])/(1-(wdC*CrkC/KC[1])));
      else      CsC[iC] = wdC*Math.sqrt((CrkC*CrkC)/(KC[iC-1]*KC[iC]));
    }
    Cs1dC = CsC[1]/(1+Math.pow(w0C,2)*Math.pow(CsC[1],2));
    for (iC=1;iC<BUTorderC;iC++)
    {
      if (iC==1) CpC[1] = CrkC-Cs1dC-CsC[2];
      else      CpC[iC] = CrkC-CsC[iC]-CsC[iC+1];
    }
    for (iC=1;iC<=BUTorderC;iC++)
    {
      CpC[iC] = CpC[iC]/(2*Math.PI*FcC*Z0C);
      CsC[iC] = CsC[iC]/(2*Math.PI*FcC*Z0C);
    }
    if (BUTorderC==2) {CsC[3]=CsC[1];                                         CpC[2]=CpC[1];}
    if (BUTorderC==3) {CsC[4]=CsC[1]; CsC[3]=CsC[2];                            CpC[3]=CpC[1];}
    if (BUTorderC==4) {CsC[5]=CsC[1]; CsC[4]=CsC[2];                            CpC[4]=CpC[1]; CpC[3]=CpC[2];}
    if (BUTorderC==5) {CsC[6]=CsC[1]; CsC[5]=CsC[2]; CsC[4]=CsC[3];               CpC[5]=CpC[1]; CpC[4]=CpC[2];}
    if (BUTorderC==6) {CsC[7]=CsC[1]; CsC[6]=CsC[2]; CsC[5]=CsC[3];               CpC[6]=CpC[1]; CpC[5]=CpC[2];  CpC[4]=CpC[3];}
    if (BUTorderC==7) {CsC[8]=CsC[1]; CsC[7]=CsC[2]; CsC[6]=CsC[3]; CsC[5]=CsC[4];  CpC[7]=CpC[1]; CpC[6]=CpC[2];  CpC[5]=CpC[3];}
        
    STRC = STRC + BUTorderC.toString() + ' Resonator capacitor coupled BPFr';
    for (jC=1;jC<=BUTorderC+1;jC++)
    { 
      if       (CsC[jC]<1e-9)  {CuC='pF';  CsC[jC]*=1e12;} 
      else if  (CsC[jC]<1e-6)  {CuC='nF';  CsC[jC]*=1e9;}  
      else if  (CsC[jC]<1e-3)  {CuC='uF';  CsC[jC]*=1e6;}  
      else if  (CsC[jC]<1)     {CuC='mF';  CsC[jC]*=1e3;}
      CsC[jC] = CsC[jC].toFixed(3);
      STRC = STRC + 'Cs' + jC.toString() + ': ' + CsC[jC].toString() + CuC + 'r';
    }
    for (jC=1;jC<=BUTorderC;jC++)
    { 
      if       (CpC[jC]<1e-9) {CuC='pF';  CpC[jC]*=1e12;} 
      else if  (CpC[jC]<1e-6) {CuC='nF';  CpC[jC]*=1e9;}  
      else if  (CpC[jC]<1e-3) {CuC='uF';  CpC[jC]*=1e6;}  
      else if  (CpC[jC]<1)    {CuC='mF';  CpC[jC]*=1e3;}
      CpC[jC] = CpC[jC].toFixed(3);
      STRC = STRC + 'Cp' + jC.toString() + ': ' + CpC[jC].toString() + CuC + 'r';
    }
    STRC = STRC + '---------------------------------r';
  }
  document.getElementById("FilterValuesC").value=STRC;
}
