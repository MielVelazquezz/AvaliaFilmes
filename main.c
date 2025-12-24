#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define LARANJA 5
#define BRANCO 7
#define AMARELINHO  10 
#define VERMELHO 12
#define CINZA 8

struct Usuario{
    int id, ativo;
    char nome[50], email[90], senha[20], data_nascimento[15];
};

struct Filme{
    int id, ativo;
    char titulo[100], genero[50], diretor[50];
    int ano_lancamento, duracao_minutos;
};

struct Avaliacao{
    int id, id_usuario, id_filme, nota, ativo;
    char comentario[255];
    char data_avaliacao[15];
};

void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void desenharCaixa(int x, int y, int largura, int altura){
    int i;

    // linha superior
    gotoxy(x, y);
    printf("%c", 201);
    for (i = 0; i < largura - 2; i++) printf("%c", 205);
    printf("%c", 187);

    // laterais
    for (i = 1; i < altura - 1; i++){
        gotoxy(x, y + i);
        printf("%c", 186);
        gotoxy(x + largura - 1, y + i);
        printf("%c", 186);
    }

    // linha inferior
    gotoxy(x, y + altura - 1);
    printf("%c", 200);
    for (i = 0; i < largura - 2; i++)
        printf("%c", 205);
    printf("%c", 188);
}

int emailValido(char email[90]){
    int tam = strlen(email);

    if (tam < 10) return 0;
    if (strcmp(&email[tam - 10], "@gmail.com") == 0) return 1;
    if (strcmp(&email[tam - 12], "@hotmail.com") == 0) return 1;

    return 0;
}


int buscaEmail(FILE *arq, char auxEmail[90]){
    struct Usuario u;
    int size = sizeof(struct Usuario);

    rewind(arq);
    while (fread(&u, size, 1, arq) == 1) if (u.ativo && strcmp(u.email, auxEmail) == 0) return ftell(arq) - size;

    return -1;
}

void addUser(FILE *arq) {
    struct Usuario u;
    char auxEmail[90];
    int i, size = sizeof(struct Usuario), emailOk = 0;

    if (arq == NULL) arq = fopen("usuarios.dat", "wb+");
    fflush(arq);
    fseek(arq, 0, SEEK_END);
    u.id = ftell(arq) / size;
    u.ativo = 1;

    system("cls");
    desenharCaixa(20, 3, 50, 20);

    gotoxy(37, 5);
    printf("GERENCIAR USUARIOS");
    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++)
    {
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);

    gotoxy(31, 9);
    setColor(LARANJA);
    printf("1 - Cadastrar Usuario");
    setColor(BRANCO);
    gotoxy(31, 10);
    printf("2 - Listar Usuarios");
    gotoxy(31, 11);
    printf("3 - Buscar Usuario por nome");
    gotoxy(31, 12);
    printf("4 - Buscar Usuario por ID");
    gotoxy(31, 13);
    printf("5 - Editar Usuario");
    gotoxy(31, 14);
    printf("6 - Deletar Usuario");
    gotoxy(31, 15);
    printf("0 - Voltar ao Menu Principal");
    desenharCaixa(69, 3, 65, 20);

    gotoxy(93, 5); printf("CADASTRAR USUARIO");

    gotoxy(75, 9);  printf("Nome: ");
    gotoxy(75,10);  printf("Email: ");
    gotoxy(75,11);  printf("Senha: ");
    gotoxy(75,12);  printf("Data Nasc.: ");

    gotoxy(81, 9);  gets(u.nome);
    while(strlen(u.nome) == 0){
        gotoxy(78, 14);
        setColor(VERMELHO);
        printf("Erro: Nome nao pode ser vazio! tente novamente.");
        setColor(BRANCO);
        gotoxy(81, 9);  gets(u.nome);
        gotoxy(78, 14); printf("                                               ");
    }

    do {
        gotoxy(82,10);
        gets(auxEmail);

        if (strlen(auxEmail) == 0){
            gotoxy(78,14);
            setColor(VERMELHO);
            printf("Erro: Email nao pode ser vazio!");
            emailOk = 0;
        }
        else if (!emailValido(auxEmail)){
            gotoxy(78,14);
            setColor(VERMELHO);
            printf("Erro: Use @gmail.com ou @hotmail.com");
            emailOk = 0;
        }
        else if (buscaEmail(arq, auxEmail) != -1){
            gotoxy(78,14);
            setColor(VERMELHO);
            printf("Erro: Email ja cadastrado!");
            emailOk = 0;
        }
        else{
            emailOk = 1; // email valido
        }

        setColor(BRANCO);
        Sleep(1500);
        gotoxy(78,14);
        printf("                                               ");

    } while (emailOk == 0);


    gotoxy(82,11);  gets(u.senha);
    while(strlen(u.senha) < 6){
        gotoxy(78, 14);
        setColor(VERMELHO);
        printf("Erro: Senha deve ter no minimo 6 caracteres!");
        setColor(BRANCO);
        gotoxy(82,11);  gets(u.senha);
        gotoxy(78, 14); printf("                                            ");
    }

    gotoxy(87,12);  gets(u.data_nascimento);
    while (strlen(u.data_nascimento) == 0){
        gotoxy(78, 14);
        setColor(VERMELHO);
        printf("Erro: Data de nascimento nao pode ser vazia!");
        setColor(BRANCO);
        gotoxy(87,12);  gets(u.data_nascimento);
        gotoxy(78, 14); printf("                                            ");
    }

    strcpy(u.email, auxEmail);
    fwrite(&u, size, 1, arq);

    gotoxy(86,18);
    setColor(CINZA);
    printf("Usuario cadastrado com sucesso!");
    gotoxy(81,21);
    setColor(AMARELINHO);
    system("pause");
    setColor(BRANCO);
}

void limparArea(int x, int y, int largura, int altura){
    int i, j;
    for (i = 0; i < altura; i++){
        gotoxy(x, y + i);
        for (j = 0; j < largura; j++)
            printf(" ");
    }
}

void listUsers(FILE *arq) {
    struct Usuario u;
    int i, size = sizeof(struct Usuario);

    int linhaInicial=9, linhaFinal=19, linhaAtual;
    if (arq == NULL) return;

    system("cls");
    desenharCaixa(20, 3, 50, 20);

    gotoxy(37, 5);
    printf("GERENCIAR USUARIOS");

    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++){
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);

    gotoxy(31, 9);
    printf("1 - Cadastrar Usuario");

    gotoxy(31, 10);
    setColor(LARANJA);
    printf("2 - Listar Usuarios");
    setColor(BRANCO);

    gotoxy(31, 11); printf("3 - Buscar Usuario por nome");
    gotoxy(31, 12); printf("4 - Buscar Usuario por ID");
    gotoxy(31, 13); printf("5 - Editar Usuario");
    gotoxy(31, 14); printf("6 - Deletar Usuario");
    gotoxy(31, 15); printf("0 - Voltar ao Menu Principal");
    desenharCaixa(69, 3, 80, 20);

    gotoxy(97, 5);
    printf("LISTA DE USUARIOS");

    gotoxy(71, 7);
    printf("ID   Nome                     Email                       Data de Nasc.");

    gotoxy(71, 8);
    for (i = 0; i < 76; i++) printf("%c", 205);

    rewind(arq);
    linhaAtual = linhaInicial;

    while (fread(&u, size, 1, arq) == 1){
        if (!u.ativo) continue;

        gotoxy(71, linhaAtual++);
        printf("%-4d %-24s %-27s %-12s",u.id, u.nome, u.email, u.data_nascimento);

        if (linhaAtual > linhaFinal){
            gotoxy(92, 21);
            setColor(AMARELINHO);
            printf("Qualquer tecla -> proxima pagina");
            setColor(BRANCO);
            getch();

            limparArea(71, linhaInicial, 76, linhaFinal - linhaInicial + 1);

            gotoxy(92, 21);
            printf("                      ");

            linhaAtual = linhaInicial;
        }
    }

    gotoxy(92, 21);
    setColor(AMARELINHO);
    system("pause");
    setColor(BRANCO);
}



void buscaNome(FILE *arq) {
    struct Usuario u;
    char nomeBusca[50];
    int i, size = sizeof(struct Usuario);

    int linhaInicial = 9;
    int linhaFinal   = 19;
    int linhaAtual;
    int encontrou = 0;

    if (arq == NULL) return;

    system("cls");
    desenharCaixa(20, 3, 50, 20);
    gotoxy(37, 5);
    printf("GERENCIAR USUARIOS");

    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++){
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);

    gotoxy(31, 9);  printf("1 - Cadastrar Usuario");
    gotoxy(31, 10); printf("2 - Listar Usuarios");

    gotoxy(31, 11);
    setColor(LARANJA);
    printf("3 - Buscar Usuario por nome");
    setColor(BRANCO);

    gotoxy(31, 12); printf("4 - Buscar Usuario por ID");
    gotoxy(31, 13); printf("5 - Editar Usuario");
    gotoxy(31, 14); printf("6 - Deletar Usuario");
    gotoxy(31, 15); printf("0 - Voltar ao Menu Principal");

    /* ===== CAIXA RESULTADOS ===== */
    desenharCaixa(69, 3, 80, 20);

    gotoxy(102, 5);
    printf("BUSCAR POR NOME");

    gotoxy(71, 7);
    printf("Nome buscado: ");
    gets(nomeBusca);

    gotoxy(71, 8);
    printf("ID   Nome                     Email                       Data de Nasc.");

    gotoxy(71, 9);
    for (i = 0; i < 76; i++) printf("%c", 205);

    rewind(arq);
    linhaAtual = linhaInicial + 1;

    while (fread(&u, size, 1, arq) == 1){
        if (u.ativo && strcmp(u.nome, nomeBusca) == 0){
            encontrou = 1;

            gotoxy(71, linhaAtual++);
            printf("%-4d %-24s %-27s %-12s",u.id, u.nome, u.email, u.data_nascimento);
            if (linhaAtual > linhaFinal){
                gotoxy(92, 21);
                setColor(AMARELINHO);
                printf("ENTER - Proxima pagina");
                setColor(BRANCO);

                getch();

                limparArea(71, linhaInicial + 1, 76,linhaFinal - linhaInicial);

                gotoxy(92, 21);
                printf("                      ");

                linhaAtual = linhaInicial + 1;
            }
        }
    }

    if (!encontrou){
        gotoxy(85, 13);
        setColor(VERMELHO);
        printf("Nenhum usuario encontrado");
        setColor(BRANCO);
    }

    gotoxy(92, 21);
    setColor(AMARELINHO);
    system("pause");
    setColor(BRANCO);
}


void menuUsers(){
    FILE *arqU = fopen("usuarios.dat", "rb+");
    int op, i;

    if(arqU == NULL) arqU = fopen("usuarios.dat", "wb+");

    do
    {
        system("cls");
        desenharCaixa(20, 3, 50, 20);

        gotoxy(37, 5);
        printf("GERENCIAR USUARIOS");
        gotoxy(20, 7);
        printf("%c", 204);
        for (i = 0; i <= 47; i++)
        {
            gotoxy(21 + i, 7);
            printf("%c", 205);
        }
        printf("%c", 185);

        gotoxy(31, 9);
        printf("1 - Cadastrar Usuario");
        gotoxy(31, 10);
        printf("2 - Listar Usuarios");
        gotoxy(31, 11);
        printf("3 - Buscar Usuario por nome");
        gotoxy(31, 12);
        printf("4 - Buscar Usuario por ID");
        gotoxy(31, 13);
        printf("5 - Editar Usuario");
        gotoxy(31, 14);
        printf("6 - Deletar Usuario");
        gotoxy(31, 15);
        printf("0 - Voltar ao Menu Principal");
        gotoxy(31, 17);
        printf("Escolha uma opcao: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op)
        {
        case 0:
            fclose(arqU);
            menu();
            break;

        case 1:
            addUser(arqU);
            fclose(arqU);
            menuUsers();
            break;

        case 2:
            listUsers(arqU);
            fclose(arqU);
            menuUsers();
            break;

        case 3:
            buscaNome(arqU);
            fclose(arqU);
            menuUsers();
            break;

        case 4:
            // buscaID();
            break;

        case 5:
            // editUser();
            break;

        case 6:
            // deleteUser();
            break;

        default:
            gotoxy(85, 12);
            printf("Opcao invalida!");
            Sleep(1000);
            menuUsers();
        }
    }while (op != 0);
    
}

void menuF(){
    int op, i;

    system("cls");
    desenharCaixa(20, 3, 50, 20);

    gotoxy(38, 5);
    printf("GERENCIAR FILMES");
    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++){
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);
    gotoxy(25, 9);
    printf("1 - Cadastrar Filme");
    gotoxy(25, 10);
    printf("2 - Listar Filmes");
    gotoxy(25, 11);
    printf("3 - Buscar Filme por titulo");
    gotoxy(25, 12);
    printf("4 - Buscar Filme por ano de lancamento");
    gotoxy(25, 13);
    printf("5 - Buscar Filme por ID");
    gotoxy(25, 14);
    printf("6 - Editar Filme");
    gotoxy(25, 15);
    printf("7 - Deletar Filme");
    gotoxy(25, 16);
    printf("0 - Voltar ao Menu Principal");
    gotoxy(25, 18);
    printf("Escolha uma opcao: ");
    scanf("%d", &op);
    fflush(stdin);

    switch (op){
    case 0:
        menu();
        break;

    case 1:
        // addFilme();
        break;

    case 2:
        // listFilmes();
        break;

    case 3:
        // buscaTitulo();
        break;

    case 4:
        // buscaAno();
        break;

    case 5:
        // buscaIDFilme();
        break;

    case 6:
        // editFilme();
        break;

    case 7:
        // deleteFilme();
        break;

    default:
        gotoxy(85, 12);
        printf("Opcao invalida!");
        Sleep(1000);
        menuF();
    }
}

void menuAv(){
    int op, i;

    system("cls");
    desenharCaixa(20, 3, 50, 20);

    gotoxy(36, 5);
    printf("GERENCIAR AVALIACOES");
    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++){
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);

    gotoxy(28, 9);
    printf("1 - Avaliar");
    gotoxy(28, 10);
    printf("2 - Listar Avaliacoes");
    gotoxy(28, 11);
    printf("3 - Buscar Avaliacao por Usuario");
    gotoxy(28, 12);
    printf("4 - Buscar Avaliacao por Filme");
    gotoxy(28, 13);
    printf("5 - Editar Avaliacao");
    gotoxy(28, 14);
    printf("6 - Deletar Avaliacao");
    gotoxy(28, 15);
    printf("0 - Voltar ao Menu Principal");
    gotoxy(28, 17);
    printf("Escolha uma opcao: ");
    scanf("%d", &op);
    fflush(stdin);


    switch (op){
    case 0:
        menu();
        break;

    case 1:
        // addAvaliacao();
        break;

    case 2:
        // listAvaliacoes();
        break;

    case 3:
        // buscaAvaliacaoUsuario();
        break;

    case 4:
        // buscaAvaliacaoFilme();
        break;

    case 5:
        // editAvaliacao();
        break;

    case 6:
        // deleteAvaliacao();
        break;

    default:
        gotoxy(85, 12);
        printf("Opcao invalida!");
        Sleep(1000);
        menuAv();
    }
}

void menu(){
    int op, i;

    system("cls");
    desenharCaixa(20, 3, 50, 20);

    gotoxy(38, 5);
    printf("MENU PRINCIPAL");
    gotoxy(20, 7);
    printf("%c", 204);
    for (i = 0; i <= 47; i++){
        gotoxy(21 + i, 7);
        printf("%c", 205);
    }
    printf("%c", 185);

    gotoxy(31, 9);
    printf("1 - Gerenciar Usuarios");
    gotoxy(31, 10);
    printf("2 - Gerenciar Filmes");
    gotoxy(31, 11);
    printf("3 - Gerenciar Avaliacoes");
    gotoxy(31, 12);
    printf("0 - Sair");
    gotoxy(31, 14);
    printf("Escolha uma opcao: ");
    scanf("%d", &op);
    fflush(stdin);


    switch (op){
    case 0:
        gotoxy(82, 12);
        printf("Encerrando o programa...");
        Sleep(1000);
        exit(0);

    case 1:
        menuUsers();
        break;

    case 2:
        menuF();
        break;

    case 3:
        menuAv();
        break;

    default:
        gotoxy(85, 12);
        printf("Opcao invalida!");
        Sleep(1000);
        menu();
    }
}

int main()
{
    menu();
    return 0;
}