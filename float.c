float x = 5.7;

int main()
{
	// float y = -4.3; // negative float is buggy
	int d = 4.5, e = 3;
	float y = 4;
	d = x;
	y = d;
	d = y; 
	d = y + x;
	d = y;
	d = e;
	e = d+2;
	e = y+ 6 ;

	printf("d : %d\n", d);
	printf("e : %d\n", e);
	printf("f : %f\n", y);
}
