//int i = 44+55;


//int() r = (int a , string c) -> {
//    return 1;
//}
//r(22,"s");
//
//int[] v = [1,2,3];
//v[0] = 1;
//struct other = {
//    tester r = new tester(); 
//    tester[] testerArray = [r];
//    tester() jr = () -> {
//        tester ff = new tester();
//        return ff;
//    }
//}
//other b = new other();
//tester t = new tester();
///b->jr()->g = t->iii;

//int() i = () {
//    if (1) {
//        return 1.99;
//    } else {
//        if (0) {
//            return 0;
//        }
//    }
//}
//int() k = (int v, int() t, int[] r) {
//    return v;
//}
//k(4);

//int() k = (int r, string v) -> { 
//   if (0) {
//      int i = 1;
//      return i;
//   } 
//   int b = 44;
//   return b;
//}

//int() k = (int foe) -> {return foe;}
//
//int() b = (int() i) -> { 
//   if (0) {
//      return 33;
//   } else {
//      print(104);
//   }
//   return i(2); 
//}

struct tester = {
   int s = 3;
}

tester nn = new tester();
tester gg = new tester();  

tester[] sd = [nn, gg];

int i = 0;
while (i < 2) {
   print(sd[i]->s);
   i = i + 1;
}