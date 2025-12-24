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
    int i, size = sizeof(struct Usuario);

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

    gotoxy(82,10);  gets(auxEmail);
    while(strlen(auxEmail) == 0){
        gotoxy(78, 14);
        setColor(VERMELHO);
        printf("Erro: Email nao pode ser vazio! tente novamente.");
        setColor(BRANCO);
        gotoxy(82,10);  gets(auxEmail);
        gotoxy(78, 14); printf("                                                ");
    }
    if (buscaEmail(arq, auxEmail) != -1){
        gotoxy(79,16);
        setColor(VERMELHO);
        printf("Erro: Email ja cadastrado! tente novamente.");
        gotoxy(79,21);
        setColor(AMARELINHO);
        system("pause");
        setColor(BRANCO);
        addUser(arq);
    }

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

void listUsers(FILE *arq) {
    struct Usuario u;
    int linha = 11;

    if (arq == NULL) return;

    system("cls");
    desenharCaixa(10, 3, 70, 20);
    gotoxy(40,5); printf("LISTA DE USUARIOS");

    gotoxy(12,9);
    printf("ID   Nome                     Email                       Data");

    rewind(arq);
    fread(&u, sizeof(u), 1, arq);
    while (!feof(arq) && u.ativo) {
            gotoxy(12, linha++);
            setColor(5);
            printf("%-4d %-25s %-25s %-12s", u.id, u.nome, u.email, u.data_nascimento);
            setColor(BRANCO);
            fread(&u, sizeof(u), 1, arq);
    }

    gotoxy(30, linha + 2);
    system("pause");
}

void buscaNome(FILE *arq) {
    struct Usuario u;
    char nomeBusca[50];

    system("cls");
    desenharCaixa(20, 3, 50, 10);
    gotoxy(35, 5); printf("BUSCAR USUARIO POR NOME");
    gotoxy(25, 7); printf("Nome: ");
    gets(nomeBusca);

    rewind(arq);
    fread(&u, sizeof(u), 1, arq);
    while (!feof(arq) && strcmp(u.nome, nomeBusca) != 0 && u.ativo) fread(&u, sizeof(u), 1, arq);

    if (!feof(arq) && strcmp(u.nome, nomeBusca) == 0) {
        gotoxy(25, 9);
        printf("Usuario encontrado:");
        gotoxy(25, 11);
        printf("Nome: %s", u.nome);
        gotoxy(25, 12);
        printf("Email: %s", u.email);
        gotoxy(25, 13);
        printf("Data Nasc.: %s", u.data_nascimento);
    } else {
        gotoxy(25, 9);
        printf("Usuario nao encontrado.");
    }

    gotoxy(30, 15);
    system("pause");
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