#if 1
#include "test2.c"
int main(int argc, char* argv[])  	{
	return 0;
}
#else
#ifdef ONE
void main() {
	return 1;
}
#endif
#endif
