//int() printArray = (int [] args, int() printFunction) -> {
//   int i = 0;
//   while (i < 3) {
//      print("START");
//      printFunction(args[i]);
//      print("END");
//      i = i + 1;
//   }
//   return 1;
//}
//
//int() printer = (int a) -> {
//   print(a);
//   return 0;
//}
//
//int[] sd = [1,2,3];
//sd[1] = 44 + sd[1];
//
//printArray(sd, printer);
int[] a = [1,2,3];
a[1] = 5;
print(a[1]);