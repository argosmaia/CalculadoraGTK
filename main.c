#include <gtk/gtk.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief declaração de funções
 * on_button_clicked: é a função de callback do botão, serve para calcular a
 * expressão.
 * 
 * on_window_destroy: é a função de callback da janela, serve para fechar a
 * janela.
 * 
 * validate_expression: é uma função que valida a expressão, usa regex para
 * validar se uma expressão tem caracteres inválidos. 
*/

static void on_button_clicked(GtkWidget *widget, gpointer data);
static void on_window_destroy(GtkWidget *widget, gpointer data);
static gboolean validate_expression(const char *expression);

/**
 * @brief 
 * entry: é um ponteiro para um widget do tipo GtkEntry.
 * window: é um ponteiro para um widget do tipo GtkWindow.
 * grid: é um ponteiro para um widget do tipo GtkGrid.
 * buttons: é um vetor de ponteiros para widgets do tipo GtkButton. 
*/
GtkWidget *entry;
GtkWidget *window;
GtkWidget *grid;
GtkWidget *buttons[18] = {};

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *) data;
    const char *expression = gtk_entry_get_text(GTK_ENTRY(entry));
    if (validate_expression(expression)) {
        double result = 0;
        char *endptr; //endptr é um ponteiro para char que aponta para o primeiro caractere não convertido.
        result = strtod(expression, &endptr);
        //strtod: função da biblioteca padrão do C que converte uma string para
        if (result == 0 && endptr == expression) {
            gtk_entry_set_text(GTK_ENTRY(entry), "Erro");
        } else {
            gtk_entry_set_text(GTK_ENTRY(entry), endptr);
        }
    } else {
        gtk_entry_set_text(GTK_ENTRY(entry), "Erro");
    }
}

static void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static gboolean validate_expression(const char *expression) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    reti = regcomp(&regex, "^[0-9+\\-*/^()\\. ]+$", 0);
    if(reti) {
        fprintf(stderr, "Não consegui compilar a expressão gerada\n");
        exit(1);
    }

    reti = regexec(&regex, expression, 0, NULL, 0);
    if (!reti) {
        return TRUE;
    } else if(reti == REG_NOMATCH) {
        return FALSE;
    } else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "combinação falhou: %s\n", msgbuf);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // Inicialização do GTK
    gtk_init(&argc, &argv);

    // Criação da janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculadora");
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    gtk_main();
}
