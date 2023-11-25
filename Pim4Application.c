#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <conio.h>

typedef struct {
    char street[50];
    char number[10];
    char neighborhood[50];
    char city[50];
    char state[3];
    char zipCode[10];
} Address;

typedef struct {
    char nameResponsible[50];
    char companyName[50];
    char cnpj[15];
    char fantasyName[50];
    char phone[15];
    char email[50];
    char openDate[11];
    Address address;
} Company;

typedef struct {
    char name[50];
    Company company;
} Customer;

typedef struct {
    char login[20];
    char password[20];
} User;

int login() {
    FILE *arquivo;
    User user;
    char userInput[20];
    char passwordInput[20];
    int found = 0;

    arquivo = fopen("usuario.txt", "r");

    if (arquivo == NULL) {
        perror("Arquivo nao encontrado");
        exit(EXIT_FAILURE);
    }

    printf("\nDigite o login: ");
    scanf("%s", userInput);
    fflush(stdin);
    printf("\nDigite a senha: ");

    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) // Enter
            break;
        putchar('*');
        passwordInput[i] = ch;
        i++;
    }
    passwordInput[i] = '\0';

    while (fscanf(arquivo, "%s %s", user.login, user.password) == 2) {
        if (strcmp(user.login, userInput) == 0 && strcmp(user.password, passwordInput) == 0) {
            found = 1;
            printf("\n\nLogin efetuado com sucesso, %s\n", user.login);
            break;
        }
    }
    fclose(arquivo);
    return found;
}

void upperCaseChars(char *string) {
    for (int i = 0; string[i]; i++) {
        string[i] = toupper((unsigned char) string[i]);
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
    upperCaseChars(string_destino);
    fflush(stdin);
}

int qtdMensal[12] = {0};
float valorMensal[12] = {0};
int qtdTotal = 0;
float valorTotal = 0;
int qtdResiduos = 0;
float valorEstimado = 0;

int monthExists(int month, char filename[50]) {
    FILE *arquivo = fopen(filename, "r");

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

void update(int mes, char fileName[50]) {
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *arquivo = fopen(fileName, "r");

    if (arquivo == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo para atualizacao.\n");
        exit(EXIT_FAILURE);
    }

    short monthExists;
    while (fscanf(arquivo, "Mês: %hd", &monthExists) == 1) {
        if (monthExists == mes) {
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

    remove(fileName);
    rename("temp.txt", fileName);

    valorTotal = 0;
    qtdTotal = 0;
}

void insertReport() {
    short mes;
    char cnpj[50];
    char fileName[50];

    printf("Informe o cnpj da empresa: ");
    scanf("%50[^\n]", cnpj);

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

    strcpy(fileName, cnpj);
    strcat(fileName, "_relatorio.txt");

    if (monthExists(mes, fileName)) {
        update(mes, fileName);
        printf("Relatorio para o mes %hd atualizado com sucesso.\n", mes);
    } else {

        FILE *arquivo = fopen(fileName, "a");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para adição.\n");
            return;
        }

        fprintf(arquivo, "Mês: %hd\n", mes);
        fprintf(arquivo, "Quantidade de resíduos tratados: %d\n", qtdResiduos);
        fprintf(arquivo, "Valor estimado de custo: %.2f\n", valorEstimado);
        fprintf(arquivo, "\n");

        fclose(arquivo);
        printf("\nRelatorio %hd adicionado com sucesso.\n", mes);

        valorTotal = 0;
        qtdTotal = 0;
    }
}

void printCustomerFromFile() {
    char cnpj[50];

    printf("\nDigite o CNPJ do cliente: ");
    scanf("%s", cnpj);

    char fileName[50];
    strcpy(fileName, cnpj);

    strcat(fileName, "-cliente.txt");

    FILE *arquivo = fopen(fileName, "r");

    if (arquivo == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    Customer c;
    char line[100];

    while (fgets(line, sizeof(line), arquivo) != NULL) {
        if (sscanf(line, "name: %49[^\n]", c.name) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "responsible: %49[^\n]", c.company.nameResponsible) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "company: %49[^\n]", c.company.companyName) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "cnpj: %14[^\n]", c.company.cnpj) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "fantasyname: %49[^\n]", c.company.fantasyName) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "phone: %14[^\n]", c.company.phone) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "street: %60[^\n]", c.company.address.street) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "number: %10[^\n]", c.company.address.number) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "neighborhood: %60[^\n]", c.company.address.neighborhood) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "city: %60[^\n]", c.company.address.city) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "state: %60[^\n]", c.company.address.state) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "zipCode: %60[^\n]", c.company.address.zipCode) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "email: %50[^\n]", c.company.email) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "opendate: %10[^\n]", c.company.openDate) == 1) {
            fgets(line, sizeof(line), arquivo);
        } else {
            printf("Erro ao ler dados do cliente.\n");
            break;
        }

        printf("\n****************************************************\n");
        printf("*     Consulta de Dados Cadastrais do Cliente      *\n");
        printf("****************************************************\n");
        printf("\n   Nome do cliente: %s ", c.name);
        printf("\n   Responsavel comercial: %s ", c.company.nameResponsible);
        printf("\n   Razao Social: %s ", c.company.companyName);
        printf("\n   Nome fantasia: %s ", c.company.fantasyName);
        printf("\n   CNPJ: %s ", c.company.cnpj);
        printf("\n   Telefone: %s ", c.company.phone);
        printf("\n   Endereco: %s, %s,"
               "\n   Bairro: %s,"
               "\n   Cidade: %s,"
               "\n   Estado: %s,"
               "\n   Cep: %s.",
               c.company.address.street,
               c.company.address.number,
               c.company.address.neighborhood,
               c.company.address.city,
               c.company.address.state,
               c.company.address.zipCode);
        printf("\n   E-mail: %s ", c.company.email);
        printf("\n   Data de abertura: %s \n", c.company.openDate);
        printf("\n****************************************************\n");

        fclose(arquivo);
    }
}

void printReportFromFile() {
    char cnpj[50];
    char fileName[50];
    int mounths;

    printf("\nDigite o CNPJ da empresa: ");
    scanf("%s", cnpj);

    printf("\nDigite a quantidade de meses: ");
    scanf("%d", &mounths);

    strcpy(fileName, cnpj);
    strcat(fileName, "_relatorio.txt");

    FILE *arquivo = fopen(fileName, "r");

    if (arquivo == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    short mes;

    printf("\n****************************************************\n");
    printf("*       Relatorio de Atividades do Cliente         *\n");
    printf("****************************************************\n");
    printf("*   Mes   *  Quantidade de Residuos  *    Custo    *\n");
    printf("****************************************************\n");

    char line[100];
    int readLines = 0;

    while (fgets(line, sizeof(line), arquivo) != NULL && readLines < mounths) {
        if (sscanf(line, "Mês: %hd", &mes) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "Quantidade de resíduos tratados: %d", &qtdResiduos) == 1 &&
            fgets(line, sizeof(line), arquivo) != NULL &&
            sscanf(line, "Valor estimado de custo: %f", &valorEstimado) == 1) {

            printf("*   %2d    *    %*d          *     %8.2f    *\n", mes, 8, qtdResiduos, valorEstimado);

            valorTotal += valorEstimado;
            qtdTotal += qtdResiduos;

            readLines++;
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
    char fileName[50];
    strcpy(fileName, customer.company.cnpj);
    strcat(fileName, "-cliente.txt");

    FILE *arquivo = fopen(fileName, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(arquivo, "name: %s\n", customer.name);
    fprintf(arquivo, "responsible: %s\n", customer.company.nameResponsible);
    fprintf(arquivo, "company: %s\n", customer.company.companyName);
    fprintf(arquivo, "cnpj: %s\n", customer.company.cnpj);
    fprintf(arquivo, "fantasyname: %s\n", customer.company.fantasyName);
    fprintf(arquivo, "phone: %s\n", customer.company.phone);
    fprintf(arquivo, "street: %s\n", customer.company.address.street);
    fprintf(arquivo, "number: %s\n ", customer.company.address.number);
    fprintf(arquivo, "neighborhood: %s\n", customer.company.address.neighborhood);
    fprintf(arquivo, "city: %s\n", customer.company.address.city);
    fprintf(arquivo, "state: %s\n", customer.company.address.state);
    fprintf(arquivo, "zipCode: %s\n", customer.company.address.zipCode);
    fprintf(arquivo, "email: %s\n", customer.company.email);
    fprintf(arquivo, "opendate: %s\n", customer.company.openDate);
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
    readString(c.company.address.state, 3);

    printf("\nInforme o cep: ");
    readString(c.company.address.zipCode, 10);

    printf("\nInforme a data de abertura (dd/mm/aaaa): ");
    readString(c.company.openDate, 11);

    printf("\nInforme o e-mail corporativo: ");
    readString(c.company.email, 50);

    printf("\nInforme o telefone comercial: ");
    readString(c.company.phone, 13);

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
    scanf("%s", u.login);
    fflush(stdin);

    printf("\nInforme uma senha: ");

    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) // Enter
            break;
        putchar('*');
        u.password[i] = ch;
        i++;
    }
    u.password[i] = '\0';

    saveEmployee(u);

    printf("\nUsuario cadastrado com sucesso!\n");
}

void displayLogin() {
    printf("\n*********** STARTUP SOLUCOES AMBIENTAIS ************\n");
    printf("\n*                 1. LOGIN                         *\n");
    printf("\n*                 2. CRIAR LOGIN                   *\n");
    printf("\n*                 0. SAIR                          *\n");
    printf("\n****************************************************\n");
}

void displayMenu() {

    printf("\n******************** HOME MENU *********************\n");
    printf("\n*               1. CADASTRAR CLIENTE               *\n");
    printf("\n*               2. CONSULTAR CLIENTE               *\n");
    printf("\n*               3. ATUALIZAR DADOS                 *\n");
    printf("\n*               4. GERAR RELATORIO                 *\n");
    printf("\n*               0. SAIR                            *\n");
    printf("\n****************************************************\n");
}

int main() {
    setlocale(LC_ALL, "");

    int option;
    int logged = 0;

    displayLogin();
    printf("\nEscolha um recurso: ");
    scanf("%d", &option);
    fflush(stdin);

    switch (option) {
        case 1:
            logged = login();
            break;
        case 2:
            registerUser();
            logged = login();
            break;
        case 0:
            printf("Finalizando o programa!\n");
            break;
        default:
            printf("Recurso invalido!\n");
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
                    printCustomerFromFile();
                    break;
                case 3:
                    insertReport();
                    break;
                case 4:
                    printReportFromFile();
                    break;
                case 0:
                    printf("Finalizando o programa!\n");
                default:
                    printf("Recurso inexistente, tente novavente.\n");
            }

        } while (option != 0);
    } else {
        printf("Falha ao fazer login, encerrando o programa.\n");
    }
    return 0;
}
