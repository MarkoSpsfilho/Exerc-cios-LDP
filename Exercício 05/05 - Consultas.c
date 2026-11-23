#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PACIENTES 15
#define MAX_CONSULTAS 30

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    char telefone[20];
} Paciente;

typedef struct {
    Paciente paciente;
    Data data;
    char descricao[100];
} Consulta;

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

Data ler_data(const char *rotulo){
    Data data;
    while(1){
        printf("%s", rotulo);
        if(scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano) == 3 && data.dia >= 1 && data.dia <= 31 && data.mes >= 1 && data.mes <= 12 && data.ano > 0){
            limpar_buffer();
            return data;
        }
        limpar_buffer();
        printf("Data inválida. Use o formato dd/mm/aaaa.\n");
    }
}

int datas_iguais(Data a, Data b){
    return a.dia == b.dia && a.mes == b.mes && a.ano == b.ano;
}

Paciente *consultar_paciente_por_codigo(Paciente pacientes[], int total, int codigo){
    for(int i = 0; i < total; i++){
        if(pacientes[i].codigo == codigo){
            return &pacientes[i];
        }
    }
    return NULL;
}

int contar_consultas_paciente(const Consulta consultas[], int total_consultas, int codigo){
    int contador = 0;
    for(int i = 0; i < total_consultas; i++){
        if(consultas[i].paciente.codigo == codigo){
            contador++;
        }
    }
    return contador;
}

Paciente *paciente_mais_atendimentos(Paciente pacientes[], int total_pacientes, const Consulta consultas[], int total_consultas){
    if(total_pacientes <= 0){
        return NULL;
    }

    Paciente *maior = &pacientes[0];
    int valor_maior = contar_consultas_paciente(consultas, total_consultas, maior->codigo);

    for(int i = 1; i < total_pacientes; i++){
        int valor = contar_consultas_paciente(consultas, total_consultas, pacientes[i].codigo);
        if(valor > valor_maior){
            valor_maior = valor;
            maior = &pacientes[i];
        }
    }
    return maior;
}

void cadastrar_paciente(Paciente pacientes[], int *total){

    if(*total >= MAX_PACIENTES){
        printf("Erro. Cadastro de pacientes cheio.\n");
        return;
    }

    int codigo;
    printf("Digite o código do paciente: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    if(consultar_paciente_por_codigo(pacientes, *total, codigo)){
        printf("Erro. Código já cadastrado.\n");
        return;
    }

    Paciente *p = &pacientes[*total];
    p->codigo = codigo;

    ler_texto("Nome: ", p->nome, sizeof p->nome);
    ler_texto("Telefone: ", p->telefone, sizeof p->telefone);

    (*total)++;
    printf("==== CADASTRO FEITO COM SUCESSO ====\n");
}

void listar_pacientes(const Paciente pacientes[], int total){
    for(int i = 0; i < total; i++){
        printf("Código: %d\t|\t Nome: %-20s\t|\t Telefone: %s\n", pacientes[i].codigo, pacientes[i].nome, pacientes[i].telefone);
    }
}

void registrar_consulta(Paciente pacientes[], int total_pacientes, Consulta consultas[], int *total_consultas){

    if(*total_consultas >= MAX_CONSULTAS){
        printf("Erro. Limite de consultas atingido.\n");
        return;
    }

    int codigo;
    printf("Digite o código do paciente: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    Paciente *p = consultar_paciente_por_codigo(pacientes, total_pacientes, codigo);
    if(p == NULL){
        printf("Erro. Paciente não encontrado.\n");
        return;
    }

    Consulta *c = &consultas[*total_consultas];
    c->paciente = *p;
    c->data = ler_data("Data da consulta (dd/mm/aaaa): ");
    ler_texto("Descrição do atendimento: ", c->descricao, sizeof c->descricao);

    (*total_consultas)++;
    printf("==== CONSULTA REGISTRADA ====\n");
}

void imprimir_consulta(const Consulta *c){
    printf("Paciente: %-20s\t|\t Data: %02d/%02d/%04d\t|\t Descrição: %s\n", c->paciente.nome, c->data.dia, c->data.mes, c->data.ano, c->descricao);
}

void pesquisar_consultas_paciente(Paciente pacientes[], int total_pacientes, const Consulta consultas[], int total_consultas){
    int codigo;
    printf("Digite o código do paciente: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    Paciente *p = consultar_paciente_por_codigo(pacientes, total_pacientes, codigo);
    if(p == NULL){
        printf("Erro. Paciente não encontrado.\n");
        return;
    }

    int encontradas = 0;
    for(int i = 0; i < total_consultas; i++){
        if(consultas[i].paciente.codigo == codigo){
            imprimir_consulta(&consultas[i]);
            encontradas++;
        }
    }

    if(encontradas == 0){
        printf("Nenhuma consulta registrada para %s.\n", p->nome);
    }
}

void listar_consultas_data(const Consulta consultas[], int total_consultas){
    Data data = ler_data("Digite a data (dd/mm/aaaa): ");

    int encontradas = 0;
    for(int i = 0; i < total_consultas; i++){
        if(datas_iguais(consultas[i].data, data)){
            imprimir_consulta(&consultas[i]);
            encontradas++;
        }
    }

    if(encontradas == 0){
        printf("Nenhuma consulta nessa data.\n");
    }
}

void relatorio_consultas(Paciente pacientes[], int total_pacientes, const Consulta consultas[], int total_consultas){
    for(int i = 0; i < total_pacientes; i++){
        printf("Paciente: %-20s\t|\t Consultas: %d\n", pacientes[i].nome, contar_consultas_paciente(consultas, total_consultas, pacientes[i].codigo));
    }

    Paciente *p = paciente_mais_atendimentos(pacientes, total_pacientes, consultas, total_consultas);
    if(p != NULL){
        printf("Paciente com mais atendimentos: %s (%d consultas)\n", p->nome, contar_consultas_paciente(consultas, total_consultas, p->codigo));
    }
}

int main(void){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    Paciente pacientes[MAX_PACIENTES];
    Consulta consultas[MAX_CONSULTAS];

    int total_pacientes = 0;
    int total_consultas = 0;
    int opcao;

    do {
        printf("==== MENU DE OPÇÕES ====\n");

        printf("1 - Cadastrar Paciente\n");
        printf("2 - Listar Pacientes\n");
        printf("3 - Registrar Consulta\n");
        printf("4 - Pesquisar Consultas de um Paciente\n");
        printf("5 - Listar Consultas por Data\n");
        printf("6 - Relatório de Consultas por Paciente\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        if(scanf("%d", &opcao) != 1) opcao = -1;
        limpar_buffer();

        if(opcao >= 2 && opcao <= 6 && total_pacientes == 0){
            printf("Erro. Não há pacientes cadastrados ainda.\n");
            continue;
        }

        if(opcao >= 4 && opcao <= 6 && total_consultas == 0){
            printf("Erro. Não há consultas registradas ainda.\n");
            continue;
        }

        switch(opcao){
            case 1: cadastrar_paciente(pacientes, &total_pacientes); break;
            case 2: listar_pacientes(pacientes, total_pacientes); break;
            case 3: registrar_consulta(pacientes, total_pacientes, consultas, &total_consultas); break;
            case 4: pesquisar_consultas_paciente(pacientes, total_pacientes, consultas, total_consultas); break;
            case 5: listar_consultas_data(consultas, total_consultas); break;
            case 6: relatorio_consultas(pacientes, total_pacientes, consultas, total_consultas); break;
            case 0: printf("==== SAINDO DO SISTEMA ====\n"); break;
            default: printf("==== OPÇÃO INVÁLIDA ====\n");
        }

    } while(opcao != 0);

    return 0;
}
