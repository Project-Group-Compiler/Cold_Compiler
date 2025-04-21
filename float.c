// float x = 5.7;

// int main()
// {
// 	// float y = -4.3; // negative float is buggy
// 	int d = 4.5, e = 3;
// 	float y = 4;
// 	char c='a';


// 	printf("d : %d\n", d);
// 	printf("e : %d\n", e);
// 	printf("f : %f\n", y);
// }



int main()
{
    char c = 'A';
    int i = 'B';
    float f = 0.5;
    i = c;
	printf("c : %c\n", c); //nhi hua print
	printf("i : %d\n", i);
	printf("f : %f\n", f);
	printf("\n");
	c = i+1;
	printf("c : %c\n", c);
	printf("i : %d\n", i);
	printf("f : %f\n", f);
    printf("\n");
	f = c;
	printf("c : %c\n", c);
	printf("i : %d\n", i);
	printf("f : %f\n", f);
    printf("\n");
	c = f+1;
	printf("c : %c\n", c);
	printf("i : %d\n", i);
	printf("f : %f\n", f);
	printf("\n");
  
}
