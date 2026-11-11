#include <stdio.h>
#include <string.h>

#define MAX_ACOES 100

#define PLANEJADA 1
#define REALIZADA 2
#define CANCELADA 3

typedef struct {
    int codigo;
    char escola[50];
    char tema[50];
    char data[15];
    char publico[50];
    char responsavel[50];
    int previstos;
    int efetivos;
    int situacao;
} Acao;

int lerInteiro(char mensagem[], int *valor)
{
    char linha[50];

    printf("%s", mensagem);
    fgets(linha, 50, stdin);
    return sscanf(linha, "%d", valor) == 1;
}

void lerTexto(char mensagem[], char texto[])
{
    printf("%s", mensagem);
    fgets(texto, 50, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

char *nomeSituacao(int situacao)
{
    if (situacao == PLANEJADA) return "PLANEJADA";
    if (situacao == REALIZADA) return "REALIZADA";
    return "CANCELADA";
}

int buscarPorCodigo(Acao acoes[], int total, int codigo)
{
    int i;

    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codigo) return i;
    }
    return -1;
}

void exibirMenu(void)
{
    printf("\n========== MENTE EM ACAO ==========\n");
    printf("1 - Cadastrar acao\n");
    printf("2 - Listar acoes\n");
    printf("3 - Pesquisar acao\n");
    printf("4 - Atualizar situacao\n");
    printf("5 - Registrar participantes\n");
    printf("6 - Gerar resumo\n");
    printf("0 - Sair\n");
}

void exibirAcao(Acao a)
{
    printf("\n----------------------------------------\n");
    printf("Codigo       : %d\n", a.codigo);
    printf("Escola       : %s\n", a.escola);
    printf("Tema         : %s\n", a.tema);
    printf("Data prevista: %s\n", a.data);
    printf("Publico-alvo : %s\n", a.publico);
    printf("Responsavel  : %s\n", a.responsavel);
    printf("Previstos    : %d\n", a.previstos);
    if (a.situacao == REALIZADA) {
        printf("Participantes: %d\n", a.efetivos);
    }
    printf("Situacao     : %s\n", nomeSituacao(a.situacao));
}

void cadastrarAcao(Acao acoes[], int *total)
{
    Acao nova;

    printf("\n===== CADASTRO DE ACAO =====\n");

    if (*total >= MAX_ACOES) {
        printf("Limite de %d acoes atingido. Cadastro nao realizado.\n", MAX_ACOES);
        return;
    }
    if (!lerInteiro("Codigo da acao: ", &nova.codigo) || nova.codigo <= 0) {
        printf("Codigo invalido! Digite um numero maior que zero.\n");
        return;
    }
    if (buscarPorCodigo(acoes, *total, nova.codigo) != -1) {
        printf("Codigo duplicado! Cadastro nao realizado.\n");
        return;
    }

    lerTexto("Nome da escola: ", nova.escola);
    lerTexto("Tema da acao: ", nova.tema);
    lerTexto("Data prevista (dd/mm/aaaa): ", nova.data);
    lerTexto("Publico-alvo: ", nova.publico);
    lerTexto("Responsavel pela acao: ", nova.responsavel);
    if (!lerInteiro("Quantidade prevista de participantes: ", &nova.previstos)) {
        nova.previstos = 0;
    }

    if (strlen(nova.escola) == 0 || strlen(nova.tema) == 0 ||
        strlen(nova.data) == 0 || strlen(nova.publico) == 0 ||
        strlen(nova.responsavel) == 0) {
        printf("\nDados invalidos! Todos os campos sao obrigatorios.\n");
        return;
    }
    if (nova.previstos <= 0) {
        printf("\nQuantidade invalida! Deve ser maior que zero.\n");
        return;
    }

    nova.efetivos = 0;
    nova.situacao = PLANEJADA;
    acoes[*total] = nova;
    (*total)++;
    printf("\nCadastro realizado com sucesso!\n");
    printf("Situacao inicial: PLANEJADA\n");
}

void listarAcoes(Acao acoes[], int total)
{
    int i;

    printf("\n===== LISTA DE ACOES =====\n");
    if (total == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }
    for (i = 0; i < total; i++) {
        exibirAcao(acoes[i]);
    }
}

void pesquisarAcao(Acao acoes[], int total)
{
    int criterio, codigo, pos, i, achou = 0;
    char texto[50];

    printf("\n===== PESQUISAR ACAO =====\n");
    printf("1 - Por codigo\n");
    printf("2 - Por escola\n");
    printf("3 - Por tema\n");
    if (!lerInteiro("Escolha o criterio: ", &criterio) || criterio < 1 || criterio > 3) {
        printf("Criterio invalido!\n");
        return;
    }

    if (criterio == 1) {
        if (!lerInteiro("Informe o codigo: ", &codigo)) {
            printf("Codigo invalido!\n");
            return;
        }
        pos = buscarPorCodigo(acoes, total, codigo);
        if (pos == -1) {
            printf("Registro inexistente: nenhuma acao com o codigo %d.\n", codigo);
        } else {
            exibirAcao(acoes[pos]);
        }
        return;
    }

    lerTexto("Informe o texto a pesquisar: ", texto);
    if (strlen(texto) == 0) {
        printf("Texto de busca vazio!\n");
        return;
    }
    for (i = 0; i < total; i++) {
        if ((criterio == 2 && strstr(acoes[i].escola, texto) != NULL) ||
            (criterio == 3 && strstr(acoes[i].tema, texto) != NULL)) {
            exibirAcao(acoes[i]);
            achou = 1;
        }
    }
    if (!achou) {
        printf("Nenhuma acao encontrada.\n");
    }
}

void atualizarSituacao(Acao acoes[], int total)
{
    int codigo, pos, situacao, participantes;

    printf("\n===== ATUALIZAR SITUACAO =====\n");
    if (!lerInteiro("Codigo da acao: ", &codigo)) {
        printf("Codigo invalido!\n");
        return;
    }
    pos = buscarPorCodigo(acoes, total, codigo);
    if (pos == -1) {
        printf("Registro inexistente: nenhuma acao com o codigo %d.\n", codigo);
        return;
    }

    printf("Situacao atual: %s\n", nomeSituacao(acoes[pos].situacao));
    printf("1 - Planejada\n2 - Realizada\n3 - Cancelada\n");
    if (!lerInteiro("Nova situacao: ", &situacao) || situacao < 1 || situacao > 3) {
        printf("Situacao invalida! Escolha 1, 2 ou 3.\n");
        return;
    }

    if (situacao == REALIZADA) {
        if (!lerInteiro("Quantidade efetiva de participantes: ", &participantes) ||
            participantes < 0) {
            printf("Quantidade invalida! Situacao nao alterada.\n");
            return;
        }
        acoes[pos].efetivos = participantes;
    } else {
        acoes[pos].efetivos = 0;
    }
    acoes[pos].situacao = situacao;
    printf("\nSituacao atualizada com sucesso para %s!\n", nomeSituacao(situacao));
}

void registrarParticipantes(Acao acoes[], int total)
{
    int codigo, pos, participantes;

    printf("\n===== REGISTRAR PARTICIPANTES =====\n");
    if (!lerInteiro("Codigo da acao: ", &codigo)) {
        printf("Codigo invalido!\n");
        return;
    }
    pos = buscarPorCodigo(acoes, total, codigo);
    if (pos == -1) {
        printf("Registro inexistente: nenhuma acao com o codigo %d.\n", codigo);
        return;
    }
    if (acoes[pos].situacao != REALIZADA) {
        printf("A acao esta %s. Altere a situacao para REALIZADA (opcao 4) antes.\n",
               nomeSituacao(acoes[pos].situacao));
        return;
    }
    if (!lerInteiro("Quantidade efetiva de participantes: ", &participantes) ||
        participantes < 0) {
        printf("Quantidade invalida! Participantes nao alterados.\n");
        return;
    }
    acoes[pos].efetivos = participantes;
    printf("\nParticipantes registrados com sucesso!\n");
}

void gerarResumo(Acao acoes[], int total)
{
    int i, planejadas = 0, realizadas = 0, canceladas = 0;
    int efetivos = 0, previstos = 0;

    printf("\n===== RESUMO GERAL =====\n");
    if (total == 0) {
        printf("Nenhuma acao cadastrada. Nao ha dados para o resumo.\n");
        return;
    }
    for (i = 0; i < total; i++) {
        if (acoes[i].situacao == PLANEJADA) {
            planejadas++;
        } else if (acoes[i].situacao == REALIZADA) {
            realizadas++;
            efetivos = efetivos + acoes[i].efetivos;
            previstos = previstos + acoes[i].previstos;
        } else {
            canceladas++;
        }
    }

    printf("Total de acoes: %d\n", total);
    printf("Planejadas    : %d\n", planejadas);
    printf("Realizadas    : %d\n", realizadas);
    printf("Canceladas    : %d\n", canceladas);
    printf("Participantes (acoes realizadas): %d\n", efetivos);
    if (realizadas == 0) {
        printf("Percentual de participacao: sem acoes realizadas ainda.\n");
    } else {
        printf("Percentual de participacao: %.1f%%\n", efetivos * 100.0 / previstos);
    }
}

int main(void)
{
    Acao acoes[MAX_ACOES];
    int total = 0;
    int opcao = -1;

    while (opcao != 0) {
        exibirMenu();
        if (!lerInteiro("Digite uma opcao: ", &opcao)) {
            opcao = -1;
        }
        switch (opcao) {
            case 1: cadastrarAcao(acoes, &total); break;
            case 2: listarAcoes(acoes, total); break;
            case 3: pesquisarAcao(acoes, total); break;
            case 4: atualizarSituacao(acoes, total); break;
            case 5: registrarParticipantes(acoes, total); break;
            case 6: gerarResumo(acoes, total); break;
            case 0: printf("\nSistema encerrado!\n"); break;
            default: printf("\nOpcao invalida! Escolha uma opcao do menu.\n");
        }
    }

    printf("\nPressione ENTER para fechar...");
    getchar();
    return 0;
}
