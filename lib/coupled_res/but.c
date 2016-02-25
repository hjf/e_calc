#include <math.h>

struct coupled_res_but_result = {
    int order;
    double Cu;
    double Lu;
}

/**** Normalised lowpass Prototype calculation *****/

#define NLPC( B, ORDER) ( 2 * sin(((2* (B) -1)/(2*(ORDER_))* M_PI) ) )

/**** capacitor ****/

#define CAP( K, FREQ, Z0) (K/(2*M_PI*FREQ*Z0))

/***** inductor ****/

#define IND( K, FREQ, Z0) ( K*Z0/(2*M_PI*FREQ) )



coupled_res_but_result coupled_res_but_calc(
    double Fc,
    double BW,
    double Z0,
    double L,
    int order
) {

  coupled_res_but_result result = {}
  result.order = order;

  L=L*0.000000001;  //l is aparently in microhenrys
  Fc=Fc*1e6; //freq in mhz
  BW=BW*1e6; //bw in mhz

  double K[10] = {0};
  double Cs[11] = {0};
  double Cp[11] = {0};
  Lrk = (2*Math.PI*Fc*L)/Z0;
  //If to calculate L, then lets calculate to a value of Lrk = unity
  if (document.getElementById("calcL").checked)
  {
    Lrk = 1;
    L = (Lrk*Z0)/(2*M_PI*Fc);
    L = L*1e9;
    L = L.toPrecision(4);
    document.getElementById("L").value=L;
  }

  double w0 = 1.0;
  double w1 = w0-(BW/Fc);
  double w2 = w0+(BW/Fc);
  double f1 = w1/(2*M_PI);
  double f2 = w2/(2*M_PI);
  double f1d = 1/(2*M_PI);
  double f0 = f1+f2- sqrt((Math.pow(f2-f1,2)+(f1*f2));
  double wd = ((f0/f1)-(f0/f2))*(f0/f1d);
  double Crk = 1/(Math.pow(w0,2)*Lrk);
  var Cu = new Array();
  var Lu = new Array();
  STR='';


  for ( int BUTorder=2 ; BUTorder <= order ; BUTorder++ ) {
    for ( int i = 1 ; i <= BUTorder ; i++) {
      K[i]=NLPC( i, BUTorder);
      if (i==1) Cs[1] = (1/w0)*sqrt((wd*Crk/K[1])/(1-(wd*Crk/K[1])));
      else      Cs[i] = wd*sqrt((Crk*Crk)/(K[i-1]*K[i]));
    }
    Cs1d = Cs[1]/(1+Math.pow(w0,2)*Math.pow(Cs[1],2));
    for (i=1;i<BUTorder;i++)
    {
      if (i==1)  Cp[1] = Crk-Cs1d-Cs[2];
      else       Cp[i] = Crk-Cs[i]-Cs[i+1];
    }
    for (i=1;i<=BUTorder;i++)
    {
      Cp[i] = CAP (Cp[i], Fc, Z0);
      Cs[i] = CAP (Cs[i], Fc, Z0);
    }
    if (BUTorder==2) {Cs[3]=Cs[1];                                          Cp[2]=Cp[1];}
    if (BUTorder==3) {Cs[4]=Cs[1]; Cs[3]=Cs[2];                             Cp[3]=Cp[1];}
    if (BUTorder==4) {Cs[5]=Cs[1]; Cs[4]=Cs[2];                             Cp[4]=Cp[1]; Cp[3]=Cp[2];}
    if (BUTorder==5) {Cs[6]=Cs[1]; Cs[5]=Cs[2]; Cs[4]=Cs[3];                Cp[5]=Cp[1]; Cp[4]=Cp[2];}
    if (BUTorder==6) {Cs[7]=Cs[1]; Cs[6]=Cs[2]; Cs[5]=Cs[3];                Cp[6]=Cp[1]; Cp[5]=Cp[2];  Cp[4]=Cp[3];}
    if (BUTorder==7) {Cs[8]=Cs[1]; Cs[7]=Cs[2]; Cs[6]=Cs[3]; Cs[5]=Cs[4];   Cp[7]=Cp[1]; Cp[6]=Cp[2];  Cp[5]=Cp[3];}
  
    STR = STR + BUTorder.toString() + ' Resonator capacitor coupled BPFr';
    for (j=1;j<=BUTorder+1;j++)
    { 
      if      (Cs[j]<1e-9) {Cu='pF';  Cs[j]*=1e12;} 
      else if (Cs[j]<1e-6) {Cu='nF';  Cs[j]*=1e9;}  
      else if (Cs[j]<1e-3) {Cu='uF';  Cs[j]*=1e6;}  
      else if (Cs[j]<1)    {Cu='mF';  Cs[j]*=1e3;}
      Cs[j] = Cs[j].toFixed(3);
      STR = STR + 'Cs' + j.toString() + ': ' + Cs[j].toString() + Cu + 'r';
    }
    for (j=1;j<=BUTorder;j++)
    { 
      if      (Cp[j]<1e-9) {Cu='pF';  Cp[j]*=1e12;} 
      else if (Cp[j]<1e-6) {Cu='nF';  Cp[j]*=1e9;}  
      else if (Cp[j]<1e-3) {Cu='uF';  Cp[j]*=1e6;}  
      else if (Cp[j]<1)    {Cu='mF';  Cp[j]*=1e3;}
      Cp[j] = Cp[j].toFixed(3);
      STR = STR + 'Cp' + j.toString() + ': ' + Cp[j].toString() + Cu + 'r';
    }
    STR = STR + '---------------------------------r';
  }
  document.getElementById("FilterValues").value=STR;
}
</script>
<div id="content"> 
<h2>Capacitor Coupled Resonator Designer (Butterworth)</h2>
Calculate the C values needed for a capacitor-coupled-resonator BFP.<br />
Enter the center, bandwidth and system impedance. Fix or calculate L
<p></p>
<!-- This is the Boxed implementation of the RF Tool -->
<table class="javaBOX">
  <tr><td align="center" colspan="3"><img src="/images/c_coupled_resonator.png" width="375" height="100" /></td></tr>
  <tr>
    <td valign="top" width="40%">Center Frequency<br />
      <input type="text" class="indat" id="FREQ" value="468" />MHz<br /><br />
      Bandwidth<br />
      <input type="text" class="indat" id="BW" value="20" />MHz<br />
    </td>
    <td valign="top" width="30%">
      Zo<br />
      <input type="text" class="indat" id="Z0" value="50" />&#937;<br /><br />
      L<br />
      <input type="text" class="indat" id="L" value="8.0" />nH<br />
    </td>
    <td align="left" valign="top" width="30%">
      <br /><input type="button" id="B1" value="Calculate" onclick="calculate()" /><br /><br />
      <input type="radio" name="Lcalc" id="calcL" onclick="calculate()" />Calculate L<br />
      <input type="radio" name="Lcalc" id="specL" checked="checked" onclick="calculate()" />Specify L<br />
    </td>
  </tr>
  <tr><td colspan="3"><hr /></td></tr>
  <tr>
    <td colspan="3">
      <textarea rows="12" cols="46" id="FilterValues"></textarea>
    </td>
  </tr>
</table>
<br />Notes:<br />
*Calculator derived from 1957 paper:<br />
Direct-Coupled-Resonator Filters (Seymour B. Cohn)<br /><br />
</div>

<!-- This is the Javascript implementation of the RF tool -->
<script type="text/javascript">
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
