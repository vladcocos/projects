package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Polygon extends Shape implements DrawableShape {
    private int numberOfPoints;
    private int[] xPoints;
    private int[] yPoints;

    public Polygon() {
    }

    // Constructorul pentru Polygon obtine din input coordonatele tuturor
    // punctelor poligonului si culoarea si opacitatea pentru contur si pentru
    // interior
    public Polygon(final String[] input) {
        this.numberOfPoints = Integer.parseInt(input[1]);
        xPoints = new int[numberOfPoints];
        yPoints = new int[numberOfPoints];

        for (int i = 0; i < numberOfPoints; i++) {
            this.xPoints[i] = Integer.parseInt(input[2 * i + INPUT2]);
            this.yPoints[i] = Integer.parseInt(input[2 * i + INPUT3]);
        }

        super.setStrokeColor(Integer.parseInt(input[input.length - INPUT4].substring(1), HEX));
        super.setStrokeOpacity(Integer.parseInt(input[input.length - INPUT3]));
        super.setFillColor(Integer.parseInt(input[input.length - INPUT2].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[input.length - INPUT1]));
    }

    public final int getNumberOfPoints() {
        return numberOfPoints;
    }

    public final int[] getXPoints() {
        return xPoints;
    }

    public final int[] getYPoints() {
        return yPoints;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
