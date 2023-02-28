// Algorítimos de Ordenação


struct Data {
    int ID;
    char *restOfLine;
};


int compare(const void *a, const void *b) {
    struct Data *ia = (struct Data *)a;
    struct Data *ib = (struct Data *)b;
    return (ia->ID - ib->ID);
}


void merge(struct Data arr[], int l, int m, int r, int *comparisons, int *swaps) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    struct Data L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0; 
    j = 0; 
    k = l; 
    int local_swaps = 0; // contador local de swaps
    while (i < n1 && j < n2) {
        if (compare(&L[i], &R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
            local_swaps++; // incrementa o contador local de swaps
        }
        k++;
        (*comparisons)++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    (*swaps) += local_swaps; // adiciona o contador local de swaps ao contador total
}

void mergeSort(struct Data arr[], int l, int r, int *comparisons, int *swaps) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, comparisons, swaps);
        mergeSort(arr, m + 1, r, comparisons, swaps);
        merge(arr, l, m, r, comparisons, swaps);
    }
}


void shellSort(struct Data arr[], int n, int *comparisons, int *swaps) {
    int gap = n / 2;
    while (gap > 0) {
        for (int i = gap; i < n; i += 1) {
            struct Data temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap].ID > temp.ID; j -= gap) {
                arr[j] = arr[j - gap];
                (*swaps)++;
                (*comparisons)++;
            }
            arr[j] = temp;
        }
        gap /= 2;
    }
}


void insertionSort(struct Data arr[], int n, int *comparisons, int *swaps) {
    int i, j;
    struct Data temp;
    for (i = 1; i < n; i++) {
        temp = arr[i];
        j = i - 1;
        (*comparisons)++;
        while (j >= 0 && arr[j].ID > temp.ID) {
            (*comparisons)++;
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
        if (j + 1 != i) {
            (*swaps)++;
        }
    }
}


void selectionSort(struct Data arr[], int n, int *comparisons, int *swaps) {
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[j].ID < arr[min_idx].ID) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            struct Data temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            (*swaps)++;
        }
    }
}


// quickSort
int partition(struct Data arr[], int low, int high, int *comparisons, int *swaps) {
    int pivot = arr[high].ID;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        (*comparisons)++;
        if (arr[j].ID < pivot) {
            i++;
            struct Data temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            (*swaps)++;
        }
    }
    struct Data temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    (*swaps)++;
    return i + 1;
}

void quickSort(struct Data arr[], int low, int high, int *comparisons, int *swaps) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons, swaps);
        quickSort(arr, low, pi - 1, comparisons, swaps);
        quickSort(arr, pi + 1, high, comparisons, swaps);
    }
}


int busca_sequencial(struct Data data[], int tamanho, int valor) {
    for (int i = 0; i < tamanho; i++) {
        if (data[i].ID == valor) {
            return i;
        }
    }
    return -1;
}


int binarySearch(struct Data *data, int l, int r, int key, int *comparisons) {
    while (l <= r) {
        int mid = l + (r - l) / 2;

        (*comparisons)++;
        if (data[mid].ID == key)
            return mid;

        (*comparisons)++;
        if (data[mid].ID < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}
