package tools;

import image.Image;

public interface DrawableShape {
    // Interfata necesara obiectelor de tip visitable (figurile geometrice);
    // contine constante pentru indecsii elementelor din vectorul de input
    // (contine elementele ce descriu fiecare figura geometrice) si metoda
    // getDrawn (metoda "accept") ce primeste ca parametri drawTool (visitor) si
    // imaginea
    int INPUT1 = 1;
    int INPUT2 = 2;
    int INPUT3 = 3;
    int INPUT4 = 4;
    int INPUT5 = 5;
    int INPUT6 = 6;
    int INPUT7 = 7;
    int INPUT8 = 8;
    int INPUT9 = 9;
    int INPUT10 = 10;
    int HEX = 16;

    void getDrawn(DrawTool drawTool, Image image);
}
