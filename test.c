
struct tester = { int g; }
struct other = {
    tester r = new tester(); 
    tester[] testerArray = [r];
    tester() jr = () {
        tester ff = new tester();
        return ff;
    }
}
other b = new other();
b->jr()->g = 1;


//int() i = () {
//    if (1) {
//        return 1;
//    } else {
//        if (0) {
//            return 0;
//        }
//    }
//}


//int() k = (int v, int() b, int[] r) {
//    return v;
//}
//k(4);



