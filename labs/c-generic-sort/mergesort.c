
void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *)) {
	void merge(void *lineptr[], int, int, int (*)(void *, void *));
	if(left < right){
		int mid = (left + right) / 2;
		mergesort(lineptr, left, mid, comp);
		mergesort(lineptr, left, mid, comp);
		merge(lineptr, left, right, comp);
	}		   
}

void merge(void *v[], int i, int j, int (*comp)(void *, void *)){
	void *array[j - i + 1];

	int mid = (i + j) / 2;

	if (i < j) {
		mergesort(v, i, mid, comp);
		mergesort(v, mid + 1, j, comp);
	}
	int x = 0, first = i, last = mid + 1;

	while (first <= mid && last <= j) {
		if((*comp) (v[first] ,v[last])<0) {
			array[x++] = v[first++];
		}else {
			array[x++] = v[last++];
		}	
	}
	while (first <= mid) {
		array[x++] = v[first++];
	}
	while (last <= j) {
		array[x++] = v[last++];
	}
	x = 0;
	while (i <= j) {
		v[i++] = array[x++];
	}
	
       
}
