#define PI 3.14159
#define SQUARE(x) ((x) * (x) *  (x))

int fun(int y, int z) {
	return y + z;
}

int main() {
	int x = 5;
	int y, z ;
	y = z = 5;
	
	return (x++) * (x++) * (x++);

}