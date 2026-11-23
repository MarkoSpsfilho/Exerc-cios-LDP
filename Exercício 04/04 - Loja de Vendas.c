#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PRODUTOS 10
#define MAX_VENDAS 20

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int quantidade_em_estoque;
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
    float valor_total;
} Venda;

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
    int quantidade;
    while(1){
        printf("%s", rotulo);
        if(scanf("%d", &quantidade) == 1 && quantidade >= 0){
            limpar_buffer();
            return quantidade;
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

void registrar_venda(Produto *p, Venda *v, int quantidade){
    v->produto = *p;
    v->quantidade = quantidade;
    v->valor_total = p->preco * quantidade;
    p->quantidade_em_estoque -= quantidade;
}

float calcular_faturamento_total(const Venda vendas[], int total_vendas){
    float soma = 0;
    for(int i = 0; i < total_vendas; i++){
        soma += vendas[i].valor_total;
    }
    return soma;
}

float calcular_faturamento_produto(const Venda vendas[], int total_vendas, int codigo){
    float soma = 0;
    for(int i = 0; i < total_vendas; i++){
        if(vendas[i].produto.codigo == codigo){
            soma += vendas[i].valor_total;
        }
    }
    return soma;
}

Produto *produto_maior_faturamento(Produto produtos[], int total_produtos, const Venda vendas[], int total_vendas){
    if(total_produtos <= 0){
        return NULL;
    }

    Produto *maior = &produtos[0];
    float valor_maior = calcular_faturamento_produto(vendas, total_vendas, maior->codigo);

    for(int i = 1; i < total_produtos; i++){
        float valor = calcular_faturamento_produto(vendas, total_vendas, produtos[i].codigo);
        if(valor > valor_maior){
            valor_maior = valor;
            maior = &produtos[i];
        }
    }
    return maior;
}

void cadastrar_produto(Produto produtos[], int *total){

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

    ler_texto("Nome: ", p->nome, sizeof p->nome);
    p->preco = ler_preco("Preço: ");
    p->quantidade_em_estoque = ler_quantidade("Quantidade em estoque: ");

    (*total)++;
    printf("==== CADASTRO FEITO COM SUCESSO ====\n");
}

void listar_produtos(const Produto produtos[], int total){
    for(int i = 0; i < total; i++){
        printf("Nome: %-20s\t|\t Preço: %.2f\t|\t Código: %d\t|\t Quantidade em Estoque: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].codigo, produtos[i].quantidade_em_estoque);
    }
}

void menu_registrar_venda(Produto produtos[], int total_produtos, Venda vendas[], int *total_vendas){

    if(*total_vendas >= MAX_VENDAS){
        printf("Erro. Limite de vendas atingido.\n");
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

    Produto *p = consultar_produto_por_codigo(produtos, total_produtos, codigo);
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

    registrar_venda(p, &vendas[*total_vendas], quantidade);
    (*total_vendas)++;

    printf("==== VENDA REGISTRADA ====\n");
    printf("Produto Vendido: %s\n", p->nome);
    printf("Quantidade Vendida: %d\n", quantidade);
    printf("Valor Total da Venda: R$ %.2f\n", vendas[*total_vendas - 1].valor_total);
    printf("Estoque Atual do Produto: %d\n", p->quantidade_em_estoque);
}

void listar_vendas(const Venda vendas[], int total){
    for(int i = 0; i < total; i++){
        printf("Venda %d\t|\t Produto: %-20s\t|\t Quantidade: %d\t|\t Valor Total: R$ %.2f\n", i + 1, vendas[i].produto.nome, vendas[i].quantidade, vendas[i].valor_total);
    }
}

void relatorio_faturamento(Produto produtos[], int total_produtos, const Venda vendas[], int total_vendas){
    printf("Faturamento total: R$ %.2f\n", calcular_faturamento_total(vendas, total_vendas));

    Produto *p = produto_maior_faturamento(produtos, total_produtos, vendas, total_vendas);
    if(p != NULL){
        printf("Produto com maior faturamento: %s (Código %d)\n", p->nome, p->codigo);
        printf("Faturamento do produto: R$ %.2f\n", calcular_faturamento_produto(vendas, total_vendas, p->codigo));
    }
}

int main(void){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    Produto produtos[MAX_PRODUTOS];
    Venda vendas[MAX_VENDAS];

    int total_produtos = 0;
    int total_vendas = 0;
    int opcao;

    do {
        printf("==== MENU DE OPÇÕES ====\n");

        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Registrar Venda\n");
        printf("4 - Listar Vendas\n");
        printf("5 - Relatório de Faturamento\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        if(scanf("%d", &opcao) != 1) opcao = -1;
        limpar_buffer();

        if(opcao >= 2 && opcao <= 3 && total_produtos == 0){
            printf("Erro. Não há produtos cadastrados ainda.\n");
            continue;
        }

        if(opcao >= 4 && opcao <= 5 && total_vendas == 0){
            printf("Erro. Não há vendas registradas ainda.\n");
            continue;
        }

        switch(opcao){
            case 1: cadastrar_produto(produtos, &total_produtos); break;
            case 2: listar_produtos(produtos, total_produtos); break;
            case 3: menu_registrar_venda(produtos, total_produtos, vendas, &total_vendas); break;
            case 4: listar_vendas(vendas, total_vendas); break;
            case 5: relatorio_faturamento(produtos, total_produtos, vendas, total_vendas); break;
            case 0: printf("==== SAINDO DO SISTEMA ====\n"); break;
            default: printf("==== OPÇÃO INVÁLIDA ====\n");
        }

    } while(opcao != 0);

    return 0;
}
