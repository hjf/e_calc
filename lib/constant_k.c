function pr(text) {
   t = ""+t+text+"\r\n";
   document.filter.screen.value=t;
}
function calc() {
   f = document.filter.freq.value;
   rin = document.filter.Rin.value;
   ro = document.filter.Rout.value;
   fac = document.filter.Qf.value;
   if((f =="") || (rin =="") || (ro =="") || (fac =="")) return;
   f = parseFloat(f); rin = parseFloat(rin); ro = parseFloat(ro); 
  qf = parseInt(fac,10);
   xcb = ro;
   xca = rin * Math.sqrt((ro/rin)/(((qf*qf)+1)-(ro/rin)));
   xla = ((qf*ro)+((rin*ro)/xca))/((qf*qf)+1);
   xcc = ro/qf;
   xcd = ro * Math.sqrt(1/(qf*qf));
   xlb = ((qf*ro)+((ro*ro)/xcb))/((qf*qf)+1);
   La = xla/(6.284*f);
    if(La < 1) fa = fa*100;
   La = (parseInt((La*fa)+0.5)/fa) + " uH"; 
  Lb = xlb/(6.284*f);
   if(Lb < 1) fb = fb*100;
   Lb = (parseInt((Lb*fb)+0.5)/fb) + " uH";
   fa = 100; fb = fa;
   with(Math) { 
        Ca = (ceil(m/(6.284*f*xca))) + " pF";
      Cb1 = ceil(m/(6.284*f*xcb));
      Cb2 = ceil(m/(6.284*f*xcc));
      Cc  = (ceil(m/(6.284*f*xcd))) + " pF";
   }
  Cb  = (Cb1 + Cb2) + " pF"; 
  display();
  info = 1;
}
