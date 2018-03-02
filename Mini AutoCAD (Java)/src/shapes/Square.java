package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Square extends Shape implements DrawableShape {
    private int startX;
    private int startY;
    private int side;

    public Square() {
    }

    // Constructorul pentru Square obtine din input coordonatele punctului de
    // start, lungimea laturii patratului si culoarea si opacitatea pentru
    // contur si pentru interior
    public Square(final String[] input) {
        this.startX = Integer.parseInt(input[INPUT1]);
        this.startY = Integer.parseInt(input[INPUT2]);
        this.side = Integer.parseInt(input[INPUT3]);
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

    public final int getSide() {
        return side;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
