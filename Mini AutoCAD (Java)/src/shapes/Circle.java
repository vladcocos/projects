package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Circle extends Shape implements DrawableShape {
    private int startX;
    private int startY;
    private int radius;

    public Circle() {
    }

    // Constructorul pentru Circle obtine din input coordonatele centrului,
    // raza, culoarea si opacitatea pentru contur si pentru interior
    public Circle(final String[] input) {
        this.startX = Integer.parseInt(input[INPUT1]);
        this.startY = Integer.parseInt(input[INPUT2]);
        this.radius = Integer.parseInt(input[INPUT3]);
        super.setStrokeColor(Integer.parseInt(input[INPUT4].substring(1), HEX));
        super.setStrokeOpacity(Integer.parseInt(input[INPUT5]));
        super.setFillColor(Integer.parseInt(input[INPUT6].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[INPUT7]));
    }

    public final int getStartX() {
        return startX;
    }

    public final int getStartY() {
        return startY;
    }

    public final int getRadius() {
        return radius;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
