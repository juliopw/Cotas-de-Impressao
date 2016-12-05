#include <iostream> // Biblioteca para mostrar as informações na tela
#include <locale.h> // Biblioteca para que o programa entenda a acentuação do português
#include <ctime> // Biblioteca de hora atual
#include <fstream> // Biblioteca para trabalhar com arquivos
#include <stdlib.h> // Obter o nome de usuário do Windows
#include <Windows.h> // Biblioteca específica do Windows

using namespace std; // Necessário para que o código fique mais limpo

// Retorna o dia atual no formato dd/mm/aaaa
const string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d/%m/%Y", &tstruct);
    return buf;
}

// Copia o resultado para a área de transferência
void toClipboard(HWND hwnd, const string& s)
{
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

int main()
{
    // Roda o programa em Português
    setlocale(LC_ALL, "Portuguese");

    // Cria as variáveis do programa
    string nome, resultado, mascFem, numPages, codPessoa;

    // Mostra na tela o texto
    cout << "\n\t\t\tCotas v0.2\n\n\tDigite as informações e aperte a tecla "
            "enter\n\n\tDigite o número de páginas: ";
    getline(cin, numPages); // Pede que o usuário digite as informações

    cout << "\n\tCódigo de pessoa: ";
    getline(cin, codPessoa);

    cout << "\n\tNome: ";
    getline(cin, nome);

    mascFem = "0"; // Reseta o verificador de gênero

    // Pede ao usuário o gênero. Roda até que o gênero seja m, M, f ou F.
    while (mascFem == "0" || (mascFem != "m" && mascFem != "M" && mascFem != "F" && mascFem != "f")) {
        cout << "\n\tGênero (m ou f): ";
        getline(cin, mascFem);
    }

    // Define o texto dependendo do gênero do aluno
    if (mascFem == "m" || mascFem == "M") {
        mascFem = "do aluno";
    }
    else if (mascFem == "f" || mascFem == "F") {
        mascFem = "da aluna";
    }

    // Verifica se o texto deve estar no singular ou no plural
    string pages;
    if (numPages == "1") {
        pages = "página";
    }
    else {
        pages = "páginas";
    }

    // Junta todas as informações de texto em uma só
    // currentDateTime() pega a data atual e coloca no texto
    resultado = "Solicito reconstituição de " + numPages + " " + pages + " da quota de impressão " + mascFem + " " + nome + " - código de pessoa nº " + codPessoa + ". Data: " + currentDateTime() + " UR: 511014016122.\n\n";

    // Coloca o resultado na área de transferência
    HWND hwnd = GetDesktopWindow();
    toClipboard(hwnd, resultado);

    // Pega o endereço da pasta de usuário do Windows
    char* pPath;
    pPath = getenv("HOMEPATH");

    // Com o nome de usuário, define que o arquivo será salvo na área de trabalho
    string path = string(pPath) + "/Desktop/Cotas.txt";

    // abre o navegador diretamente no link Chamados TI
    ShellExecute(NULL, "open", "https://intranet.univali.br/intranet/servicoIntranet.php?srvId=5", NULL, NULL, SW_SHOWNORMAL);

    // Abre o arquivo e escreve o resultado na última linha
    // Cria o arquivo, se este não existir
    ofstream myfile;
    myfile.open(path.c_str(), ios_base::app);
    myfile << resultado;

    // Salva o arquivo
    myfile.close();
}
