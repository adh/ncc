int foo;
int init_gvar_test(){
  foo = 1337;
}
int gvar_test(){
  return foo == 1337;
}
float float_test(){
  return 1.0;
}
int test_lvar(){
  int foo = 7331;
  float_test();
  if (foo != 7331){
    return 0;
  }
  return 1;
}
int test_loop(){
  int i = 7;
  int j = 0;
  while (i){
    i = i - 1;
    j = j + 1;
  }
  return j == 7;
}
int test_ternary(){
  int to_cons = 13;
  int to_alt = 37;
  return (0?to_cons:to_alt) == 37;
}
int test_short_circuit(){
  
}

int main(){
  init_gvar_test();
  if(!gvar_test()){
    return 1;
  }
  if (1.0 != float_test()){
    return 2;
  }
  if (!test_lvar()){
    return 3;
  }
  if (!test_loop()){
    return 4;
  }
  if (!test_ternary()){
    return 5;
  }
  return 0; /* success */
}
