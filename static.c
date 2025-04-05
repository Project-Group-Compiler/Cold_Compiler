int count = 5;
int glo2 = 7;
static int glo = 9;

void increment()
{
  int x = 8;
  static int count = 4;
  count += 9;
}

int glo4 = 7;

int main()
{
  static int x = 8;
  static int count = 8;
}

int glo3 = 7;