#include <stdio.h>

int main() {
    FILE *f = fopen("prueba.txt", "w");
    if (f == NULL) {
        perror("No se pudo crear archivo de prueba");
        return 1;
    }
    fprintf(f, "Hola mundo\n");
    fclose(f);
    return 0;
}
