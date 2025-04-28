
int main(){
  float f=3.14;
  float g=++f;
  printf("%f    %f\n",g,f);
  g=--f;
  printf("%f    %f\n",g,f);
  g=f++;
  printf("%f    %f\n",g,f);
  g=f--;
  printf("%f    %f\n",g,f);
  return 0;
}




// 0:  FUNC_4main_v start :
// 1:  f = __f_0		(float)
// 2:  __f_1 = intToFloat(1)
// 3:  _t_1 = f - __f_1		(float)
// 4:  h = _t_1		(float)
// 5:  param h
// 6:  param h
// 7:  param f
// 8:  param __str_0
// 9:  _t_3 = CALL printf 4
// 10:  FUNC_4main_v end
