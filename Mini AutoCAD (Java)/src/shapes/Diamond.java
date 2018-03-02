package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Diamond extends Shape implements DrawableShape {
    private int startX;
    private int startY;
    private int height;
    private int width;

    public Diamond() {
    }

    // Constructorul pentru Diamond obtine din input coordonatele punctului de
    // start, lungimea diagonalei verticale (inaltimea), lungimea diagonale
    // orizontale (latimea) si culoarea si opacitatea pentru contur si pentru
    // interior
    public Diamond(final String[] input) {
        this.startX = Integer.parseInt(input[INPUT1]);
        this.startY = Integer.parseInt(input[INPUT2]);
        this.height = Integer.parseInt(input[INPUT3]);
        this.width = Integer.parseInt(input[INPUT4]);
        super.setStrokeColor(Integer.parseInt(input[INPUT5].substring(1), HEX));
        super.setStrokeOpacity(Integer.parseInt(input[INPUT6]));
        super.setFillColor(Integer.parseInt(input[INPUT7].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[INPUT8]));
    }

    public final int getStartX() {
        return startX;
    }

    public final int getStartY() {
        return startY;
    }

    public final int getHeight() {
        return height;
    }

    public final int getWidth() {
        return width;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
