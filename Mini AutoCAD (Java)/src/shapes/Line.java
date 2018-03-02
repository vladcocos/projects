package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Line extends Shape implements DrawableShape {
    private int startX;
    private int startY;
    private int finalX;
    private int finalY;

    public Line() {
    }

    // Constructorul pentru Line obtine din input coordonatele punctelor de
    // start si de final, si culoarea si opacitatea pentru contur si pentru
    // interior
    public Line(final String[] input) {
        this.startX = Integer.parseInt(input[INPUT1]);
        this.startY = Integer.parseInt(input[INPUT2]);
        this.finalX = Integer.parseInt(input[INPUT3]);
        this.finalY = Integer.parseInt(input[INPUT4]);
        super.setFillColor(Integer.parseInt(input[INPUT5].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[INPUT6]));
    }

    public final int getStartX() {
        return startX;
    }

    public final int getStartY() {
        return startY;
    }

    public final int getFinalX() {
        return finalX;
    }

    public final int getFinalY() {
        return finalY;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
