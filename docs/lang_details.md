## Some implementation details regarding Cold language and its compiler

- Object-oriented syntax :

    - Declaring a class :

        ```markdown
        class <class_name>
        {
            ...fields and methods...
        };
        ```

    - All members must be declared along with their access specifier (which can be `public`, `private` or `protected`).

        ```markdown
        public{
            int age;
        };
        ```

    - Inheritance specified during class declaration using `<-` operator along with method of inheritance. Multi-level and multiple inheritance syntax is supported.

        ```markdown
        class Student <- public Person{
            ...members...
        };
        ```

    - Constructors are supported but destructors are not.
  
        ```markdown
        public{
            Student(int x) {
                this->age = x;
            }
        };
        ```

    - For declaring objects of a class, specify as `class <class_name> <object_name>` and access members using `.` and `->` operators.

        ```markdown
        class Student s1;
        s1.study();
        ```

- Empty struct in C is undefined behaviour. Our compiler chooses to throw a syntax error on an empty struct. That is, only non-empty structs can be declared.
- C++ style comments ( starting with `//` are supported )
