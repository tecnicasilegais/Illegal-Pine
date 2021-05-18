// **********************************************************************
// Pine Shooter
// Eduardo Andrade e Marcelo Heredia
// **********************************************************************


#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;
#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>
#include <nlohmann/json.hpp>
#else

#include <sys/time.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "headers/Ponto.h"
#include "headers/Poligono.h"

#include "headers/Temporizador.h"

Temporizador T;
double accum_delta_t = 0;

//declarando poligonos que serao utilizados
Poligono A;
vector<Ponto> B, binit;

//Limites logicos da area de desenho
Ponto Min, Max;
Ponto Meio, Terco, Largura;

bool desenha = false;

float angle = 0.0;
float walk = 0.0;

const string f_path = "data/";

/**
 * Le o arquivo nome e popula o poligono P
 */
void le_poligono(const string nome, Poligono &P)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    int nLinha = 0;
    unsigned int qtdVertices;

    input >> qtdVertices;

    for (int i = 0; i < qtdVertices; i++)
    {
        double x, y;
        // Le cada elemento da linha
        input >> x >> y;
        if (!input)
        {
            break;
        }
        nLinha++;
        P.insere_vertice(Ponto(x, y));
    }
    cout << "Poligono lido com sucesso!" << endl;

}

void init()
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    A.insere_vertice(Ponto(20,20));
    A.insere_vertice(Ponto(20,25));
    A.insere_vertice(Ponto(25,25));
    A.insere_vertice(Ponto(22.5,22.5));
    A.insere_vertice(Ponto(25,20));


    binit.emplace_back(20,20);
    binit.emplace_back(20,25);
    binit.emplace_back(25,25);
    binit.emplace_back(25,20);

    B.emplace_back(20,20);
    B.emplace_back(20,25);
    B.emplace_back(25,25);
    B.emplace_back(25,20);

    cout << "\tMinimo:";
    A.get_min().imprime();
    cout << "\tMaximo:";
    A.get_max().imprime();

    // Atualiza os limites globais apos cada leitura
    Min = Ponto(0,0) ;
    Max = Ponto(50,50);

    cout << "Limites Globais" << endl;
    cout << "\tMinimo:";
    Min.imprime();
    cout << "\tMaximo:";
    Max.imprime();
    cout << endl;

    // Ajusta a largura da janela logica
    // em funcao do tamanho dos poligonos
    Largura.x = Max.x - Min.x;
    Largura.y = Max.y - Min.y;

}

double n_frames = 0;
double tempo_total = 0;

void animate()
{
    double dt;
    dt = T.get_delta_t();
    accum_delta_t += dt;
    tempo_total += dt;
    n_frames++;

    if (accum_delta_t > 1.0 / 30) // fixa a atualizacao da tela em 30
    {
        accum_delta_t = 0;
        glutPostRedisplay();
    }
    if (tempo_total > 5.0)
    {
        cout << "Tempo Acumulado: " << tempo_total << " segundos. ";
        cout << "Nros de Frames sem desenho: " << n_frames << endl;
        cout << "FPS(sem desenho): " << n_frames / tempo_total << endl;
        tempo_total = 0;
        n_frames = 0;
    }
}

/**
 * trata o redimensionamento da janela OpenGL
 * @param w - largura
 * @param h - altura
 */
void reshape(int w, int h)
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x, Max.x,
            Min.y, Max.y,
            0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void CalculaPonto(Ponto p, Ponto &out) {

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];

}
Ponto p1, p2;
void display(void)
{

    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites logicos da area OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(22.5+walk, 22.5, 0);
    glRotatef(angle, 0,0,1);
    glTranslatef(-22.5, -22.5, 0);
    glLineWidth(2);
    glColor3f(0,1,0); // R, G, B  [0..1]
    A.desenha_poligono();


    for(int i=0; i<binit.size(); i++)
    {
        Ponto p;
        CalculaPonto(binit[i], p);
        B[i].x = p.x;
        B[i].y = p.y;
    }

    glPopMatrix();
    glLineWidth(0.5);
    glColor3f(1,0,0); // R, G, B  [0..1]
    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : B)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();

    glLineWidth(0.5);
    glColor3f(0,0,1); // R, G, B  [0..1]
    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : binit)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();
    //imprime p1_new e p2_new
    glutSwapBuffers();
}

/**
 * Informa quantos frames se passaram no tempo informado.
 * @param tempo - Tempo em segundos
 */
void conta_tempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while (true)
    {
        tempo -= T.get_delta_t();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:        // Termina o programa qdo
            exit(0);   // a tecla ESC for pressionada
            break;
        case 't':
            conta_tempo(3);
            break;
        case ' ':
            desenha = !desenha;
            break;
        case 'a':
            walk += -1;
            break;
        case 'd':
            walk += 1;
            break;
        default:
            break;
    }
}

void arrow_keys(int a_keys, int x, int y)
{
    switch (a_keys)
    {
        case GLUT_KEY_UP:       // Se pressionar UP
            glutFullScreen(); // Vai para Full Screen
            break;
        case GLUT_KEY_DOWN:     // Se pressionar UP
            // Reposiciona a janela
            glutPositionWindow(50, 50);
            glutReshapeWindow(700, 500);
            break;
        case GLUT_KEY_RIGHT:
            angle -= 1;
            break;
        case GLUT_KEY_LEFT:
            angle += 1;
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    cout << "Programa OpenGL" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize(800, 600);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow("Primeiro Programa em OpenGL");

    // executa algumas inicializacoes
    init();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // sera chamada automaticamente quando
    // for necessario redesenhar a janela
    glutDisplayFunc(display);

    // Define que o tratador de evento para
    // o invalidacao da tela. A funcao "display"
    // sera chamada automaticamente sempre que a
    // maquina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // sera chamada automaticamente quando
    // o usuario alterar o tamanho da janela
    glutReshapeFunc(reshape);

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // sera chamada automaticamente sempre
    // o usuario pressionar uma tecla comum
    glutKeyboardFunc(keyboard);

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" sera chamada
    // automaticamente sempre o usuario
    // pressionar uma tecla especial
    glutSpecialFunc(arrow_keys);

    // inicia o tratamento dos eventos
    glutMainLoop();

    return 0;
}
