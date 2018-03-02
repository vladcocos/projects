package tools;

import image.Image;
import shapes.Canvas;
import shapes.Line;
import shapes.Square;
import shapes.Rectangle;
import shapes.Circle;
import shapes.Triangle;
import shapes.Diamond;
import shapes.Polygon;

public interface DrawTool {
    // Interfata necesara obiectelor de tip "visitor" (unealta de desenat).
    // Contine constantele pentru numarul necesar pentru shiftare (octetul cel
    // mai semnificativ trebuie sa contina valoarea alpha a culorii), numarul de
    // puncte ale unui triunghi (necesar clasei Triangle) si metoda draw
    // supraincarcata pentru fiecare tip de obiect "visitable" (fiecare figura
    // geometrica).
    int ALPHA_SHIFT = 24;
    int TRIANGLE_POINTS = 3;

    void draw(Canvas canvas, Image image);

    void draw(Line line, Image image);

    void draw(Square square, Image image);

    void draw(Rectangle rectangle, Image image);

    void draw(Circle circle, Image image);

    void draw(Triangle triangle, Image image);

    void draw(Diamond diamond, Image image);

    void draw(Polygon polygon, Image image);
}
