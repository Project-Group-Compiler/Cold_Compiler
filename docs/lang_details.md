## Some implementation details regarding Cold language and its compiler


- Supported data types : `int`, `float` and `char`.
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


- No type casting is allowed when passing arguments to functions. Function arguments must match exactly with the parameter types. That is, for `int` parameter only int variable or int constant is allowed. Similarly for float and char.
- Empty struct in C is undefined behaviour. Our compiler chooses to throw a syntax error on an empty struct. That is, only non-empty structs can be declared.
- C++ style comments ( starting with `//` are supported )

----
