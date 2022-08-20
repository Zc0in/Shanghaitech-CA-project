#include<iostream>
using namespace std;

int f(int n){
	if (n == 1){
		return 1;
	}
	return n * f(n - 1);
}

int main(){
	int a = f(6);
	cout << a << endl;
}
