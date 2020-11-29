// If this file's guard is not defined...
#ifndef BASKET_H
// Define the guard and include the contents of this file.
#define BASKET_H
// (Else, do not include the contents of this file.)

class Basket {
private:
	int m_apples;
public:
	int get_apples() const;
	void set_apples(int value);
};

// Finish the header guard IF directive.
#endif