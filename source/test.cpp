/*
TestCopyConstructor
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	This tutorial illustrates the concept of copy constructor.
*	It covers issues such as conversion constructor and typeid operator.
*/

#pragma region Notes
// COPY CONSTRUCTOR
// Info:
// + http://publib.boulder.ibm.com/infocenter/compbgpl/v9v111/index.jsp?topic=/com.ibm.xlcpp9.bg.doc/language_ref/cplr390.htm
// + http://msdn.microsoft.com/en-us/library/87by589c.aspx
// + http://msdn.microsoft.com/en-us/library/63kwz036.aspx
// Why? see clone in Java:
// + http://en.wikipedia.org/wiki/Cloning_%28programming%29
// + makes a copy (not alias) of an object
// + copies are either SHALLOW (just the reference var) or DEEP (all fields)
// Copy constructor is special case of constructor conversion: 
// + if first param is reference and SAME TYPE as class, then you get a get copy of the object
// + high-level: http://msdn.microsoft.com/en-us/library/63kwz036.aspx
// + more clear (scroll down):
//   - http://msdn.microsoft.com/en-us/library/87by589c.aspx
//   - see also http://msdn.microsoft.com/en-us/magazine/cc163742.aspx
// + syntax:
//   - first param of constructor with param as same type as class
//   - recommended by http://msdn.microsoft.com/en-us/library/87by589c.aspx:
//     use MyClass(const MyClass& param) to prevent constructor from changing object's data
//     (afterall, you wanted a COPY, right? :-)
// + when does copy constructor get called?
//   - create another object of same class (call conversion constructor with same type of object)
//   - pass an object by value
// + the compiler will automatically generate a copy constructor!
//   from http://msdn.microsoft.com/en-us/library/63kwz036.aspx
//   - "A compiler-generated copy constructor sets up a new object and performs a 
//      memberwise copy of the contents of the object to be copied. If base class or member 
//      constructors exist, they are called; otherwise, bitwise copying is performed."
//   - "If all base and member classes of a class type have copy constructors that accept 
//      a const argument, the compiler-generated copy constructor accepts a single argument 
//      of type const type&. Otherwise, the compiler-generated copy constructor accepts a 
//      single argument of type type&."
//  - see also http://msdn.microsoft.com/en-us/library/8wa4affx.aspx
#pragma endregion

#include <iostream>  // for output
#include <typeinfo.h> // fot typeid
using namespace std;  // for output

class Thing;

class Thing {
private:
	int x;
	char label;
	static int count;
public:
	/* default constructor */ Thing(int v)                  { label = static_cast<char>('A' + count++); x = v;   cout << "ctor called from "; info(); }
	/* copy constructor    */ Thing(const Thing& t)         { label = static_cast<char>('A' + count++); x = t.x; cout << "copy-ctor called from "; info(); }
	/* destructor          */ ~Thing()                      { cout << "dtor called from "; info(); }
	/* pass an object      */ void test1(Thing t)           { };
	/* pass an object      */ void test2(const Thing& t)    { };
	/* return an object    */ const Thing generate1(int v)  { return v; }
	/* return an object    */ const Thing generate2()       { return Thing(50); }
	/* getter              */ const int& getX() const       { return x; }
	/* getter              */ const char& getLabel() const  { return label; }
	/* setter              */ void setX(int v)              { x = v;}
	/* like toString       */ const void info() const       { cout << typeid(*this).name() << ", object t" << label << endl; }
};

int Thing::count = 0; // need to initialize count before using it

int main() {

	// regular value objects:
	cout << "\nMake a value object:" << endl;                                 // output: Make a value object:
	Thing t1(10);  // initialize Thing object t1, pass 10 to its constructor  // output: ctor called from class Thing, object tA
	cout << "t1 data: " << t1.getX() << ", " << t1.getLabel() << endl;        // output: t1 data: 10, A

	cout << "\nUse copy/conversion constructor to duplicate t1: " << endl;    // output: Use copy/conversion constructor to duplicate t1:
	Thing t2(t1);  // initialize Thing object t2, pass t1 to it               // output: copy-ctor called from class Thing, object tB
	cout << "t2 data: " << t2.getX() << ", " << t2.getLabel() << endl;        // output: t2 data: 10, B

	cout << "\nDoes t1 change if you change t2? " << endl;                              // output: Does t1 change if you change t2?
	t2.setX(20);   // pass 20 as the argument of member function setX of object t2, x gets 20     
	cout << "t2 data:      " << t2.getX() << endl;                                      // output: t2 data:      20
	cout << "t2'2 address: " << &t2 << endl;                                            // output: t2'2 address: 00F8FBEC
	cout << "t1 data:      " << t1.getX() << endl;                                      // output: t1 data:      10
	cout << "t1's address: " << &t1 << endl;                                            // output: t1's address: 00F8FBFC (the same as t2)

	cout << "\nPass objects by val and ref (calling test1/test2 functions):" << endl;   // output: Pass objects by val and ref (calling test1/test2 functions):
	cout << "function test1: " << endl;                                                 // output: function test1:
	t1.test1(t2);  // pass t2 to test1                                                  // output: copy-ctor called from class Thing, object tC
	                                                                                    // output: dtor called from class Thing, object tC
	t2.test1(t1);  // pass t1 to test2                                                  // output: copy-ctor called from class Thing, object tD
	                                                                                    // output: dtor called from class Thing, object tD
	cout << "function test2 (should be nothing below here):\n" << endl;                 // output: function test2 (should be nothing below here):
	t1.test2(t1);
	t2.test2(t2);

	cout << "\nNote how label keeps changing:" << endl;                                 // output: Note how label keeps changing:
	Thing t3(30);  // pass 30 to the constructor of object t3                           // output: ctor called from class Thing, object tE
	cout << "t3 data: " << t3.getX() << ", " << t3.getLabel() << endl;                  // output: t3 data: 30, E

	cout << "\nCall copy constructor via function return (and cause problems):" << endl;// output: Call copy constructor via function return (and cause problems):
	Thing t4(t1.generate1(40));  // initialize an object t4, pass 40 to its constructor // output: ctor called from class Thing, object tF
	cout << "t4 data: " << t4.getX() << ", " << t4.getLabel() << endl;                  // output: t4 data: 40, F

	cout << "\nFunction return doesn't use ref type:" << endl;                          // output: Function return doesn't use ref type:
	Thing t5 = t1.generate2();  // initialize an object t5, return Thing(50)            // output: ctor called from class Thing, object tG
	cout << "t5 data: " << t5.getX() << ", " << t5.getLabel() << endl;                  // output: t5 data: 50, G

	cout << "\nWhat if you use pointers? [t6, t7, t7->test2(*t6), Thing t8(*t7)]" << endl;  // output: What if you use pointers? [t6, t7, t7->test2(*t6), Thing t8(*t7)]
	Thing* t6 = new Thing(60);  // initialize an object t6 by passing value 60              // output: ctor called from class Thing, object tH
	Thing* t7 = t6;  // initialize a pointer t7 points to a Thing object, pass t6 to it     // output: Thing t8(*t7) does!
	t7->test2(*t6);  // pass object t6
	cout << "Thing t8(*t7) does!" << endl;  // output: Thing t8(*t7) does!
	Thing t8(*t7);  // initialize an Thing object t8, pass dereferendce of t7 to its copy constructor
	cout << "t6 data: " << t6->getX() << ", " << t6->getLabel() << endl;                // output: t6 data: 60, H
	cout << "t7 data: " << t7->getX() << ", " << t7->getLabel() << endl;                // output: t7 data: 60, H (t6 and t7 points to the same obejct)
	cout << "t8 data: " << t8.getX() << ", " << t8.getLabel() << endl;                  // output: t8 data: 60, I (t8 points to the copied obejct)

    cout << "\nUse conversion function shortcut (Thing t = ...)" << endl;               // output: Use conversion function shortcut (Thing t = ...)
    Thing t9(999);                                                                      // output: ctor called from class Thing, object tJ
    Thing t10 = t9; // http://en.wikipedia.org/wiki/Copy_constructor#Initialization (conversion constructor)  // output: copy-ctor called from class Thing, object tK
    cout << "t10 data: " << t10.getX() << ", " << t10.getLabel() << endl;               // output: t10 data: 999, K

	cout << "\nIt's clobbering time!" << endl;  // output: It's clobbering time!
	cout << "Delete t6: " << endl;              // output: Delete t6: 
	delete t6;  // deallocate memory
	            // output: dtor called from class Thing, object tH
				// output: dtor called from class Thing, object tK
				// output: dtor called from class Thing, object tJ
				// output: dtor called from class Thing, object tG
				// output: dtor called from class Thing, object tF
				// output: dtor called from class Thing, object tE
				// output: dtor called from class Thing, object tB
				// output: dtor called from class Thing, object tA

}