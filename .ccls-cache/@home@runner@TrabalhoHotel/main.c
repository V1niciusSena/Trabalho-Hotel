#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 100
#define MAX_FUNCIONARIOS 50
#define MAX_RESERVAS 100
#define MAX_QUARTOS 50

typedef struct {
    int codigo;
    char nome[50];
    char endereco[100];
    char telefone[20];
} Cliente;

typedef struct {
    int codigo;
    char nome[50];
    char telefone[20];
    char cargo[30];
    float salario;
} Funcionario;

typedef struct {
    int numero;
    int capacidade;
    float precoDiaria;
    char status[15];
} Quarto;

typedef struct {
    int codigo;
    char dataEntrada[11];
    char dataSaida[11];
    int numDiarias;
    int codigoCliente;
    int numeroQuarto;
} Reserva;

int calcularNumDiarias(const char* dataEntrada, const char* dataSaida) {
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    sscanf(dataEntrada, "%d/%d/%d", &diaEntrada, &mesEntrada, &anoEntrada);
    sscanf(dataSaida, "%d/%d/%d", &diaSaida, &mesSaida, &anoSaida);

    struct tm entrada = { .tm_year = anoEntrada - 1900, .tm_mon = mesEntrada - 1, .tm_mday = diaEntrada };
    struct tm saida = { .tm_year = anoSaida - 1900, .tm_mon = mesSaida - 1, .tm_mday = diaSaida };

    time_t entradaTime = mktime(&entrada);
    time_t saidaTime = mktime(&saida);

    double diferenca = difftime(saidaTime, entradaTime);
    return diferenca / (60 * 60 * 24);
}

int verificarDisponibilidadeQuarto(Reserva reservas[], int totalReservas, int numeroQuarto, const char* dataEntrada, const char* dataSaida) {
    for (int i = 0; i < totalReservas; i++) {
        if (reservas[i].numeroQuarto == numeroQuarto) {
            if ((strcmp(dataEntrada, reservas[i].dataEntrada) >= 0 && strcmp(dataEntrada, reservas[i].dataSaida) < 0) ||
                (strcmp(dataSaida, reservas[i].dataEntrada) > 0 && strcmp(dataSaida, reservas[i].dataSaida) <= 0) ||
                (strcmp(dataEntrada, reservas[i].dataEntrada) <= 0 && strcmp(dataSaida, reservas[i].dataSaida) >= 0)) {
                return 0;
            }
        }
    }
    return 1;
}

void cadastrarEstadia(Reserva reservas[], int *totalReservas, Cliente clientes[], int totalClientes, Quarto quartos[], int totalQuartos) {
    Reserva novaReserva;
    novaReserva.codigo = *totalReservas + 1;

    printf("Digite o código do cliente: ");
    scanf("%d", &novaReserva.codigoCliente);
    getchar();

    int clienteExiste = 0;
    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo == novaReserva.codigoCliente) {
            clienteExiste = 1;
            break;
        }
    }

    if (!clienteExiste) {
        printf("Cliente não cadastrado.\n");
        return;
    }

    printf("Digite a quantidade de hóspedes: ");
    int quantidadeHospedes;
    scanf("%d", &quantidadeHospedes);
    getchar();

    printf("Digite a data de entrada (ddmmyyyy): ");
    fgets(novaReserva.dataEntrada, sizeof(novaReserva.dataEntrada), stdin);
    novaReserva.dataEntrada[strcspn(novaReserva.dataEntrada, "\n")] = 0;

    printf("Digite a data de saída (ddmmyyyy): ");
    fgets(novaReserva.dataSaida, sizeof(novaReserva.dataSaida), stdin);
    novaReserva.dataSaida[strcspn(novaReserva.dataSaida, "\n")] = 0;

    novaReserva.numDiarias = calcularNumDiarias(novaReserva.dataEntrada, novaReserva.dataSaida);

    int quartoEncontrado = 0;
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].capacidade >= quantidadeHospedes && strcmp(quartos[i].status, "livre") == 0) {
            if (verificarDisponibilidadeQuarto(reservas, *totalReservas, quartos[i].numero, novaReserva.dataEntrada, novaReserva.dataSaida)) {
                novaReserva.numeroQuarto = quartos[i].numero;
                strcpy(quartos[i].status, "ocupado");
                quartoEncontrado = 1;
                break;
            }
        }
    }

    if (!quartoEncontrado) {
        printf("Nenhum quarto disponível para a quantidade de hóspedes ou nas datas informadas.\n");
        return;
    }

    reservas[*totalReservas] = novaReserva;
    (*totalReservas)++;
    printf("Estadia cadastrada com sucesso!\n");
}

void adicionarCliente(Cliente clientes[], int *totalClientes) {
    Cliente novoCliente;
    novoCliente.codigo = *totalClientes + 1;

    printf("Digite o nome do cliente: ");
    fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
    novoCliente.nome[strcspn(novoCliente.nome, "\n")] = 0;

    printf("Digite o endereço do cliente: ");
    fgets(novoCliente.endereco, sizeof(novoCliente.endereco), stdin);
    novoCliente.endereco[strcspn(novoCliente.endereco, "\n")] = 0;

    printf("Digite o telefone do cliente: ");
    fgets(novoCliente.telefone, sizeof(novoCliente.telefone), stdin);
    novoCliente.telefone[strcspn(novoCliente.telefone, "\n")] = 0;

    clientes[*totalClientes] = novoCliente;
    (*totalClientes)++;
    printf("Cliente cadastrado com sucesso! (ID: %d)\n",novoCliente.codigo);
}

void adicionarFuncionario(Funcionario funcionarios[], int *totalFuncionarios) {
    Funcionario novoFuncionario;
    novoFuncionario.codigo = *totalFuncionarios + 1;

    printf("Digite o nome do funcionário: ");
    fgets(novoFuncionario.nome, sizeof(novoFuncionario.nome), stdin);
    novoFuncionario.nome[strcspn(novoFuncionario.nome, "\n")] = 0;

    printf("Digite o telefone do funcionário: ");
    fgets(novoFuncionario.telefone, sizeof(novoFuncionario.telefone), stdin);
    novoFuncionario.telefone[strcspn(novoFuncionario.telefone, "\n")] = 0;

    printf("Digite o cargo do funcionário: ");
    fgets(novoFuncionario.cargo, sizeof(novoFuncionario.cargo), stdin);
    novoFuncionario.cargo[strcspn(novoFuncionario.cargo, "\n")] = 0;

    printf("Digite o salário do funcionário: ");
    scanf("%f", &novoFuncionario.salario);
    getchar();

    funcionarios[*totalFuncionarios] = novoFuncionario;
    (*totalFuncionarios)++;
    printf("Funcionário cadastrado com sucesso! (ID: %d)\n", novoFuncionario.codigo);
}

void adicionarQuarto(Quarto quartos[], int *totalQuartos) {
    Quarto novoQuarto;
    novoQuarto.numero = *totalQuartos + 1;

    printf("Digite a capacidade do quarto: ");
    scanf("%d", &novoQuarto.capacidade);
    getchar();

    printf("Digite o preço da diária do quarto: ");
    scanf("%f", &novoQuarto.precoDiaria);
    getchar();

    strcpy(novoQuarto.status, "livre");

    quartos[*totalQuartos] = novoQuarto;
    (*totalQuartos)++;
    printf("Quarto cadastrado com sucesso! (ID: %d)\n", novoQuarto.numero);
}

void listarEstadiasCliente(Reserva reservas[], int totalReservas, int codigoCliente) {
    printf("Estadias do Cliente:\n");
    for (int i = 0; i < totalReservas; i++) {
        if (reservas[i].codigoCliente == codigoCliente) {
            printf("Código da Reserva: %d\n", reservas[i].codigo);
            printf("Número do Quarto: %d\n", reservas[i].numeroQuarto);
            printf("Data de Entrada: %s\n", reservas[i].dataEntrada);
            printf("Data de Saída: %s\n", reservas[i].dataSaida);
            printf("Número de Diárias: %d\n", reservas[i].numDiarias);
            printf("--------------------\n");
            }
            }
            }

            void salvarClientes(Cliente clientes[], int totalClientes) {
            FILE *arquivo = fopen("clientes.dat", "wb");
            if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo clientes.dat\n");
            return;
            }

            fwrite(clientes, sizeof(Cliente), totalClientes, arquivo);
            fclose(arquivo);
            }

            void carregarClientes(Cliente clientes[], int *totalClientes) {
            FILE *arquivo = fopen("clientes.dat", "rb");
            if (arquivo == NULL) {
            printf("Arquivo clientes.dat não encontrado. Criando novo arquivo...\n");
            return;
            }

            *totalClientes = fread(clientes, sizeof(Cliente), MAX_CLIENTES, arquivo);
            fclose(arquivo);
            }

            void salvarFuncionarios(Funcionario funcionarios[], int totalFuncionarios) {
            FILE *arquivo = fopen("funcionarios.dat", "wb");
            if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo funcionarios.dat\n");
            return;
            }

            fwrite(funcionarios, sizeof(Funcionario), totalFuncionarios, arquivo);
            fclose(arquivo);
            }

            void carregarFuncionarios(Funcionario funcionarios[], int *totalFuncionarios) {
            FILE *arquivo = fopen("funcionarios.dat", "rb");
            if (arquivo == NULL) {
            printf("Arquivo funcionarios.dat não encontrado. Criando novo arquivo...\n");
            return;
            }

            *totalFuncionarios = fread(funcionarios, sizeof(Funcionario), MAX_FUNCIONARIOS, arquivo);
            fclose(arquivo);
            }

            void salvarQuartos(Quarto quartos[], int totalQuartos) {
            FILE *arquivo = fopen("quartos.dat", "wb");
            if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo quartos.dat\n");
            return;
            }

            fwrite(quartos, sizeof(Quarto), totalQuartos, arquivo);
            fclose(arquivo);
            }

            void carregarQuartos(Quarto quartos[], int *totalQuartos) {
            FILE *arquivo = fopen("quartos.dat", "rb");
            if (arquivo == NULL) {
            printf("Arquivo quartos.dat não encontrado. Criando novo arquivo...\n");
            return;
            }

            *totalQuartos = fread(quartos, sizeof(Quarto), MAX_QUARTOS, arquivo);
            fclose(arquivo);
            }

            void salvarReservas(Reserva reservas[], int totalReservas) {
            FILE *arquivo = fopen("reservas.dat", "wb");
            if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo reservas.dat\n");
            return;
            }

            fwrite(reservas, sizeof(Reserva), totalReservas, arquivo);
            fclose(arquivo);
            }

            void carregarReservas(Reserva reservas[], int *totalReservas) {
            FILE *arquivo = fopen("reservas.dat", "rb");
            if (arquivo == NULL) {
            printf("Arquivo reservas.dat não encontrado. Criando novo arquivo...\n");
            return;
            }

            *totalReservas = fread(reservas, sizeof(Reserva), MAX_RESERVAS, arquivo);
            fclose(arquivo);
            }

            int main() {
            setlocale(LC_ALL, "Portuguese");

            Cliente clientes[MAX_CLIENTES];
            Funcionario funcionarios[MAX_FUNCIONARIOS];
            Quarto quartos[MAX_QUARTOS];
            Reserva reservas[MAX_RESERVAS];

            int totalClientes = 0;
            int totalFuncionarios = 0;
            int totalQuartos = 0;
            int totalReservas = 0;

            carregarClientes(clientes, &totalClientes);
            carregarFuncionarios(funcionarios, &totalFuncionarios);
            carregarQuartos(quartos, &totalQuartos);
            carregarReservas(reservas, &totalReservas);

            int opcao;
            do {
            printf("\n==== Software Hotel Paragem ====\n");
            printf("1. Cadastrar Cliente\n");
            printf("2. Cadastrar Funcionário\n");
            printf("3. Cadastrar Quarto\n");
            printf("4. Cadastrar Estadia\n");
            printf("5. Listar Estadias de um Cliente\n");
            printf("6. Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao);
            getchar();

            switch (opcao) {
            case 1:
                adicionarCliente(clientes, &totalClientes);
                salvarClientes(clientes, totalClientes);
                break;
            case 2:
                adicionarFuncionario(funcionarios, &totalFuncionarios);
                salvarFuncionarios(funcionarios, totalFuncionarios);
                break;
            case 3:
                adicionarQuarto(quartos, &totalQuartos);
                salvarQuartos(quartos, totalQuartos);
                break;
            case 4:
                cadastrarEstadia(reservas, &totalReservas, clientes, totalClientes, quartos, totalQuartos);
                salvarReservas(reservas, totalReservas);
                salvarQuartos(quartos, totalQuartos); 
                break;
            case 5: {
                int codigoCliente;
                printf("Digite o código do cliente: ");
                scanf("%d", &codigoCliente);
                listarEstadiasCliente(reservas, totalReservas, codigoCliente);
                break;
            }
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            } while (opcao != 6);

            return 0;
            }
