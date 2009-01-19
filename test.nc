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
  return 0; /* success */
}
