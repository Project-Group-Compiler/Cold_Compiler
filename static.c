static int glo = 7;
static int glo2;

void increment()
{
  static int count = 4;
  static int total;
  count++;
  total += count;
}

int main()
{
  glo = glo + 5;
  glo2 = glo + 5;
  increment();
  increment();
  return 0;
}
