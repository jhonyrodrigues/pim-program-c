#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>

// Definição da estrutura para endereço
typedef struct {
    char street[50];
    char number[10];
    char neighborhood[50];
    char city[50];
    char state[3];
    char zipCode[10];
} Address;

// Definição da estrutura para indústria
typedef struct {
    char nameResponsible[50];
    char companyName[50];
    char cnpj[15];
    char corporateName[50];
    char fantasyName[50];
    char phone[15];
    char email[50];
    char openDate[11];
    Address address;
} Company;

// Definição da estrutura para clientes
typedef struct {
    char name[50];
    Company company;
} Customer;

typedef struct {
    char login[20];
    char password[20];
} User;

// Função para realizar o login
int login() {
    FILE *arquivo;
    User user;
    char userInput[20];
    char passwordInput[20];
    int found = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("usuario.txt", "r");

    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    printf("\nDigite o login: ");
    scanf("%s", userInput);
    fflush(stdin);
    printf("\nDigite a senha: ");
    scanf("%s", passwordInput);

    while (fscanf(arquivo, "%s %s", user.login, user.password) == 2) {
        if (strcmp(user.login, userInput) == 0 && strcmp(user.password, passwordInput) == 0) {
            found = 1;
            printf("\nLogin efetuado com sucesso, %s\n", user.login);
            break;
        }
    }
    fclose(arquivo);
    return found;
}

void uppercaseChars(char *string) {
    for (int i = 0; string[i]; i++) {
        string[i] = toupper((unsigned char)string[i]);
    }
}

void removeLineBreak(char *string) {
    if (string != NULL && strlen(string) > 0) {
        short size = strlen(string);
        if (string[size - 1] == '\n') {
            string[size - 1] = '\0';
        }
    }
}

void readString(char *string_destino, int quantidade_caracteres) {
    fgets(string_destino, quantidade_caracteres, stdin);
    removeLineBreak(string_destino);
    uppercaseChars(string_destino);
    fflush(stdin);
}

int qtdMensal[12] = {0};
float valorMensal[12] = {0};
int qtdTotal = 0;
float valorTotal = 0;
int qtdResiduos = 0;
float valorEstimado = 0;

int monthExists(int month) {
    FILE *arquivo = fopen("relatorio.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    short mesExistente;
    while (fscanf(arquivo, "Mês: %hd", &mesExistente) == 1) {
        if (mesExistente == month) {
            fclose(arquivo);
            return 1;
        } else {
            fseek(arquivo, 1, SEEK_CUR);
        }
    }

    fclose(arquivo);
    return 0;
}

void update(int mes) {
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *arquivo = fopen("relatorio.txt", "r");

    if (arquivo == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo para atualização.\n");
        exit(EXIT_FAILURE);
    }

    short mesExistente;
    while (fscanf(arquivo, "Mês: %hd", &mesExistente) == 1) {
        if (mesExistente == mes) {
            fprintf(tempFile, "Mês: %hd\n", mes);
            fprintf(tempFile, "Quantidade de resíduos tratados: %d\n", qtdResiduos);
            fprintf(tempFile, "Valor estimado de custo: %.2f\n", valorEstimado);
            fprintf(tempFile, "\n");
        } else {
            fseek(arquivo, 1, SEEK_CUR);
        }
    }

    fclose(arquivo);
    fclose(tempFile);

    remove("relatorio.txt");
    rename("temp.txt", "relatorio.txt");

    valorTotal = 0;
    qtdTotal = 0;
}

void insertReport() {
    short mes;

    printf("Informe o mes (1-12): ");
    scanf("%hd", &mes);

    if (mes < 1 || mes > 12) {
        printf("Mes invalido. Insira um valor entre 1 e 12.\n");
        return;
    }

    printf("Informe a quantidade de residuos tratados: ");
    scanf("%d", &qtdResiduos);
    qtdMensal[mes - 1] += qtdResiduos;
    qtdTotal += qtdResiduos;

    printf("Informe o valor estimado de custo: ");
    scanf("%f", &valorEstimado);
    valorMensal[mes - 1] += valorEstimado;
    valorTotal += valorEstimado;

    if (monthExists(mes)) {
        update(mes);
        printf("Relatório para o mês %hd atualizado com sucesso.\n", mes);
    } else {
        FILE *arquivo = fopen("relatorio.txt", "a");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para adição.\n");
            return;
        }

        fprintf(arquivo, "Mês: %hd\n", mes);
        fprintf(arquivo, "Quantidade de resíduos tratados: %d\n", qtdResiduos);
        fprintf(arquivo, "Valor estimado de custo: %.2f\n", valorEstimado);
        fprintf(arquivo, "\n");

        fclose(arquivo);
        printf("Relatório para o mês %hd adicionado com sucesso.\n", mes);

        valorTotal = 0;
        qtdTotal = 0;
    }
}

void printReportFromFile() {
    FILE *arquivo = fopen("relatorio.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    short mes;

    printf("\n****************************************************\n");
    printf("*       Relatorio de Atividades do Cliente         *\n");
    printf("****************************************************\n");
    printf("*   Mes   *  Quantidade de Residuos  *    Custo    *\n");
    printf("****************************************************\n");

    char linha[100];


    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "Mês: %hd", &mes) == 1) {

            fgets(linha, sizeof(linha), arquivo);

            if (sscanf(linha, "Quantidade de resíduos tratados: %d", &qtdResiduos) != 1) {
                printf("Erro ao ler a quantidade de resíduos.\n");
                break;
            }

            fgets(linha, sizeof(linha), arquivo);

            if (sscanf(linha, "Valor estimado de custo: %f", &valorEstimado) != 1) {
                printf("Erro ao ler o valor estimado.\n");
                break;
            }

            printf("*   %2d    *    %*d          *     %8.2f    *\n", mes, 8, qtdResiduos, valorEstimado);

            valorTotal += valorEstimado;
            qtdTotal += qtdResiduos;
        }
    }

    fclose(arquivo);

    printf("****************************************************\n");
    printf("*  Total         %*d                %8.2f    *\n", 6, qtdTotal, valorTotal);
    printf("****************************************************\n");

    valorTotal = 0;
    qtdTotal = 0;
}

void saveCustomer(Customer customer) {
    FILE *arquivo = fopen("cliente.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "name: %s\n", customer.name);
    fprintf(arquivo, "responsible: %s\n", customer.company.nameResponsible);
    fprintf(arquivo, "company: %s\n", customer.company.companyName);
    fprintf(arquivo, "cnpj: %s\n", customer.company.cnpj);
    fprintf(arquivo, "fantasy_name: %s\n", customer.company.fantasyName);
    fprintf(arquivo, "phone: %s\n", customer.company.phone);
    fprintf(arquivo, "address: %s, %s, %s, %s, %s, %s\n",
            customer.company.address.street,
            customer.company.address.number,
            customer.company.address.neighborhood,
            customer.company.address.city,
            customer.company.address.state,
            customer.company.address.zipCode);
    fprintf(arquivo, "e-mail: %s\n", customer.company.email);
    fprintf(arquivo, "opening_date: %s\n", customer.company.openDate);
    fprintf(arquivo, "\n");

    fclose(arquivo);
}

void registerCustomer() {
    Customer c;
    printf("\nInforme o nome do cliente: ");
    readString(c.name, 50);

    printf("\nInforme o nome do responsavel pela empresa: ");
    readString(c.company.nameResponsible, 20);

    printf("\nInforme o cnpj: ");
    readString(c.company.cnpj, 12);

    printf("\nInforme a razao social da empresa: ");
    readString(c.company.companyName, 50);

    printf("\nInforme o nome fantasia: ");
    readString(c.company.fantasyName, 50);

    printf("\nInforme o logradouro: ");
    readString(c.company.address.street, 50);

    printf("\nInforme o numero: ");
    readString(c.company.address.number, 12);

    printf("\nInforme o bairro: ");
    readString(c.company.address.neighborhood, 50);

    printf("\nInforme a cidade: ");
    readString(c.company.address.city, 50);

    printf("\nInforme o estado: ");
    readString(c.company.address.state, 2);

    printf("\nInforme o cep: ");
    readString(c.company.address.zipCode, 10);

    printf("\nInforme a data de abertura (yyyy/MM/dd): ");
    readString(c.company.openDate, 11);

    printf("\nInforme o e-mail corporativo: ");
    readString(c.company.email, 50);

    printf("\nInforme o telefone comercial: ");
    readString(c.company.phone, 12);

    saveCustomer(c);
    printf("Cliente cadastrado com sucesso!\n");
}

void saveEmployee(User user) {
    FILE *arquivo = fopen("usuario.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(arquivo, "%s\n", user.login);
    fprintf(arquivo, "%s\n", user.password);
    fprintf(arquivo, "\n");

    fclose(arquivo);
}

void registerUser() {
    User u;

    printf("\nInforme um login: ");
    readString(u.login, 20);

    printf("\nInforme uma senha: ");
    readString(u.password, 10);

    saveEmployee(u);

    printf("Usuario cadastrado com sucesso!\n");
}

void displayLogin() {
    printf("\n*********** STARTUP SOLUCOES AMBIENTAIS ************\n");
    printf("\n*                 1. CRIAR USUARIO                 *\n");
    printf("\n*                 2. LOGAR                         *\n");
    printf("\n****************************************************\n");
}

void displayMenu() {

    printf("\n******************** HOME MENU *********************\n");
    printf("\n*               1. CADASTRAR CLIENTE               *\n");
    printf("\n*               2. ATUALIZAR DADOS                 *\n");
    printf("\n*               3. GERAR RELATORIO                 *\n");
    printf("\n*               4. SAIR                            *\n");
    printf("\n****************************************************\n");
}

int main() {
    SMALL_RECT windowSize = {0, 0, 51, 24};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    setlocale(LC_ALL, "");

    int option;
    int logged = 0;

    displayLogin();
    printf("\nEscolha um recurso: ");
    scanf("%d", &option);
    fflush(stdin);

    if (option == 1){
        registerUser();
    }
    {
        logged = login();
    }

    if (logged) {
        do {
            displayMenu();
            printf("\nEscolha um recurso: ");
            scanf("%d", &option);
            fflush(stdin);

            switch (option) {
                case 1:
                    registerCustomer();
                    break;
                case 2:
                    insertReport();
                    break;
                case 3:
                    printReportFromFile();
                    break;
                case 4:
                    printf("Finalizando o programa!\n");
                    break;
                default:
                    printf("Recurso inexistente, tente novavente.\n");
            }

        } while (option != 4); // 4 é a opção de sair
    } else {
        printf("Falha ao fazer login, encerrando o programa.\n");
    }
    return 0;
}
