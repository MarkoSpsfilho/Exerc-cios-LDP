#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PRODUTOS 15

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int quantidade_em_estoque;
} Produto;

void limpar_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

float ler_preco(const char *rotulo){
    float preco;
    while(1){
        printf("%s", rotulo);
        if(scanf("%f", &preco) == 1 && preco > 0){
            limpar_buffer();
            return preco;
        }
        limpar_buffer();
        printf("Valor inválido. Digite um valor maior que 0.\n");
    }
}

int ler_quantidade(const char *rotulo){
    int quantidade_em_estoque;
    while(1){
        printf("%s", rotulo);
        if(scanf("%d", &quantidade_em_estoque) == 1 && quantidade_em_estoque >= 0){
            limpar_buffer();
            return quantidade_em_estoque;
        }
        limpar_buffer();
        printf("Valor inválido. Digite um valor maior ou igual a 0.\n");
    }
}

Produto *consultar_produto_por_codigo(Produto produtos[], int total, int codigo){
    for(int i = 0; i < total; i++){
        if(produtos[i].codigo == codigo){
            return &produtos[i];
        }
    }
    return NULL;
}

void consultar_produto(Produto produtos[], int total){
    int codigo;
    printf("Entre com o código: ");

    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    Produto *p = consultar_produto_por_codigo(produtos, total, codigo);
    if(p == NULL){
        printf("Código não encontrado.\n");
        return;
    }
    printf("Nome: %s\n", p->nome);
    printf("Código: %d\n", p->codigo);
    printf("Preço: %.2f\n", p->preco);
    printf("Quantidade em Estoque: %d\n", p->quantidade_em_estoque);
}

float calcular_valor_total(const Produto produtos[], int total){
    float soma = 0;
    for(int i = 0; i < total; i++){
        soma += produtos[i].preco * produtos[i].quantidade_em_estoque;
    }
    return soma;
}

Produto *produto_maior_valor(Produto produtos[], int total){
    if(total <= 0){
        return NULL;
    }

    Produto *maior = &produtos[0];
    float valor_maior = maior->preco * maior->quantidade_em_estoque;

    for(int i = 1; i < total; i++){
        float valor = produtos[i].preco * produtos[i].quantidade_em_estoque;
        if(valor > valor_maior){
            valor_maior = valor;
            maior = &produtos[i];
        }
    }
    return maior;
}

void relatorio_estoque(Produto produtos[], int total){
    printf("Valor total em estoque: R$ %.2f\n", calcular_valor_total(produtos, total));

    Produto *p = produto_maior_valor(produtos, total);
    if(p != NULL){
        printf("Produto com maior valor em estoque: %s (Código %d)\n", p->nome, p->codigo);
        printf("Valor em estoque: R$ %.2f\n", p->preco * p->quantidade_em_estoque);
    }
}

void repor_estoque(Produto *p, int quantidade){
    p->quantidade_em_estoque += quantidade;
}

void registrar_venda(Produto *p, int quantidade_vendida){
    p->quantidade_em_estoque -= quantidade_vendida;
}

void menu_registrar_venda(Produto produtos[], int total){
    int codigo;

    printf("Digite o código: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    Produto *p = consultar_produto_por_codigo(produtos, total, codigo);
    if(p == NULL){
        printf("Erro. Produto não encontrado.\n");
        return;
    }

    printf("Produto: %s\t|\t Estoque Atual: %d\n", p->nome, p->quantidade_em_estoque);

    int quantidade = ler_quantidade("Quantidade a ser vendida: ");
    if(quantidade <= 0){
        printf("Erro. A quantidade a ser vendida deve ser maior que 0\n");
        return;
    }
    if(quantidade > p->quantidade_em_estoque){
        printf("Erro. Estoque Insuficiente. Disponível: %d\n", p->quantidade_em_estoque);
        return;
    }

    registrar_venda(p, quantidade);
    printf("==== VENDA REGISTRADA ====\n");
    printf("Código do Produto Vendido: %d\n", p->codigo);
    printf("Nome do Produto Vendido: %s\n", p->nome);
    printf("Quantidade do Produto Vendido: %d\n", quantidade);
    printf("Estoque Atual do Produto Vendido: %d\n", p->quantidade_em_estoque);
}

void menu_repor_estoque(Produto produtos[], int total){
    int codigo;

    printf("Digite o código: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    Produto *p = consultar_produto_por_codigo(produtos, total, codigo);
    if(p == NULL){
        printf("Erro. Produto não encontrado.\n");
        return;
    }

    printf("Produto: %s\t|\t Estoque Atual: %d\n", p->nome, p->quantidade_em_estoque);

    int quantidade = ler_quantidade("Quantidade a repor: ");
    if(quantidade <= 0){
        printf("Erro. A reposição deve ser maior que 0.\n");
        return;
    }

    repor_estoque(p, quantidade);
    printf("Estoque atualizado: %d\n", p->quantidade_em_estoque);
}

void cadastrar_Produto(Produto produtos[], int *total){

    if(*total >= MAX_PRODUTOS){
        printf("Erro. Estoque de Produtos cheio.\n");
        return;
    }

    int codigo;
    printf("Digite o código do produto: ");
    if(scanf("%d", &codigo) != 1){
        limpar_buffer();
        printf("Código inválido.\n");
        return;
    }
    limpar_buffer();

    if(consultar_produto_por_codigo(produtos, *total, codigo)){
        printf("Erro. Código já cadastrado.\n");
        return;
    }

    Produto *p = &produtos[*total];
    p->codigo = codigo;

    printf("Nome: ");
    fgets(p->nome, sizeof p->nome, stdin);
    size_t len = strcspn(p->nome, "\n");
    if(p->nome[len] == '\0') limpar_buffer();
    else p->nome[len] = '\0';

    p->preco = ler_preco("Preço: ");
    p->quantidade_em_estoque = ler_quantidade("Quantidade em estoque: ");

    (*total)++;
    printf("==== CADASTRO FEITO COM SUCESSO ====\n");
}

void listar_produtos(const Produto produtos[], int total){
    for (int i = 0; i < total; i++){
        printf("Nome: %-20s\t|\t Preço: %.2f\t|\t Código: %d\t|\t Quantidade em Estoque: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].codigo, produtos[i].quantidade_em_estoque);
    }
}

int main(void){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    Produto produtos[MAX_PRODUTOS];

    int total = 0;
    int opcao;

    do {
        printf("==== MENU DE OPÇÕES ====\n");

        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Repor Estoque\n");
        printf("4 - Registrar Venda\n");
        printf("5 - Consultar Produto pelo Código\n");
        printf("6 - Relatório de Estoque\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        if(scanf("%d", &opcao) != 1) opcao = -1;
        limpar_buffer();

        if(opcao >= 2 && opcao <= 6 && total == 0){
            printf("Erro. Não há produtos cadastrados ainda.\n");
            continue;
        }

        switch(opcao){
            case 1: cadastrar_Produto(produtos, &total); break;
            case 2: listar_produtos(produtos, total); break;
            case 3: menu_repor_estoque(produtos, total); break;
            case 4: menu_registrar_venda(produtos, total); break;
            case 5: consultar_produto(produtos, total); break;
            case 6: relatorio_estoque(produtos, total); break;
            case 0: printf("==== SAINDO DO SISTEMA ====\n"); break;
            default: printf("==== OPÇÃO INVÁLIDA ====\n");
        }

    } while(opcao != 0);

    return 0;
}
