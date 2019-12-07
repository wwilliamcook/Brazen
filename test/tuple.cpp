/*
 * tuple.cpp
 * 
 * Tests some functionality of /include/tuple.h
 */

#include "tuple.h"

void print(void) {
	std::cout << std::endl;
}
template <typename T, typename... Args>
void print(T arg, Args... args) {
	std::cout << arg << " ";
	print(args...);
}

int main() {
	Tuple<1> a1(3), b1(4);
	Tuple<2> a2(2, 3), b2(4, 5);
	Tuple<3> a3(1, 2, 3), b3(4, 5, 6);
	Tuple<4> a4(0, 1, 2, 3), b4(4, 5, 6, 7);
/*
	print("Print Test");

	print("a1:", a1);
	print("b1:", b1);
	print("a2:", a2);
	print("b2:", b2);
	print("a3:", a3);
	print("b3:", b3);
	print("a4:", a4);
	print("b4:", b4);

	print("\nAddition/Subtraction Test");

	print("a1 + b1:", a1 + b1);
	print("a1 - b1:", a1 - b1);
	print("a2 + b2:", a2 + b2);
	print("a2 - b2:", a2 - b2);
	print("a3 + b3:", a3 + b3);
	print("a3 - b3:", a3 - b3);
	print("a4 + b4:", a4 + b4);
	print("a4 - b4:", a4 - b4);

	print("\nScalar Multiplication Test");

	print("a1 * 2:", a1 * 2);
	print("2 * b1:", 2 * b1);
	print("a2 * 2:", a2 * 2);
	print("2 * b2:", 2 * b2);
	print("a3 * 2:", a3 * 2);
	print("2 * b3:", 2 * b3);
	print("a4 * 2:", a4 * 2);
	print("2 * b4:", 2 * b4);

	print("\nScalar Division Test");

	print("a1 / 2:", a1 / 2);
	print("a2 / 2:", a2 / 2);
	print("a3 / 2:", a3 / 2);
	print("a4 / 2:", a4 / 2);

	print("\nMagnitude Test");

	print("|a1|:", magnitude(a1));
	print("|a2|:", magnitude(a2));
	print("|a3|:", magnitude(a3));
	print("|a4|:", magnitude(a4));

	print("\nUnit Vector Test");

	print("unit(a1):", unit(a1));
	print("unit(a2):", unit(a2));
	print("unit(a3):", unit(a3));
	print("unit(a4):", unit(a4));

	print("\nUnit Vector of Zero Vector Test");
*/
	for (int i = 0; i < 1; i++) {
		print("unit(a1 - a1):", unit(a1 - a1));
			print("unit(a2 - a2):", unit(a2 - a2));
		print("unit(a3 - a3):", unit(a3 - a3));
		print("unit(a4 - a4):", unit(a4 - a4));
		print();
	}
/*
	print("Magnitude of Unit Vector of Zero Vector Test");

	print("|unit(a1 - a1)|:", magnitude(unit(a1 - a1)));
	print("|unit(a2 - a2)|:", magnitude(unit(a2 - a2)));
	print("|unit(a3 - a3)|:", magnitude(unit(a3 - a3)));
	print("|unit(a4 - a4)|:", magnitude(unit(a4 - a4)));

	print("\nDot Product Test");

	print("a1 * b1:", dot(a1, b1));
	print("a2 * b2:", dot(a2, b2));
	print("a3 * b3:", dot(a3, b3));
	print("a4 * b4:", dot(a4, b4));

	print("\nCross Product Test");

	print("a3 x b3:", cross(a3, b3));
	print("a3 * (a3 x b3):", dot(a3, cross(a3, b3)));

	print("\nIn-place Addition Test");

	print("a1 += b1:", a1 += b1);
	print("a2 += b2:", a2 += b2);
	print("a3 += b3:", a3 += b3);
	print("a4 += b4:", a4 += b4);

	print("\nIn-place Subtraction Test");

	print("a1 -= b1:", a1 -= b1);
	print("a2 -= b2:", a2 -= b2);
	print("a3 -= b3:", a3 -= b3);
	print("a4 -= b4:", a4 -= b4);

	print("\nIn-place Scalar Multiplication Test");

	print("a1 *= 2:", a1 *= 2);
	print("a2 *= 2:", a2 *= 2);
	print("a3 *= 2:", a3 *= 2);
	print("a4 *= 2:", a4 *= 2);

	print("\nIn-place Scalar Division Test");

	print("a1 /= 2:", a1 /= 2);
	print("a2 /= 2:", a2 /= 2);
	print("a3 /= 2:", a3 /= 2);
	print("a4 /= 2:", a4 /= 2);
*/
	return 0;
}
