#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

typedef struct{
    char nome[50];
    char email[50];
    char cpf[20];
    char dataNascimento[20];
    char dataCadastro[20];
    int codigo;
}Cliente;

typedef struct{
    int numero;
    Cliente cliente;
    float saldo;
    float limite;
    float saldoTotal; //saldo+limite
}Conta;

static Conta contas[50];
static int contador_contas = 0;
static int contador_clientes = 0;

void menu();
void infoCliente(Cliente conta);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
float atualizaSaldoTotal(Conta conta);
Conta buscarContaPorNumero(int numero);
void sacar(Conta conta, float valor);
void depositar(Conta conta, float valor);
void transferir(Conta contaOrigem, Conta contaDestino, float valor);


int main()
{   printf("\t>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("\t>>>>>>>>>>| JSC |>>>>>>>>>\n");
    printf("\t>>>>>>>>>>|BANK |>>>>>>>>>\n");
    printf("\t>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");

    menu();
    return 0;
}

void menu(){
    printf("Selecione uma opcao do menu:\n");
    printf("1 - Criar Conta.\n");
    printf("2 - Efetuar Saque.\n");
    printf("3 - Efetuar Deposito.\n");
    printf("4 - Efetuar Transferencia.\n");
    printf("5 - Listar Contas.\n");
    printf("6 - Sair do Sistema.\n");

    int op = 0;
    scanf("%d", &op);
    getchar();

    switch (op){
        case 1:
            criarConta();
            break;
        case 2:
            efetuarSaque();
            break;
        case 3:
            efetuarDeposito();
            break;
        case 4:
            efetuarTransferencia();
            break;
        case 5:
            listarContas();
            break;
        case 6:
            printf("Até a proxima!!");
            Sleep(2);
            exit(0);
        default:
            printf("Opcao invalida. Tente outra opcao por favor!!");
            Sleep(2);
            menu();
            break;
    }
}

void criarConta(){

    Cliente cliente;

    //Data de Cadastro
    char dia[3];
    char mes[3];
    char ano[5];
    char data_cadastro[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //dia
    if(tm.tm_mday < 10){   //tm_mday é o nosso dia
        sprintf(dia, "0%d", tm.tm_mday);
    }else{
        sprintf(dia, "%d", tm.tm_mday);
    }

    //mes
    if(tm.tm_mon + 1 < 10){   //tm_mon é o nosso mes
        sprintf(mes, "0%d", tm.tm_mon+1);
    }else{
        sprintf(mes, "%d", tm.tm_mon+1);
    }

    //ano
    sprintf(ano, "%d", tm.tm_year + 1900);

    strcpy(data_cadastro, "");
    strcat(data_cadastro, dia);  //09
    strcat(data_cadastro, "/"); //09/
    strcat(data_cadastro, mes); //09/06
    strcat(data_cadastro, "/"); //09/06/
    strcat(data_cadastro, ano); //09/06/2021
    strcat(data_cadastro, "\0"); //finaliza a string
    strcpy(cliente.dataCadastro, data_cadastro);

    //criando cliente
    printf("Por favor, informe os seus dados:\n");
    cliente.codigo = contador_clientes + 1;

    printf("Qual o seu nome?\n");
    fgets(cliente.nome, 50, stdin);

    printf("Qual o seu email?\n");
    fgets(cliente.email, 50, stdin);

    printf("Qual o sua data de nascimento?\n");
    fgets(cliente.dataNascimento, 20, stdin);

    printf("Qual o seu cpf?\n");
    fgets(cliente.cpf, 20, stdin);
    contador_clientes++;

    //criar conta
    contas[contador_contas].numero = contador_contas + 1;
    contas[contador_contas].cliente = cliente;
    contas[contador_contas].saldo = 0.0;
    contas[contador_contas].limite = 0.0;
    contas[contador_contas].saldoTotal = atualizaSaldoTotal(contas[contador_contas]);

    printf("Conta criada com Sucesso!!\n\n");
    printf("Dados da conta criada:\n\n");
    infoConta(contas[contador_contas]);
    contador_contas++;
    Sleep(3);
    menu();

}

void infoCliente(Cliente cliente){
	printf("Código: %d \nNome: %s \nData de Nascimento: %s \nCadastro: %s\n",
			cliente.codigo, strtok(cliente.nome, "\n"), strtok(cliente.dataNascimento, "\n"),
			strtok(cliente.dataCadastro, "\n"));
}
void infoConta(Conta conta){
	printf("Número da conta: %d \nCliente: %s \nData Nascimento: %s \nData Cadastro: %s \nSaldo Total: %.2f\n",
			conta.numero, strtok(conta.cliente.nome, "\n"), strtok(conta.cliente.dataNascimento, "\n"),
			strtok(conta.cliente.dataCadastro, "\n"), conta.saldoTotal);
}

void efetuarSaque(){
    if(contador_contas > 0){
        int numero;
        printf("Informe o número da conta:\n");
        scanf("%d", &numero);

        Conta conta = buscarContaPorNumero(numero);

        if(conta.numero == numero){
            float valor;
            printf("Qual valor voce deseja sacar?");
            scanf("%f", &valor);

            sacar(conta, valor);
        }else{
           printf("Nao foi encontrada uma conta com o numero %d.", numero);
        }

    }else{
        printf("Crie uma conta para realizar saques!!");
    }
    Sleep(3);
    menu();
}

void efetuarDeposito(){
    if(contador_contas > 0){
       int numero;
        printf("Informe o número da conta:\n");
        scanf("%d", &numero);

        Conta conta = buscarContaPorNumero(numero);

        if(conta.numero == numero){
            float valor;
            printf("Qual valor voce deseja depositar?");
            scanf("%f", &valor);

            depositar(conta, valor);
        }else{
           printf("Nao foi encontrada uma conta com o numero %d.", numero);
        }

    }else{
        printf("Crie uma conta para realizar depositos!!");
    }
    Sleep(3);
    menu();
    }
void efetuarTransferencia(){
    if(contador_contas > 0){
        int numeroOrigem, numeroDestino;
        printf("Informe o número da sua conta:\n");
        scanf("%d", &numeroOrigem);

        Conta conta_origem = buscarContaPorNumero(numeroOrigem);
        if(conta_origem.numero == numeroOrigem){
            printf("Informe o número da conta de destino:\n");
            scanf("%d", &numeroDestino);

            Conta contaDestino = buscarContaPorNumero(numeroDestino);
            if(contaDestino.numero = numeroDestino){
                float valor;
                 printf("Qual valor voce deseja transferir?");
                 scanf("%f", &valor);

                transferir(conta_origem, contaDestino, valor);
            }else{
                printf("A conta destino com numero %d nao foi encontrada.\n", numeroDestino);
            }
        }else{
            printf("A conta com numero %d nao foi encontrada.\n", numeroOrigem);
        }
    }else{
        printf("Crie uma conta para realizar transferencias!!");
    }
    Sleep(3);
    menu();
}

void listarContas(){
    if(contador_contas > 0){
        printf("\tListagem de Contas:\n");
        for(int i = 0; i < contador_contas; i++){
            infoConta(contas[i]);
            printf("----------------------------------\n");
            Sleep(2);
        }
        Sleep(2);
        menu();
    }else {
        printf("Ainda nao tem contas cadastradas. Cadastre agora:\n\n");
    }
    Sleep(3);
    menu();
}

float atualizaSaldoTotal(Conta conta){
    return conta.saldo + conta.limite;
}

Conta buscarContaPorNumero(int numero){
    Conta c;
    if(contador_contas > 0){
        for(int i = 0; i < contador_contas; i++){
            if(contas[i].numero == numero){
                c = contas[i];
            }
        }
    }
    return c;
}

void sacar(Conta conta, float valor){
    if(valor > 0 && conta.saldoTotal >= valor){
       for(int i = 0; i < contador_contas; i++){
        if(contas[i].numero == conta.numero){
            if(contas[i].saldo >= valor){
                contas[i].saldo = contas[i].saldo - valor;
                contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                printf("Saque efetuado com sucesso!!\n");
            }else{
                float restante = contas[i].saldo - valor;
                contas[i].limite =  contas[i].limite + restante;
                contas[i].saldo = 0.0;
                contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                printf("Saque efetuado com sucesso!!\n");
            }
          }
       }
    }else{
        printf("Saque nao realizado\n");
    }
}

void depositar(Conta conta, float valor){
    if(valor > 0){
        for(int i = 0; i < contador_contas; i++){
            if(contas[i].numero == conta.numero){
                contas[i].saldo = contas[i].saldo + valor;
                contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
                printf("Deposito efetuado com sucesso!!\n");
            }
        }
    }else{
        printf("Deposito nao realizado!!\n");
    }
}

void transferir(Conta contaOrigem, Conta contaDestino, float valor){
    if(valor > 0 && contaOrigem.saldoTotal >= valor){
        for(int co = 0; co < contador_contas; co++){
            if(contas[co].numero == contaOrigem.numero){
                for(int cd = 0; cd < contador_contas; cd++){
                    if(contas[cd].numero == contaDestino.numero){
                       if(contas[co].saldo >= valor){
                        contas[co].saldo = contas[co].saldo - valor;
                        contas[cd].saldo = contas[co].saldo + valor;
                        contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
                        contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
                        printf("Transferencia efetuada com sucesso!!\n");
                    }else{
                        float restante = contas[co].saldo - valor;
                        contas[co].limite =  contas[co].limite + restante;
                        contas[co].saldo = 0.0;
                        contas[cd].saldo = contas[cd].saldo + valor;
                        contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
                        contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
                        printf("Transferencia efetuada com sucesso!!\n");
                    }
                }
            }
        }
    }
    }else{
        printf("Nao foi possivel realizar a transferencia\n");
    }
}
