#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_ALUNOS 10
#define MEDIA_APROVACAO 6.0f

typedef struct {
    int matricula;
    char nome[20];
    float nota01, nota02;
    float media;
} Aluno;

void limpar_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

float ler_nota(const char *rotulo){
    float nota;
    while(1){
        printf("%s", rotulo);
        if(scanf("%f", &nota) == 1 && nota >= 0 && nota <= 10){
            limpar_buffer();
            return nota;
        }
        limpar_buffer();
        printf("Nota inválida. Digite um valor entre 0 e 10.\n");
    }
}

float calcular_media(const Aluno *a){
    return (a->nota01 + a->nota02)/2;
}

Aluno *buscar_por_matricula(Aluno alunos[], int total, int matricula){
    for(int i = 0; i < total; i++){
        if(alunos[i].matricula == matricula){
            return &alunos[i];
        }
    }
    return NULL;
}

float media_geral(const Aluno alunos[], int total){
    float soma = 0;
    for(int i = 0; i < total; i++){
        soma += alunos[i].media;
    }
    return soma/total;
}

void cadastrarAluno(Aluno alunos[], int *total){

    if(*total >= MAX_ALUNOS){
        printf("Erro. Turma cheia!\n");
        return;
    }

    int matricula;
    printf("Matrícula: ");
    if(scanf("%d", &matricula) != 1){
        limpar_buffer();
        printf("Matrícula inválida.");
        return;
    }
    limpar_buffer();

    if(buscar_por_matricula(alunos, *total, matricula)){
        printf("Erro. Matrícula já cadastrada.");
        return;
    }

    Aluno *a = &alunos[*total];
    a->matricula = matricula;

    printf("Nome: ");
    fgets(a->nome, sizeof a->nome, stdin);
    size_t len = strcspn(a->nome, "\n");
    if(a->nome[len] == '\0') limpar_buffer();
    else a->nome[len] = '\0';

    a->nota01 = ler_nota("Nota 01: ");
    a->nota02 = ler_nota("Nota 02: ");
    a->media = calcular_media(a);

    (*total)++;
    printf("==== CADASTRO FEITO COM SUCESSO ====\n");
}

void exibir_situacao(const Aluno alunos[], int total){
    for(int i = 0; i < total; i++){
        printf("%-20s média %.2f - %s\n", alunos[i].nome, alunos[i].media, alunos[i].media >= MEDIA_APROVACAO ? "APROVADO" : "REPROVADO");
    }
}

void exibir_maior_menor(const Aluno alunos[], int total){
    const Aluno *maior = &alunos[0];
    const Aluno *menor = &alunos[0];

    for(int i = 1; i < total; i++){
        if(alunos[i].media > maior->media) maior = &alunos[i];
        if(alunos[i].media < menor->media) menor = &alunos[i];
    }
    printf("Maior média: %s (%.2f)\n", maior->nome, maior->media);
    printf("Menor média: %s (%.2f)\n", menor->nome, menor->media);
}

void exibir_acima_da_media(const Aluno alunos[], int total){
    float geral = media_geral(alunos, total);
    int cont = 0;

    for(int i = 0; i < total; i++){
        if(alunos[i].media > geral) cont++;
    }
    printf("Média geral: %.2f\n", geral);
    printf("Alunos acima da média: %d\n", cont);
}

void exibir_media_individual(Aluno alunos[], int total){
    int matricula;
    printf("Entre com a matrícula: ");
    
    if(scanf("%d", &matricula) != 1){
        limpar_buffer();
        printf("Matrícula inválida.\n");
        return;
    }
    limpar_buffer();

    Aluno *a = buscar_por_matricula(alunos, total, matricula);
    if(a == NULL){
        printf("Matrícula não encontrada.\n");
        return;
    }
    printf("Nome: %s\n", a->nome);
    printf("Nota 01: %.2f\n", a->nota01);
    printf("Nota 02: %.2f\n", a->nota02);
    printf("Média: %.2f\n", a->media);

}
int main(void){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    Aluno alunos[MAX_ALUNOS];
    int total = 0;
    int opcao;

    do {
        printf("==== SISTEMA DE ESCOLHAS ====\n");

        printf("==== ESCOLHA O QUE DESEJA FAZER ====\n");
        printf("1 - CADASTRAR ALUNO\n2 - CALCULAR MÉDIA INDIVIDUAL\n3 - EXIBIR SITUAÇÃO DOS ALUNOS\n4 - IDENTIFICAR ALUNO COM MAIOR E MENOR MÉDIA\n5 - CALCULAR MÉDIA GERAL DA TURMA\n6 - INFORMAR QUANTOS ALUNOS FICARAM ACIMA DA MÉDIA\n0 - SAIR\n");
        printf("Entre com sua escolha: ");
        if(scanf("%d", &opcao) != 1) opcao = -1;
        limpar_buffer();

        if(opcao >=2 && opcao <= 6 && total == 0){
            printf("Erro. Não há alunos cadastrados.\n");
            continue;
        }

        switch(opcao){
            case 1: cadastrarAluno(alunos, &total); break;
            case 2: exibir_media_individual(alunos, total); break;
            case 3: exibir_situacao(alunos, total); break;
            case 4: exibir_maior_menor(alunos, total); break;
            case 5: printf("Média geral: %.2f\n", media_geral(alunos, total)); break;
            case 6: exibir_acima_da_media(alunos, total); break;
            case 0: printf("==== SAINDO DO SISTEMA ====\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    

    return 0;
}