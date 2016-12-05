#include <iostream> // Biblioteca para mostrar as informa��es na tela
#include <locale.h> // Biblioteca para que o programa entenda a acentua��o do portugu�s
#include <ctime> // Biblioteca de hora atual
#include <fstream> // Biblioteca para trabalhar com arquivos
#include <stdlib.h> // Obter o nome de usu�rio do Windows
#include <Windows.h> // Biblioteca espec�fica do Windows

using namespace std; // Necess�rio para que o c�digo fique mais limpo

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

// Copia o resultado para a �rea de transfer�ncia
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
    // Roda o programa em Portugu�s
    setlocale(LC_ALL, "Portuguese");

    // Cria as vari�veis do programa
    string nome, resultado, mascFem, numPages, codPessoa;

    // Mostra na tela o texto
    cout << "\n\t\t\tCotas v0.2\n\n\tDigite as informa��es e aperte a tecla "
            "enter\n\n\tDigite o n�mero de p�ginas: ";
    getline(cin, numPages); // Pede que o usu�rio digite as informa��es

    cout << "\n\tC�digo de pessoa: ";
    getline(cin, codPessoa);

    cout << "\n\tNome: ";
    getline(cin, nome);

    mascFem = "0"; // Reseta o verificador de g�nero

    // Pede ao usu�rio o g�nero. Roda at� que o g�nero seja m, M, f ou F.
    while (mascFem == "0" || (mascFem != "m" && mascFem != "M" && mascFem != "F" && mascFem != "f")) {
        cout << "\n\tG�nero (m ou f): ";
        getline(cin, mascFem);
    }

    // Define o texto dependendo do g�nero do aluno
    if (mascFem == "m" || mascFem == "M") {
        mascFem = "do aluno";
    }
    else if (mascFem == "f" || mascFem == "F") {
        mascFem = "da aluna";
    }

    // Verifica se o texto deve estar no singular ou no plural
    string pages;
    if (numPages == "1") {
        pages = "p�gina";
    }
    else {
        pages = "p�ginas";
    }

    // Junta todas as informa��es de texto em uma s�
    // currentDateTime() pega a data atual e coloca no texto
    resultado = "Solicito reconstitui��o de " + numPages + " " + pages + " da quota de impress�o " + mascFem + " " + nome + " - c�digo de pessoa n� " + codPessoa + ". Data: " + currentDateTime() + " UR: 511014016122.\n\n";

    // Coloca o resultado na �rea de transfer�ncia
    HWND hwnd = GetDesktopWindow();
    toClipboard(hwnd, resultado);

    // Pega o endere�o da pasta de usu�rio do Windows
    char* pPath;
    pPath = getenv("HOMEPATH");

    // Com o nome de usu�rio, define que o arquivo ser� salvo na �rea de trabalho
    string path = string(pPath) + "/Desktop/Cotas.txt";

    // abre o navegador diretamente no link Chamados TI
    ShellExecute(NULL, "open", "https://intranet.univali.br/intranet/servicoIntranet.php?srvId=5", NULL, NULL, SW_SHOWNORMAL);

    // Abre o arquivo e escreve o resultado na �ltima linha
    // Cria o arquivo, se este n�o existir
    ofstream myfile;
    myfile.open(path.c_str(), ios_base::app);
    myfile << resultado;

    // Salva o arquivo
    myfile.close();
}
