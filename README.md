impossible
==========

Impossible is a esoteric, stack based, quasi-functional, programming language. The language is pretty obscure but you can have fun with it!

It has an interactive console in which you can execute pieces of code and see the results (or obtain some segmentation faults). Some quick examples:

    i>2
      2 : int

__i>__ is the prompt, what I'm doing here is just pushing a value on the stack.

To add two numbers, for example, just push both of them and then push __+__, the operator will pop two values and push the result on the stack:

    i>1 3+
      4 : int

Every operator in Impossible is highly polymorphic, indeed many of them work with different arguments or even different amount of arguments:

		i>3.0~
		  0.3333 : float
		i>"foobar"~
		  raboof : string
		
As you can see the __~__ operator will calculate the reciprocal of a float if it finds a float, or reverse a string if it finds a string (and many other options).

The language supports multiple data structures like lists, arrays, maps and also other less common data structures like ranges and lazy lists. 

    i>(1 (2 3 4) 5)1@2@
      4 : int

This snippet pushes a list which contains a list and then fetches element at index 1 form the outer list, which is the inner list. Then it gets a specific element of the latter.

There is also a lambda type which is declared with brackets __[ code ]__ and can be pushed onto the stack and used together with many operators:

    i>[2*]{1 2 3}%.>.
		  {2 4 6} : list
		
This snippet explained:

* push a lamda which pushes a 2, then pop two values and push their sum
* push a list
* swap the two topmost values on the stack (so you have list lamba <)
* invoke the **.>.** map operator which takes a collection and a lambda and creates a new collection by applying the function to each element of the former collection.

To be finished! In the meanwhile browse the inline help of the interpreter which explains each operator available up to now!
