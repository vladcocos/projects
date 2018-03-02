package tools;

import shapes.Shape;
import shapes.Line;
import shapes.Canvas;
import shapes.Circle;
import shapes.Square;
import shapes.Rectangle;
import shapes.Triangle;
import shapes.Diamond;
import shapes.Polygon;

public final class Factory {
    // Pattern-ul Singleton folosit pentru a instantia o singura clasa de tip
    // Factory
    private static Factory shapeFactory = null;

    private Factory() {
    };

    // Metoda returneaza o noua instanta a clasei Factory daca nu a fost deja
    // instantiata; daca a fost, se returneaza referinta catre aceasta
    public static Factory getFactory() {
        if (shapeFactory == null) {
            shapeFactory = new Factory();
        }

        return shapeFactory;
    }

    // Metoda implementeaza pattern-ul Factory de a returna un obiect de tip
    // diferit in functie de inputul primit; in acest caz, returneaza o figura
    // geometrica, bazat pe String-ul citit din fisierul de input, care contine
    // tipul formei geometrice si detalii despre aceasta
    public Shape getShape(final String inputLine) {
        String[] input = inputLine.split(" ");

        if (input[0].equals("CANVAS")) {
            return new Canvas(input);
        } else if (input[0].equals("LINE")) {
            return new Line(input);
        } else if (input[0].equals("CIRCLE")) {
            return new Circle(input);
        } else if (input[0].equals("SQUARE")) {
            return new Square(input);
        } else if (input[0].equals("RECTANGLE")) {
            return new Rectangle(input);
        } else if (input[0].equals("TRIANGLE")) {
            return new Triangle(input);
        } else if (input[0].equals("DIAMOND")) {
            return new Diamond(input);
        } else if (input[0].equals("POLYGON")) {
            return new Polygon(input);
        }

        return null;
    }
}
