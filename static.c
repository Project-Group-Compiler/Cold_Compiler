const int count=45;
void increment()
{
  const int count = 4;
  int x=5;
  x++;
  x=x+1;
  count += 9;
  count=count*8;
  count=count/2;
  count=count+1;
  x-=5;
  count=count-1;
}


int main()
{
  increment();
  increment();
  increment();
  count++;
  count=count+count;
  count--;
  return 0;
}
