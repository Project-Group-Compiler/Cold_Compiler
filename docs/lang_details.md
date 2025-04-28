## Some implementation details regarding Cold language and its compiler


- Supported basic data types : `int`, `float` and `char`.
- Object-oriented syntax :

    - Declaring a class :

        ```cpp
        class <class_name>
        {
            ...fields and methods...
        };
        ```

    - All members must be declared inside some access specifier (which can be `public`, `private` or `protected`).

        ```cpp
        public{
            int age;
            
            int getAge(){

            }
        };
        ```

    - Inheritance specified during class declaration using `<-` operator. Multi-level inheritance is supported. Inheritance mode is always public.

        ```cpp
        class Student <- Person{
            ...members...
        };
        ```

    - For declaring objects of a class, specify as `class <class_name> <object_name>` and access members using `.` or `->` operator.

        ```cpp
        class Student s1;
        class Student *ptr;
        s1.study();
        (*ptr).study();
        ptr->study();
        // all three are OK
        ```

    - Any member inside a class method needs to be accessed using `this` pointer. 

        Example :
        
        ```cpp
        class Person { 
            private{
                int age;
            }; 
            public{ 
                int getAge()  
                { 
                    return this->age;
                } 
            };
        }; 
        ```

- Function overloading is being supported through name mangling.
- Following C library functions are supported by default (no header files need to included) :

    `scanf`, `printf`, `malloc`, `calloc`, `free`, `fopen`, `fputs`, `fgets`, `fclose`, `fprintf`, `fscanf`, `fgetc`, `fputc`, `fread`, `fwrite`, `fseek`, `ftell`, `rewind`, `strlen`, `strcmp`, `strncmp`, `strcpy`, `strcat`, `memcpy`, `memset`, `atoi`, `atof`, `fabs`, `sin`, `cos`, `tan`, `exp`, `log`, `log10`, `pow`, `sqrt`, `ceil`, `floor`, `round`, `fmod`, `fmax`, `fmin`, `va_start`, `va_arg`, `va_end`

- typedef works only for unqualified types (that is, works for int but not const int). Also typedef for struct is supported but keyword 'struct' needs to be written with the type alias as well. Example in tests/asm_gen/typedef.cold
- Compound initialization of arrays is supported. For example, `int a[3] = {1, 2, 3};`.
- Global variables (and arrays) can be initialized only using constant values. For example, `int a = 5;` is OK but `int a = b + c;` is not OK.
- No type casting is allowed when passing arguments to functions. Function arguments must match exactly with the parameter types. That is, for `int` parameter only int variable or int constant is allowed. Similarly for float and char.
- Take care to write `return` statement in all functions at the end (including those with void return type).
- `int`, `float` and `char` can be implicitly and explicitly converted into each other.
- Pointer types can only be converted to and from `void*`. Apart from that, a pointer type can't be converted to another pointer or non-pointer type implicitly or explicitly. Neither can a non-pointer type converted to a pointer type implicitly or explicitly.
- `static` keyword should not be used with functions. It can only be used with variables (including arrays).
- Don't mix variable declaration of pointer or array with normal variable declaration. For example, `int a, *b, c;` is not allowed. Instead, declare them separately as `int a, c; int *b;`.
- Empty struct in C is undefined behaviour. Our compiler chooses to throw a syntax error on an empty struct. That is, only non-empty structs can be declared. Similarly, only non-empty classes can be declared.
- C++ style comments ( starting with `//` ) are supported.
- `Cold` does not have a preprocessor, hence don't use preprocessor directives (`#include` etc).

----
