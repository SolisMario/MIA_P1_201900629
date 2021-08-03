#include "iostream"
#include "parser.h"  // Nuestro parser
#include "scanner.h"  // Nuestro scanner

using namespace std;

extern int yyparse();
int p;
extern int linea;
extern int yylineno;

int main(int argc, char *argv[]) {

    string ingrese = "------------------------------Ingrese un comando------------------------------\n";

    string line = "";

    while (line != "exit") {    //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << ingrese;
        getline(cin, line);
        if (line != "exit") {
            if (line != "") {
                YY_BUFFER_STATE buffer = yy_scan_string(line.c_str());

                if (yyparse() == 0) {
                    printf("\nComando ejecutado correctamente\n\n");
                } else {
                    printf("\nNo se pudo ejecutar el comando\n\n");
                }
                yy_delete_buffer(buffer);
            }
        }
    }
}