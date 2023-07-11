#ifndef COMMON_H
#define COMMON_H
template<typename T>
void print(vector<T> data) {
	/*
		arr print
		This func don't support line break.
	*/
	printf("[");
	for (int i = 0; i < data.size(); i++) {
		if (i + 1 % 20 == 0)printf("\n");
		printf("%.2f,", data[i]);
	}
	printf("\b]\n");
}

template<typename T>
void print(vector<vector<T>> data) {
	/*
		2dim-arr print
	*/
	printf("[\n");
	for (int i = 0; i < data.size(); i++) {
		print(data[i]);
	}
	printf("]\n");
}
#endif // !_COMMON_H