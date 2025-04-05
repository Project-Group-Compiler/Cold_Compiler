1. we allow same name of class and struct. c++ doesnt bcoz it has functions in struct. but since we don't it wont affect our name mangling.

2. pointer as function arg is working fine ig(atleast int*).

3. don't do this : class person *ptr, p; (if u have to declare, use separate lines like class person *ptr; class person p;)

4. Avoid assigning values to class fields in the class definition. (like int age = 89;). No error but TAC is slightly wrong so avoid.

5. struct and union with same name not allowed. (but class and struct of same name allowed.)

6. no error produced for these 2 (which is ok, nice error handling has been done otherwise): 
    
```cpp
    union Data
    {
        int i;
        char ch;
    };
    void solve()
    {
        struct Data data;
    }
```

and 

```cpp
    struct Data
    {
        int i;
        char ch;
    };

    void test_union()
    {
        union Data data; // not allowed different types
        return;
    }
```

7. static vars -> local scope and global scope both allowed. static can be int, char or float. CAUTION : Please don't initialize float static with an int constant (no error but wrong code), please initialize it with float constant.

8. We don't allow const & static in struct and classes for now -> their behaviour can be unexpected

9. Don't do printf("%d\n",add(2)); ->not handled yet