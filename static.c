void increment()
{
  static int count = 4;
  count += 9;
}

static int count = 5;

int main()
{
  increment();
  increment();
  increment();
  count++;
  return 0;
}
