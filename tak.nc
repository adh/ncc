int tak(int x, int y, int z){
  if (x <= y){
    return z;
  } else {
    return tak(tak(x-1, y, z),
               tak(y-1, z, x),
               tak(z-1, x, y));
  }
}

int main(){
    return tak(24, 16, 8);
}