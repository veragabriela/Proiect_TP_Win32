
#define Max 1000
#define NULL 0

struct Node {
	int poz;
	int data;
	Node*next;
};

void creare_histograma(int **mat, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mat[i][j] == 1)
				mat[i][j] = 0;
			else if (i > 0) 
				mat[i][j] = mat[i - 1][j] + 1;
			else 
				mat[i][j] = 1;
		}
	}
}

void push(Node*&top, int data, int poz) {
	Node*nou = new Node;
	nou->data = data;
	nou->poz = poz;
	nou->next = top;
	top = nou;
}

int pop(Node*&top) {
	int poz = top->poz;
	Node*temp = top;
	top = top->next;
	delete temp;
	return poz;
}

int arie_max(int v[], int n) {
	int pozitie_initiala = -1, arie, pozitie, maxim = 0, i;
	v[n++] = 0;
	Node*top = NULL;
	for (i = 0; i < n; i++) {
		if (v[i] != 0 && (top == NULL || top->data <= v[i])) {
			push(top, v[i], i);
		}
		else {
			while (top != NULL && top->data>v[i]) {
				pozitie = pop(top);
				if (top != NULL) 
					arie = v[pozitie] * (i - pozitie_initiala - 1);
				else 
					arie = v[pozitie] * (i - pozitie);
				if (arie > maxim)
					maxim = arie;
			}
			if (v[i] != 0)
				push(top, v[i], i);
		}
		if (v[i] == 0)
			pozitie_initiala = i;
	}
	return maxim;
}

int aflare_arie(int **mat, int n, int m) {
	int arie, maxim = 0;
	for (int i = 0; i < n; i++) {
		arie = arie_max(mat[i], m);
		if (arie > maxim) maxim = arie;
	}
	return maxim;
}

