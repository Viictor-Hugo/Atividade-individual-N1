#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LENGTH 100
#define MAX_ALBUNS 3
#define MAX_NAME_LENGTH 100
#define FILENAME "artistas.txt"


typedef struct {
    char nome[MAX_LENGTH];
    char genero[MAX_LENGTH];
    char regiao[MAX_LENGTH];
    char albuns[MAX_ALBUNS][MAX_LENGTH];
    int num_albuns;
} Artista;


typedef struct {
    char nome[MAX_NAME_LENGTH];
    char genero[MAX_LENGTH];
    char origem[MAX_LENGTH];
    char album[MAX_LENGTH];
} ArtistaInfo;


int compararArtistas(const void *a, const void *b) {
    return strcmp(((ArtistaInfo*)a)->nome, ((ArtistaInfo*)b)->nome);
}


int buscaBinaria(ArtistaInfo array[], int tamanho, const char *chave) {
    int inicio = 0;
    int fim = tamanho - 1;
    int meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;

        
        int comparacao = strcmp(array[meio].nome, chave);

       
        if (comparacao == 0)
            return meio;
       
        else if (comparacao > 0)
            fim = meio - 1;
       
        else
            inicio = meio + 1;
    }

 
    return -1;
}


int albumExists(char *albumName) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file) != NULL) {
       
        line[strcspn(line, "\n")] = 0;
        if (strcmp(albumName, line) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}


void inserirArtista() {
    FILE *arquivo;
    Artista artista;

    
    arquivo = fopen(FILENAME, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

  
    printf("Digite o nome do artista: ");
    fgets(artista.nome, MAX_LENGTH, stdin);

    printf("Digite o genero musical do artista: ");
    fgets(artista.genero, MAX_LENGTH, stdin);

    printf("Digite a regiao de nascimento do artista: ");
    fgets(artista.regiao, MAX_LENGTH, stdin);

    printf("Digite o numero de albuns do artista (minimo e maximo %d): ", MAX_ALBUNS);
    scanf("%d", &artista.num_albuns);
    getchar(); 

    
    if (artista.num_albuns < 3 || artista.num_albuns > MAX_ALBUNS) {
        printf("Numero de albuns invalido. Por favor, insira  %d.\n", MAX_ALBUNS);
        fclose(arquivo);
        exit(1);
    }

  
    for (int i = 0; i < artista.num_albuns; i++) {
        printf("Digite o nome do album %d: ", i+1);
        fgets(artista.albuns[i], MAX_LENGTH, stdin);
    }


    fprintf(arquivo, "%s", artista.nome);
    fprintf(arquivo, "%s", artista.genero);
    fprintf(arquivo, "%s", artista.regiao);
    for (int i = 0; i < artista.num_albuns; i++) {
        fprintf(arquivo, "%s", artista.albuns[i]);
    }
    fprintf(arquivo, "---\n");

 
    fclose(arquivo);

    printf("Detalhes do artista foram salvos no arquivo %s com sucesso.\n", FILENAME);
}


void removerArtista() {
    char name[MAX_NAME_LENGTH];

    printf("Digite o nome do artista: ");
    fgets(name, MAX_NAME_LENGTH, stdin);


    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';

    FILE *file, *tempfile;
    char line[1000];
    int found = 0;

    file = fopen(FILENAME, "r");
    tempfile = fopen("temp.txt", "w");

    if (file == NULL || tempfile == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, name) != NULL) {
            found = 1;
           
            for (int i = 0; i < 6; i++) {
                if (fgets(line, sizeof(line), file) == NULL) {
                    break; 
                }
            }
        } else {
            fputs(line, tempfile);
        }
    }

    fclose(file);
    fclose(tempfile);

    remove(FILENAME);          
    rename("temp.txt", FILENAME);  

    if (!found) {
        printf("Artista nao encontrado no arquivo.\n");
    } else {
        printf("O artista foi removido.\n");
    }
}


void buscarArtista() {
    FILE *arquivo;
    char nomeBusca[MAX_NAME_LENGTH];
    ArtistaInfo *artistas;
    int tamanho, posicao;

  
    arquivo = fopen(FILENAME, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    
    tamanho = 0;
    while (fgets(nomeBusca, MAX_NAME_LENGTH, arquivo) != NULL) {
        tamanho++;
    }

  
    rewind(arquivo);

   
    artistas = (ArtistaInfo*)malloc(tamanho * sizeof(ArtistaInfo));
    if (artistas == NULL) {
        printf("Erro ao alocar memoria.\n");
        fclose(arquivo);
        exit(1);
    }

    
    for (int i = 0; i < tamanho; i++) {
        fgets(artistas[i].nome, MAX_NAME_LENGTH, arquivo);
        fgets(artistas[i].genero, MAX_LENGTH, arquivo);
        fgets(artistas[i].origem, MAX_LENGTH, arquivo);
        fgets(artistas[i].album, MAX_LENGTH, arquivo);
      
        artistas[i].nome[strcspn(artistas[i].nome, "\n")] = '\0';
        artistas[i].genero[strcspn(artistas[i].genero, "\n")] = '\0';
        artistas[i].origem[strcspn(artistas[i].origem, "\n")] = '\0';
        artistas[i].album[strcspn(artistas[i].album, "\n")] = '\0';
    }

    
    qsort(artistas, tamanho, sizeof(ArtistaInfo), compararArtistas);

    
    printf("Digite o nome do artista a ser buscado: ");
    fgets(nomeBusca, MAX_NAME_LENGTH, stdin);
    
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    posicao = buscaBinaria(artistas, tamanho, nomeBusca);

    if (posicao != -1) {
        printf("Artista encontrado:\n");
        printf("Nome: %s\n", artistas[posicao].nome);
        printf("Gênero: %s\n", artistas[posicao].genero);
        printf("Origem: %s\n", artistas[posicao].origem);
        printf("Álbum: %s\n", artistas[posicao].album);
    } else {
        printf("Artista nao encontrado\n");
    }

    
    free(artistas);
    fclose(arquivo);
}


void buscarAlbum() {
    char album[MAX_LENGTH];

    printf("Digite o nome do álbum: ");
    fgets(album, MAX_LENGTH, stdin);
    
    album[strcspn(album, "\n")] = 0;

    if (albumExists(album)) {
        printf("O album '%s' existe.\n", album);
    } else {
        printf("O album '%s' nao existe.\n", album);
    }
}

int main() {
    int opcao;

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1. Insercao ordenada de novos artistas\n");
        printf("2. Remocao de um artista\n");
        printf("3. Edicao de um artista\n");
        printf("4. Busca binaria por um artista\n");
        printf("5. Busca sequencial por um album\n");
        printf("0. Sair\n");

        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                inserirArtista();
                break;
            case 2:
                removerArtista();
                break;
            case 3:
                
                break;
            case 4:
                buscarArtista();
                break;
            case 5:
                buscarAlbum();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Por favor, escolha novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
