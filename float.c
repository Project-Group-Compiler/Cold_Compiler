float x = 5.6;

int main()
{
	// float y = -4.3; // what??
	int d = 4.5;

	float y = 4;
	d = y; // if this is last line then things go out of hand
	d = x;
	y = d;

	printf("d : %d\n", d);
	printf("f : %f\n", y);

	// weird errors for this :
	// d = y;
	// d = y;
	// d = e;
	// e = d+2;
	// e = d+ 6 ;
}
