#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_FUNCIONARIOS 8
#define HORAS_MENSAIS 160
#define ACRESCIMO_HORA_EXTRA 1.5f
#define DESCONTO 0.10f

typedef struct {
    int codigo;
    char nome[50];
    char cargo[30];
    float salario_base;
    int horas_extras;
} Funcionario;

void limpar_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void ler_texto(const char *rotulo, char *destino, int tamanho){
    printf("%s", rotulo);
    fgets(destino, tamanho, stdin);
    size_t len = strcspn(destino, "\n");
    if(destino[len] == '\0') limpar_buffer();
    else destino[len] = '\0';
}

float ler_salario(const char *rotulo){
    float salario;
    while(1){
        printf("%s", rotulo);
        if(scanf("%f", &salario) == 1 && salario > 0){
            limpar_buffer();
            return salario;
        }
        limpar_buffer();
        printf("Valor inválido. Digite um valor maior que 0.\n");
    }
}

int ler_horas_extras(const char *rotulo){
    int horas;
    while(1){
        printf("%s", rotulo);
        if(scanf("%d", &horas) == 1 && horas >= 0){
            limpar_buffer();
            return horas;
        }
        limpar_buffer();
        printf("Valor inválido. Digite um valor maior ou igual a 0.\n");
    }
}

Funcionario *consultar_funcionario_por_codigo(Funcionario funcionarios[], int total, int codigo){
    for(int i = 0; i < total; i++){
        if(funcionarios[i].codigo == codigo){
            return &funcionarios[i];
        }
    }
    return NULL;
}

float calcular_valor_hora(const Funcionario *f){
    return f->salario_base / HORAS_MENSAIS;
}

float calcular_salario_bruto(const Funcionario *f){
    float valor_hora = calcular_valor_hora(f);
    float valor_hora_extra = valor_hora * ACRESCIMO_HORA_EXTRA;
    return f->salario_base + valor_hora_extra * f->horas_extras;
}

float calcular_salario_liquido(const Funcionario *f){
    float bruto = calcular_salario_bruto(f);
    return bruto - bruto * DESCONTO;
}

Funcionario *funcionario_maior_liquido(Funcionario funcionarios[], int total){
    if(total <= 0){
        return NULL;
    }

    Funcionario *maior = &funcionarios[0];
    float valor_maior = calcular_salario_liquido(maior);

    for(int i = 1; i < total; i++){
        float valor = calcular_salario_liquido(&funcionarios[i]);
        if(valor > valor_maior){
            valor_maior = valor;
            maior = &funcionarios[i];
        }
    }
    return maior;
}

float calcular_custo_total(const Funcionario funcionarios[], int total){
    float soma = 0;
    for(int i = 0; i < total; i++){
        soma += calcular_salario_liquido(&funcionarios[i]);
    }
    return soma;
}

void cadastrar_funcionario(Funcionario funcionarios[], int *total){

    if(*total >= MAX_FUNCIONARIOS){
        printf("Erro. Quadro de funcionários cheio.\n");
        return;
    }

    int codigo;
    printf("Digite o código do funcionário: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    if(consultar_funcionario_por_codigo(funcionarios, *total, codigo)){
        printf("Erro. Código já cadastrado.\n");
        return;
    }

    Funcionario *f = &funcionarios[*total];
    f->codigo = codigo;

    ler_texto("Nome: ", f->nome, sizeof f->nome);
    ler_texto("Cargo: ", f->cargo, sizeof f->cargo);
    f->salario_base = ler_salario("Salário-base: ");
    f->horas_extras = ler_horas_extras("Horas extras: ");

    (*total)++;
    printf("==== CADASTRO FEITO COM SUCESSO ====\n");
}

void listar_funcionarios(const Funcionario funcionarios[], int total){
    for(int i = 0; i < total; i++){
        printf("Código: %d\t|\t Nome: %-20s\t|\t Cargo: %-15s\t|\t Salário-base: %.2f\t|\t Horas Extras: %d\n", funcionarios[i].codigo, funcionarios[i].nome, funcionarios[i].cargo, funcionarios[i].salario_base, funcionarios[i].horas_extras);
    }
}

void folha_pagamento(Funcionario funcionarios[], int total){
    printf("==== FOLHA DE PAGAMENTO ====\n");
    for(int i = 0; i < total; i++){
        Funcionario *f = &funcionarios[i];
        float bruto = calcular_salario_bruto(f);
        printf("Nome: %s\n", f->nome);
        printf("Valor da hora: R$ %.2f\n", calcular_valor_hora(f));
        printf("Salário bruto: R$ %.2f\n", bruto);
        printf("Desconto (10%%): R$ %.2f\n", bruto * DESCONTO);
        printf("Salário líquido: R$ %.2f\n", calcular_salario_liquido(f));
        printf("----------------------------\n");
    }

    Funcionario *maior = funcionario_maior_liquido(funcionarios, total);
    if(maior != NULL){
        printf("Maior salário líquido: %s (R$ %.2f)\n", maior->nome, calcular_salario_liquido(maior));
    }
    printf("Custo total da folha: R$ %.2f\n", calcular_custo_total(funcionarios, total));
}

int main(void){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    Funcionario funcionarios[MAX_FUNCIONARIOS];

    int total = 0;
    int opcao;

    do {
        printf("==== MENU DE OPÇÕES ====\n");

        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Folha de Pagamento\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        if(scanf("%d", &opcao) != 1) opcao = -1;
        limpar_buffer();

        if(opcao >= 2 && opcao <= 3 && total == 0){
            printf("Erro. Não há funcionários cadastrados ainda.\n");
            continue;
        }

        switch(opcao){
            case 1: cadastrar_funcionario(funcionarios, &total); break;
            case 2: listar_funcionarios(funcionarios, total); break;
            case 3: folha_pagamento(funcionarios, total); break;
            case 0: printf("==== SAINDO DO SISTEMA ====\n"); break;
            default: printf("==== OPÇÃO INVÁLIDA ====\n");
        }

    } while(opcao != 0);

    return 0;
}
