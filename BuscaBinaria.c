#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int busca_binaria(int arr[], int n, int valor, int* comparacoes) {
    int l = 0;
    int r = n - 1;
    int mid;
    
    if (comparacoes) *comparacoes = 0;
    
    while (l <= r) {
        if (comparacoes) (*comparacoes)++;
        
        mid = l + (r - l) / 2;
        
        if (arr[mid] == valor) {
            return mid; 
        }
        else if (arr[mid] < valor) {
            l = mid + 1;
        }
        else {
            r = mid - 1;
        }
    }
    
    return -1;  // Não encontrado
}

int* criar_array_ordenado(int n) {
    int* arr = malloc(n * sizeof(int));
    if (arr == NULL) return NULL;
    
    for (int i = 0; i < n; i++) {
        arr[i] = i * 2;  // Valores pares
    }
    
    return arr;
}

void medir_limites_busca_binaria() {
    srand(time(NULL));
    
    int tamanhos[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};
    int num_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    printf("BUSCA BINARIA - Limites de Complexidade\n");
    printf("========================================\n");
    printf("%-10s %-15s %-15s %-15s %-15s\n", 
           "n", "log(n)", "Omega(1)", "O(log n)", "Casos Reais");
    printf("----------------------------------------------------------------\n");
    
    FILE *arquivo = fopen("busca_binaria_limites.csv", "w");
    if (arquivo) {
        fprintf(arquivo, "n,log2_n,limite_inferior,limite_superior,melhor_caso,pior_caso,caso_medio\n");
    }
    
    for (int i = 0; i < num_testes; i++) {
        int n = tamanhos[i];
        
        int* arr = criar_array_ordenado(n);
        if (arr == NULL) continue;
        
        int comparacoes;
        double log_n = log2(n);
        
        // 1. MELHOR CASO (Ω): elemento exatamente no meio
        int alvo_melhor = arr[(n/2) - 1];
        busca_binaria(arr, n, alvo_melhor, &comparacoes);
        int melhor_caso = comparacoes;
        
        // 2. PIOR CASO (O): elemento não existe / última posição checada
        int alvo_pior = -1;
        busca_binaria(arr, n, alvo_pior, &comparacoes);
        int pior_caso = comparacoes;
        
        // 3. CASO MÉDIO: média de várias buscas aleatórias
        double soma_comparacoes = 0;
        int num_amostras = 100;
        
        for (int j = 0; j < num_amostras; j++) {
            // 50% de existir, 50% de não existir
            int alvo;
            if (rand() % 2 == 0) {
                alvo = arr[rand() % n];  // Elemento existente
            } else {
                alvo = (rand() % (2 * n)) * 2 + 1;  // Elemento ímpar (não existe)
            }
            
            busca_binaria(arr, n, alvo, &comparacoes);
            soma_comparacoes += comparacoes;
        }
        double caso_medio = soma_comparacoes / num_amostras;
        
        // 4. Limites teóricos
        double limite_inferior = 1.0;  // Ω(1)
        double limite_superior = ceil(log_n);  // O(log n)
        
        printf("%-10d %-15.2f %-15.1f %-15.2f %-15.2f\n", 
               n, log_n, limite_inferior, limite_superior, caso_medio);
        
        if (arquivo) {
            fprintf(arquivo, "%d,%.2f,%.1f,%.2f,%d,%d,%.2f\n", 
                    n, log_n, limite_inferior, limite_superior, 
                    melhor_caso, pior_caso, caso_medio);
        }
        
        free(arr);
    }
    
    if (arquivo) {
        fclose(arquivo);
        printf("\nDados salvos em 'busca_binaria_limites.csv'\n");
    }
}

int main() {
    printf("Teste basico da busca binaria:\n");
    
    int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");
    
    int valores_teste[] = {4, 10, 16, 5, 25};
    int num_testes = sizeof(valores_teste) / sizeof(valores_teste[0]);
    
    for (int i = 0; i < num_testes; i++) {
        int resultado = busca_binaria(arr, n, valores_teste[i], NULL);
        if (resultado != -1) {
            printf("Valor %d encontrado na posicao %d\n", valores_teste[i], resultado);
        } else {
            printf("Valor %d nao encontrado\n", valores_teste[i]);
        }
    }
    
    printf("\n\n");
    
    medir_limites_busca_binaria();
    
    return 0;
}